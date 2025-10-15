#ifndef TAXON_H
#define TAXON_H

#include <string>
#include <unordered_map> // hash
#include <vector>
using namespace std;

struct Node {
    string id;
    string parentId;
    string name; // scientific name
};

class Taxon {
public:
    bool load(const string &filename);
    const Node* getNode(const string &id) const;
    string getScientificID(const string &name) const;
    vector<string> getAncestors(const string &id) const;

    unordered_map<string, Node> taxon;
    unordered_map<string, string> sciToId;
};

#endif
