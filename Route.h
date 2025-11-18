#ifndef ROUTE_H
#define ROUTE_H
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class Route {
    private:
    string routeName; 
    vector<string> stops;
    int** routeMatrix;
    int size;

    public:
    Route(string routeName);
    Route(string routeName, vector<string> stops, int** routeMatrix, int size);
    ~Route();

    void printStops();
    void printRoute();

    string getName();
    int getSize();
};

#endif