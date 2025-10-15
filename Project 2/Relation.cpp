#include "Relation.h"
#include <algorithm>
using namespace std;

string Relation::findLCA(const Taxon &taxon, const string &a, const string &b) { // find LCA by searching through the path of both species
    auto pathA = taxon.getAncestors(a); // vector of ids as path
    auto pathB = taxon.getAncestors(b);

    reverse(pathA.begin(), pathA.end()); // reverse in descending order
    reverse(pathB.begin(), pathB.end());

    string lca;
    for (size_t i = 0; i < pathA.size() && i < pathB.size(); ++i) { // find matching node
        if (pathA[i] == pathB[i]) lca = pathA[i];
        else break;
    }
    return lca;
}

double Relation::compute(const Taxon &taxon, const string &a, const string &b) {
    auto path1 = taxon.getAncestors(a);
    auto path2 = taxon.getAncestors(b);
    string lca = findLCA(taxon, a, b);
    if (lca.empty()) return 0.0;

    int depth1 = path1.size();
    int depth2 = path2.size();
    int depthLCA = taxon.getAncestors(lca).size();

    return (2.0 * depthLCA / (depth1 + depth2)) * 100.0;
}
