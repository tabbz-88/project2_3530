#include <fstream>
#include <sstream>
#include <iostream>
#include "Taxon.h"
#include <cctype>
using namespace std;

void loadName(const string &filename, Taxon &graph) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open " << filename << "\n";
        return;
    }

    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string temp;
        string id, parentID, name;
        int col = 0;

        while (getline(ss, temp, '\t')) {
            if (col == 0) id = temp;            // col:ID
            else if (col == 1) parentID = temp; // col:parentID
            else if (col == 4) name = temp;     // col:scientificName
            ++col; 
        }

        for (char &c : name) {
            c = tolower(static_cast<unsigned char>(c));
        }

        graph.scientificName(id, name);
        if (!parentID.empty())
            graph.relation(id, parentID);
    }
    cout << "Loaded " << filename << "\n";
}

void loadVernacularName(const string &filename, Taxon &graph) {
    ifstream file(filename);
    if (!file.is_open()) 
        return;

    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string temp;
        string taxonID, name;
        int col = 0;

        while (getline(ss, temp, '\t')) {
            if (col == 0) taxonID = temp;       // col:taxonID
            else if (col == 2) name = temp;     // col:name
            ++col; 
        }

        for (char &c : name) {
            c = tolower(static_cast<unsigned char>(c));
        }

        graph.commonName(taxonID, name);
    }
    cout << "Loaded " << filename << "\n";
}
