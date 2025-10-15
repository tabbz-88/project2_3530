#ifndef VERNACULAR_H
#define VERNACULAR_H

#include <string>
#include <unordered_map>
using namespace std;

class CommonName {
public:
    bool load(const string &filename);
    string getCommonID(const string &name) const;

private:
    unordered_map<string, string> commonToId;
};

#endif
