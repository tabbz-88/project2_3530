#ifndef RELATION_H
#define RELATION_H

#include "Taxon.h"
using namespace std;

class Relation {
public:
    static string findLCA(const Taxon &taxon, const std::string &a, const std::string &b); // find least common ancestor
    static double compute(const Taxon &taxon, const std::string &a, const std::string &b); 
    // compute percentage based on (2 * depth(LCA)) / (depth(species1) + depth(species2)) * 100
};

#endif
