#include "CommonName.h"
#include "Utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

bool CommonName::load(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open " << filename << "\n";
        return false;
    }

    string line; // read line, check tsv files for format
    getline(file, line); // skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string tmp, taxonId, commonName;
        int col = 0;

        while (getline(ss, tmp, '\t')) {
            if (col == 0) taxonId = tmp;         // col:taxonID
            else if (col == 2) commonName = tmp; // col:name
            ++col;
        }

        if (!taxonId.empty() && !commonName.empty()) {
            toLower(commonName);
            commonToId[commonName] = taxonId;
        }
    }

    cout << "Loaded common names: " << commonToId.size() << " entries.\n";
    return true;
}

string CommonName::getCommonID(const string &name) const {
    string lowered = name;
    toLower(lowered);
    auto it = commonToId.find(lowered);
    return it != commonToId.end() ? it->second : "";
}
