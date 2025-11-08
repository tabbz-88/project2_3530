#include <iostream>
#include <chrono>
#include <algorithm>
#include <cstring>
#include "httplib.h"
#include "Taxon.h"
using namespace std;

// Html for page 1
const char *html = R"(
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Critter Connections!</title>
<style>
body {
	background-color: #a7e0a2
}
   .container {
       position: relative;
       width: device-width;
       height: device-height;
       display: flex;
       justify-content: center;
       margin: 0;
   }

   .rectangle {
       width: 400px;
       height: 150px;
       position: absolute;
   }

   .one {
       background-color: #22b512;
       border-radius: 15px;
       z-index: -1;
   }

   .two {
       background-color: #22b512;
       top: 200px;
       border-radius: 15px;
       z-index: -1;
   }
}
</style>
</head>
<body>
   <div class="container">
       <div class="rectangle one"></div>
       <div class="rectangle two"></div>
   </div>
	<font color=#824c1d><h1 style="text-align: center;">Animal 1</h1></font>
    <br>
    <form style="text-align: center;" id="formElem">
  	<input style="text-align: center;" type="text" id="name1" name="name1" placeholder="Enter first name">
    <br>
    <br>
    <br>
    <br>
    <br>
	<font color=#824c1d><h1 style="text-align: center;">Animal 2</h1></font>
    <br>
    <input style="text-align: center;" type="text" id="name2" name="name2" placeholder="Enter second name">
    <br>
    <br>
    <br>
    <br>
    <input style="text-align: center;" type="submit">
	</form>
</body>
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
</html>
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

    vector<string> path;
    string lca;
    double relationship;
    double bfs;
    double dfs;

    // server start
    Server svr;

    svr.Get("/", [](const Request &req, Response &res) {
        res.set_content(html, "text/html");
    });

    svr.Get("/display", [&inputA, &inputB, &graph, &path, &lca, &relationship, &bfs, &dfs](const Request &req, Response &res) {

        // all data needed loaded through here
        // vars: array path, string lca, double relationship, double bfs, double dfs
        // vars: string name1, string name2
        string data = "var path = [";
        for (const auto &i : path) {
            data += '"' + graph.getName(i) + R"(", )";
        }
        data = data.substr(0, data.length() - 2) + "];\n";
        data += R"(var lca = ")" + graph.getName(lca) + R"(";)" + "\n";
        data += "var relationship = " + to_string(relationship) + ";\n";
        data += "var bfs = " + to_string(bfs) + ";\n";
        data += "var dfs = " + to_string(dfs) + ";\n";
        data += R"(var name1 = ")" + inputA + R"(";)" + "\n";
        data += R"(var name2 = ")" + inputB + R"(";)" + "\n";


        // html/css/javascript for second page
        // data needed is included in data variable and can access any of that data in this code
        string html2 = R"(
        <body>
            <p id="firstName"></p>
            <p id="secondName"></p>


        <script>
            )" + data +
            R"(
        document.getElementById("firstName").innerHTML = name1;
        document.getElementById("secondName").innerHTML = name2;
        </script>
        </body>
        )";


        const char *page =  html2.c_str(); // html for page 2
        res.set_content(page, "text/html");
    });


    svr.Post("/post", [&inputA, &inputB, &idA, &idB, &graph, &path, &lca, &relationship, &bfs, &dfs](const Request &req, Response &res) {
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
        lca = graph.commonAncestor(idA, idB);
        if (lca.empty()) {
            res.set_content("noCommon", "text/plain");
            return;
        }


        // path between species
        path = graph.findPath(idA, idB);
        if (path.empty()) {
            res.set_content("noPath", "text/plain");
            return;
        }

        // display
        cout << "\nCommon ancestor: " << graph.getName(lca) << "\n";

        relationship = graph.relatedness(idA, idB);
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

        bfs = chrono::duration<double, micro>(t2 - t1).count();
        dfs = chrono::duration<double, micro>(t3 - t2).count();
        cout << "\nBFS time: " << bfs << " microseconds\n";
        cout << "DFS time: " << dfs << " microseconds\n";


    });

    svr.listen("localhost", 8080);
};
