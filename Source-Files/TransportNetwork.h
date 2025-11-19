#ifndef TRANSPORTNETWORK_H
#define TRANSPORTNETWORK_H

#include "Route.h"
#include <vector>
#include <string>

using namespace std;

struct Edge {
    string source;
    string destination;
    int weight;
    
    Edge(string src, string dest, int w) : source(src), destination(dest), weight(w) {}
};

class TransportNetwork {
private:
    vector<Route> routes;
    vector<string> stops;
    int** adjMatrix;
    int size;
    static const int INF;
    
    void rebuildAdjMatrix();
    int getStopIndex(string stopName);
    
public:
    TransportNetwork();
    ~TransportNetwork();
    
    void addRoute(Route route);
    vector<string> dijkstra(string origin, string destination);
    vector<string> BFS(string start);
    vector<string> DFS(string start);
    vector<Edge> kruskal();
    vector<Edge> prim();
    void printGraph();
    
    vector<Route> getRoutes();
    vector<string> getAllStops();
    int getSize();
    int** getMatrix();
};

#endif