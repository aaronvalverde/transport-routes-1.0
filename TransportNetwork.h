#ifdef TRANSPORTNETWORK_H
#define TRANSPORTNETWORK_H

#include <iostream>
#include <fstream>
#include "Route.h"

using namespace std;

class TransportNetwork {
    private: 
    list<Route> routes;

    public: 
    void addRoute(Route* route);
    void loadRoutes(string filePath);
    void saveRoutes(string filePath);
    Route findRoute(string routeName);

    
};

#endif