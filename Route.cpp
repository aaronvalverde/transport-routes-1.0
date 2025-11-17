#include "Route.h"

Route::Route(string routeName){
    this->routeName = routeName;
}

Route::Route(string routeName, vector<string> stops, int** routeMatrix, int size){
    this->routeName = routeName;
    this->size = size;

    for (auto& name : stops){
        this->stops.push_back(name);
    }

    for (int i = 0; i < size; i++){
        for (int j = 0; i < size; i++){
            this->routeMatrix[i][j] = routeMatrix[i][j];
        }
    }
}

void Route::printStops() {
    for (int i = 0; i < stops.size(); i++){
        cout << stops[i] << "\n";
    }
}

void Route::printRoute(){
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            cout << routeMatrix[i][j] << "\n";
        }
    }
}

string Route::getName(){
    return routeName;
}

int Route::getSize(){
    return size;
}



