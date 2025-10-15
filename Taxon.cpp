#include "Taxon.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

bool Taxon::load(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open " << filename << "\n";
        return false;
    }

    string line; // read line, check tsv files for format
    getline(file, line); // skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string tmp;
        string id, parentId, name;
        int col = 0;

        while (getline(ss, tmp, '\t')) {
            if (col == 0) id = tmp;           // col:ID
            else if (col == 1) parentId = tmp; // col:parentID
            else if (col == 4) name = tmp;     // col:scientificName
            ++col;
        }

        if (!id.empty() && !name.empty()) {
            taxon[id] = {id, parentId, name};
            sciToId[name] = id;
        }
    }

    cout << "Loaded taxonomy: " << taxon.size() << " entries.\n";
    return true;
}

const Node* Taxon::getNode(const string &id) const { // find taxon in hash
    auto it = taxon.find(id);
    return it != taxon.end() ? &it->second : nullptr;
}

string Taxon::getScientificID(const string &name) const { // get scientific name
    auto it = sciToId.find(name);
    return it != sciToId.end() ? it->second : "";
}

vector<string> Taxon::getAncestors(const string &id) const { // path for ancestor in hash
    vector<string> path;
    string cur = id;
    while (!cur.empty()) {
        auto it = taxon.find(cur);
        if (it == taxon.end()) break;
        path.push_back(cur);
        cur = it->second.parentId;
    }
    return path;
}
