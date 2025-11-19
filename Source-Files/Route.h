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
    
    // Constructor de copia
    Route(const Route& other);
    
    // Operador de asignación
    Route& operator=(const Route& other);
    
    // Destructor
    ~Route();
    
    void printStops();
    void printRoute();
    string getName();
    vector<string> getStops();
    int** getMatrix();
    int getSize();
};

#endif