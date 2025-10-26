#include <iostream>
#include <chrono>
#include <algorithm>
#include "Taxon.h"
using namespace std;

void loadName(const string &, Taxon &);
void loadVernacularName(const string &, Taxon &);

int main() {
    Taxon graph;
    cout << "Loading...\n";
    loadName("NameUsage.tsv", graph);
    loadVernacularName("VernacularName.tsv", graph);
    graph.computeDepths();
    cout << "Done!\n\n";

    string inputA, inputB, idA, idB;

    // first input
    while (true) {
        cout << "Enter first species (scientific or common name): ";
        getline(cin, inputA);
        if (inputA.empty()) { // invalid input
            cerr << "Invalid input. Please try again.\n";
            continue;
        }

        idA = graph.nameToID(inputA);
        if (idA.empty()) { // invalid species
            cerr << "Invalid species (try capitalizing or lowercasing). Please try again.\n";
            continue;
        }
        break;
    }

    // second input
    while (true) {
        cout << "Enter second species (scientific or common name): ";
        getline(cin, inputB);
        if (inputB.empty()) { // invalid input
            cerr << "Invalid input. Please try again.\n";
            continue;
        }

        idB = graph.nameToID(inputB);
        if (idB.empty()) { // invalid species
            cerr << "Invalid species (try capitalizing or lowercasing). Please try again.\n";
            continue;
        }
        break;
    }

    // common ancestor
    string lca = graph.commonAncestor(idA, idB);
    if (lca.empty()) {
        cerr << "No common ancestor found.\n";
        return 1;
    }

    // path between species
    auto path = graph.findPath(idA, idB);
    if (path.empty()) {
        cerr << "No path found between the two species.\n";
        return 1;
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

    cout << "\nPress Enter to exit...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get(); // still cant figure out why this takes so long to exit, exe istantly exits without it though

    return 0;
}
