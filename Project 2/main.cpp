#include <iostream>
#include <chrono>
#include <algorithm>
#include "httplib.h"
#include "Taxon.h"
using namespace std;

// Html for page 1
const char *html = R"(
<form id="formElem">
  <label for="name1">Animal 1:</label>
  <input type="text" id="name1" name="name1" placeholder="Enter first name">
    <label for="name2">Animal 2:</label>
  <input type="text" id="name2" name="name2" placeholder="Enter second name">
    <input type="submit"/>
</form>

<script>
  formElem.onsubmit = async (e) => {
    e.preventDefault();
    let res = await fetch('/post?name1=' + document.getElementById('name1').value + '&name2=' + document.getElementById('name2').value, {
      method: 'POST',
      body: new FormData(formElem)
    });
    let form = await res.text()
    if(form == "empty") {
        alert("One or more of your inputs are empty");
    } else if (form == "invalidA") {
        alert("Invalid first species. Please try again.");
    } else if (form == "invalidB") {
       alert("Invalid second species. Please try again.");
    } else if (form == "noCommon") {
       alert("No common ancestor found.");
    } else if (form == "noPath") {
       alert("No path found between the two species.");
    } else {
       window.location.assign("/display");
    }
  };
</script>
)";

void loadName(const string &, Taxon &);
void loadVernacularName(const string &, Taxon &);

int main() {
    using namespace httplib;

    // loading
    Taxon graph;
    cout << "Loading...\n";
    loadName("NameUsage.tsv", graph);
    loadVernacularName("VernacularName.tsv", graph);
    graph.computeDepths();
    cout << "Done! Visit localhost:8080\n\n";

    string inputA, inputB, idA, idB;

    // server start
    Server svr;

    svr.Get("/", [](const Request &req, Response &res) {
        res.set_content(html, "text/html");
    });

    svr.Get("/display", [](const Request &req, Response &res) {
        const char *html2 = R"(
        )"; // html for page 2
        res.set_content(html2, "text/html");
    });

    svr.Post("/post", [&inputA, &inputB, &idA, &idB, &graph](const Request &req, Response &res) {
        inputA = req.has_param("name1") ? req.get_param_value("name1") : "";
        inputB = req.has_param("name2") ? req.get_param_value("name2") : "";
        if (inputA.empty() || inputB.empty()) { // invalid input
            res.set_content("empty", "text/plain");
            return;
        }
        idA = graph.nameToID(inputA);
        if (idA.empty()) { // invalid species
            res.set_content("invalidA", "text/plain");
            return;
        }
        idB = graph.nameToID(inputB);
        if (idB.empty()) { // invalid species
            res.set_content("invalidB", "text/plain");
            return;
        }

        // common ancestor
        string lca = graph.commonAncestor(idA, idB);
        if (lca.empty()) {
            res.set_content("noCommon", "text/plain");
            return;
        }

        // path between species
        auto path = graph.findPath(idA, idB);
        if (path.empty()) {
            res.set_content("noPath", "text/plain");
            return;
        }

        // display
        cout << "\nCommon ancestor: " << graph.getName(lca) << "\n";

        double relationship = graph.relatedness(idA, idB);
        cout << "Relatedness: " << relationship << "%\n\n";

        cout << "Path:\n";
        for (const auto &i : path) {
            cout << "   " << graph.getName(i) << "\n";
        }

        // compare BFS and DFS
        auto t1 = chrono::high_resolution_clock::now();
        graph.BFS(idA, lca);
        auto t2 = chrono::high_resolution_clock::now();
        graph.DFS(idA, lca);
        auto t3 = chrono::high_resolution_clock::now();

        cout << "\nBFS time: " << chrono::duration<double, micro>(t2 - t1).count() << " microseconds\n";
        cout << "DFS time: " << chrono::duration<double, micro>(t3 - t2).count() << " microseconds\n";
    });

    svr.listen("localhost", 8080);
};
