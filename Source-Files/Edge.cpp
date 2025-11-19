#ifndef EDGE_H
#define EDGE_H

#include <string>
using namespace std;

struct Edge {
    string source;
    string destination;
    int weight;
    
    Edge(string s, string d, int w) : source(s), destination(d), weight(w) {}
};

#endif