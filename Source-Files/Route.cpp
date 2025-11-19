#include "Route.h"

Route::Route(string routeName){
    this->routeName = routeName;
    this->routeMatrix = nullptr;
    this->size = 0;
}

Route::Route(string routeName, vector<string> stops, int** routeMatrix, int size){
    this->routeName = routeName;
    this->size = size;

    for (auto& name : stops){
        this->stops.push_back(name);
    }

    this->routeMatrix = new int*[size];
    for(int i = 0; i < size; i++){
        this->routeMatrix[i] = new int[size];
    }

    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){ 
            this->routeMatrix[i][j] = routeMatrix[i][j];
        }
    }
}

// Constructor de copia
Route::Route(const Route& other){
    this->routeName = other.routeName;
    this->stops = other.stops;
    this->size = other.size;
    
    if(other.routeMatrix != nullptr){
        this->routeMatrix = new int*[size];
        for(int i = 0; i < size; i++){
            this->routeMatrix[i] = new int[size];
            for(int j = 0; j < size; j++){
                this->routeMatrix[i][j] = other.routeMatrix[i][j];
            }
        }
    } else {
        this->routeMatrix = nullptr;
    }
}

// Operador de asignación
Route& Route::operator=(const Route& other){
    if(this != &other){
        // Liberar memoria existente
        if(routeMatrix != nullptr){
            for(int i = 0; i < size; i++){
                delete[] routeMatrix[i];
            }
            delete[] routeMatrix;
        }
        
        // Copiar datos
        this->routeName = other.routeName;
        this->stops = other.stops;
        this->size = other.size;
        
        // Copiar matriz
        if(other.routeMatrix != nullptr){
            this->routeMatrix = new int*[size];
            for(int i = 0; i < size; i++){
                this->routeMatrix[i] = new int[size];
                for(int j = 0; j < size; j++){
                    this->routeMatrix[i][j] = other.routeMatrix[i][j];
                }
            }
        } else {
            this->routeMatrix = nullptr;
        }
    }
    return *this;
}

Route::~Route(){
    if(routeMatrix != nullptr){
        for(int i = 0; i < size; i++){
            delete[] routeMatrix[i];
        }
        delete[] routeMatrix;
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

vector<string> Route::getStops(){
    return stops;
}

int** Route::getMatrix(){
    return routeMatrix;
}

int Route::getSize(){
    return size;
}