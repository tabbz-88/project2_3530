#ifndef TAXON_H
#define TAXON_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

class Taxon {
public:
    void scientificName(const string &id, const string &sci);
    void commonName(const string &id, const string &common);
    void relation(const string &childID, const string &parentID); // constructing graph edges

    string nameToID(const string &name) const;
    string getName(const string &id) const;

    vector<string> getAncestors(const string &id) const;
    string commonAncestor(const string &a, const string &b) const; // common ancestor for species a and species b
    vector<string> findPath(const string &a, const string &b) const; // path for graph
    unordered_map<string, int> depth;
    void computeDepths();
    double relatedness(const string &a, const string &b) const;


    //algorithms
    vector<string> BFS(const string &start, const string &target) const; // check documentation for references!!!
    vector<string> DFS(const string &start, const string &target) const; // check documentation for references!!!
    bool helperDFS(const string &current, const string &target, vector<string> &path) const;

    unordered_map<string, string> parent;               // child -> parent
    unordered_map<string, vector<string>> children;     // parent -> child
    unordered_map<string, string> name;                 // id -> scientific
    unordered_map<string, string> scientificID;         // scientific -> id
    unordered_map<string, string> commonID;             // common -> id
};

#endif