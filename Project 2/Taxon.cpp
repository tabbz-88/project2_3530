#include "Taxon.h"
#include <algorithm>
#include <queue>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

void Taxon::scientificName(const string &id, const string &sci) { // scientific name and id
    name[id] = sci;
    scientificID[sci] = id;
}

void Taxon::commonName(const string &id, const string &common) { // common name from id
    commonID[common] = id;
}

void Taxon::relation(const string &childID, const string &parentID) { // map child to parent
    parent[childID] = parentID;
    children[parentID].push_back(childID); // point parent to child, bidirectional
}

string Taxon::nameToID(const string &input) const { // read input and get id from common/scientific
    if (scientificID.count(input)) 
        return scientificID.at(input);
    if (commonID.count(input)) 
        return commonID.at(input);
    return "";
}

string Taxon::getName(const string &id) const {
    auto found = name.find(id);
    if (found != name.end())
        return found->second; // point to pair
    return id;
}

vector<string> Taxon::getAncestors(const string &id) const { // get chain of ancestors
    vector<string> ancestors;
    string current = id;
    while (parent.count(current)) {
        current = parent.at(current);
        ancestors.push_back(current);
    }
    return ancestors;
}

string Taxon::commonAncestor(const string &a, const string &b) const { // common ancestor for species a and species b
    auto A = getAncestors(a);
    auto B = getAncestors(b);
    unordered_set<string> setA(A.begin(), A.end()); // compare set of A to vector of B
    for (const auto &i : B)
        if (setA.count(i)) 
            return i;
    return "";
}

vector<string> Taxon::findPath(const string &a, const string &b) const {
    string lca = commonAncestor(a, b);
    if (lca.empty()) 
        return {};

    auto pathA = DFS(a, lca);
    auto pathB = DFS(b, lca);

    reverse(pathB.begin(), pathB.end()); // reverse in descending order
    pathB.erase(pathB.begin()); // back tracking on the path from B

    pathA.insert(pathA.end(), pathB.begin(), pathB.end()); // writing out path from A to B
    return pathA;
}

void Taxon::computeDepths() {
    queue<string> q;

    // find all roots
    unordered_set<string> roots;
    for (auto &p : parent) roots.insert(p.first);
    for (auto &p : parent) roots.insert(p.second);

    for (auto &node : roots) {
        if (parent.find(node) == parent.end() || parent.at(node).empty()) {
            depth[node] = 0;  // root
            q.push(node);
        }
    }

    while (!q.empty()) {
        string current = q.front();
        q.pop();
        for (auto &child : children[current]) {
            depth[child] = depth[current] + 1;
            q.push(child);
        }
    }
    cout << "Loaded depths.\n";
}

double Taxon::relatedness(const string &a, const string &b) const {
    string lca = commonAncestor(a, b);
    if (lca.empty()) 
        return 0.0;

    int depthA = depth.at(a);
    int depthB = depth.at(b);
    int depthAncestor = depth.at(lca);

    double rel = double(depthAncestor) / double(max(depthA, depthB));
    return rel * 100.0;
}

vector<string> Taxon::BFS(const string &start, const string &target) const { // modified BFS from geeksforgeeks
    queue<vector<string>> q;
    q.push({start});

    while (!q.empty()) {
        auto path = q.front(); 
        q.pop();
        string last = path.back();
        if (last == target) 
            return path;
        if (parent.count(last)) {
            auto next = path;
            next.push_back(parent.at(last));
            q.push(next);
        }
    }
    return {};
}

bool Taxon::helperDFS(const string &current, const string &target, vector<string> &path) const { // modified DFS helper from geeksforgeeks
    path.push_back(current);
    if (current == target) 
        return true;
    if (parent.count(current)) { // visit adjacent vertices
        if (helperDFS(parent.at(current), target, path)) 
            return true;
    }
    path.pop_back();
    return false;
}

vector<string> Taxon::DFS(const string &start, const string &target) const { // modified DFS from geeksforgeeks
    vector<string> path;
    helperDFS(start, target, path);
    return path;
}