#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <algorithm>
using namespace std;

inline void toLower(string &s) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return tolower(c); });
}

#endif
