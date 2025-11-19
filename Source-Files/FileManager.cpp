#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>

using namespace std;

void FileManager::loadUsers(string filename, UserTree<User>& tree, Node<User>*& root){
    ifstream file(filename);
    
    if(!file.is_open()){
        cout << "No se pudo abrir el archivo " << filename << ". Se creará uno nuevo.\n";
        return;
    }
    
    string line;
    while(getline(file, line)){
        if(line.empty()) continue;
        
        istringstream ss(line);
        long id;
        string name, ageStr, balanceStr;
        int age;
        long balance;
        
        // Leer ID
        getline(ss, line, ',');
        id = stol(line);
        
        // Leer Nombre (puede tener espacios)
        getline(ss, name, ',');
        // Eliminar espacios al inicio y final
        size_t start = name.find_first_not_of(" \t");
        size_t end = name.find_last_not_of(" \t");
        if(start != string::npos && end != string::npos){
            name = name.substr(start, end - start + 1);
        }
        
        // Leer Edad
        getline(ss, ageStr, ',');
        age = stoi(ageStr);
        
        // Leer Saldo
        getline(ss, balanceStr);
        balance = stol(balanceStr);
        
        User* newUser = new User(id, name, age, balance);
        tree.add(root, newUser);
    }
    
    file.close();
    cout << "Usuarios cargados exitosamente desde " << filename << "\n";
}

void FileManager::saveUsers(string filename, Node<User>* root){
    ofstream file(filename);
    
    if(!file.is_open()){
        cout << "Error al crear/abrir el archivo " << filename << "\n";
        return;
    }
    
    saveUsersRecursive(file, root);
    
    file.close();
    cout << "Usuarios guardados exitosamente en " << filename << "\n";
}

void FileManager::saveUsersRecursive(ofstream& file, Node<User>* root){
    if(!root) return;
    
    saveUsersRecursive(file, root->left);
    
    // Guardar en formato: ID,Nombre,Edad,Saldo
    file << root->data.getID() << ","
         << root->data.getName() << ","
         << root->data.getAge() << ","
         << root->data.getBalance() << "\n";
    
    saveUsersRecursive(file, root->right);
}

void FileManager::loadRoutes(string filename, TransportNetwork& network){
    ifstream file(filename);
    
    if(!file.is_open()){
        cout << "ERROR: No se pudo abrir el archivo " << filename << "\n";
        cout << "Verifica que el archivo existe en la ruta especificada.\n";
        return;
    }
    
    cout << "Archivo " << filename << " abierto correctamente.\n";
    
    string line;
    int routeCount = 0;
    
    try {
        while(getline(file, line)){
            // Saltar líneas vacías y comentarios
            if(line.empty() || line[0] == '#') continue;
            
            // Eliminar espacios en blanco al inicio y final
            size_t start = line.find_first_not_of(" \t\r\n");
            size_t end = line.find_last_not_of(" \t\r\n");
            if(start == string::npos) continue;
            line = line.substr(start, end - start + 1);
            
            // Formato: NombreRuta
            string routeName = line;
            cout << "Cargando ruta: " << routeName << "\n";
            
            // Leer número de paradas
            if(!getline(file, line)){
                cout << "ERROR: No se pudo leer el número de paradas\n";
                break;
            }
            
            int numStops = stoi(line);
            cout << "  Número de paradas: " << numStops << "\n";
            
            // Leer paradas
            vector<string> stops;
            for(int i = 0; i < numStops; i++){
                if(!getline(file, line)){
                    cout << "ERROR: No se pudieron leer todas las paradas\n";
                    file.close();
                    return;
                }
                
                // Limpiar la línea
                start = line.find_first_not_of(" \t\r\n");
                end = line.find_last_not_of(" \t\r\n");
                if(start != string::npos){
                    line = line.substr(start, end - start + 1);
                }
                
                stops.push_back(line);
                cout << "    Parada " << (i+1) << ": " << line << "\n";
            }
            
            // Leer matriz de adyacencia
            int** matrix = new int*[numStops];
            for(int i = 0; i < numStops; i++){
                matrix[i] = new int[numStops];
                
                if(!getline(file, line)){
                    cout << "ERROR: No se pudo leer la fila " << i << " de la matriz\n";
                    // Liberar memoria
                    for(int k = 0; k <= i; k++){
                        delete[] matrix[k];
                    }
                    delete[] matrix;
                    file.close();
                    return;
                }
                
                istringstream ss(line);
                string value;
                
                for(int j = 0; j < numStops; j++){
                    if(!getline(ss, value, ',')){
                        cout << "ERROR: Formato de matriz incorrecto en fila " << i << "\n";
                        // Liberar memoria
                        for(int k = 0; k <= i; k++){
                            delete[] matrix[k];
                        }
                        delete[] matrix;
                        file.close();
                        return;
                    }
                    
                    // Limpiar espacios
                    start = value.find_first_not_of(" \t\r\n");
                    end = value.find_last_not_of(" \t\r\n");
                    if(start != string::npos){
                        value = value.substr(start, end - start + 1);
                    }
                    
                    if(value == "INF"){
                        matrix[i][j] = 999999;
                    } else {
                        matrix[i][j] = stoi(value);
                    }
                }
            }
            
            // Crear y agregar la ruta
            Route route(routeName, stops, matrix, numStops);
            network.addRoute(route);
            routeCount++;
            
            cout << "  Ruta agregada exitosamente.\n\n";
            
            // Liberar matriz temporal
            for(int i = 0; i < numStops; i++){
                delete[] matrix[i];
            }
            delete[] matrix;
        }
        
        file.close();
        cout << "Total de rutas cargadas: " << routeCount << "\n";
        
    } catch(const exception& e){
        cout << "ERROR al parsear archivo: " << e.what() << "\n";
        file.close();
        return;
    }
}

void FileManager::saveRoutes(string filename, TransportNetwork& network){
    ofstream file(filename);
    
    if(!file.is_open()){
        cout << "Error al crear/abrir el archivo " << filename << "\n";
        return;
    }
    
    file << "# Archivo de Rutas de Transporte\n";
    file << "# Formato: NombreRuta, NumParadas, Lista de Paradas, Matriz de Adyacencia\n\n";
    
    vector<Route> routes = network.getRoutes();
    
    for(auto& route : routes){
        file << route.getName() << "\n";
        
        vector<string> stops = route.getStops();
        int size = route.getSize();
        
        file << size << "\n";
        
        for(const auto& stop : stops){
            file << stop << "\n";
        }
        
        int** matrix = route.getMatrix();
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                if(matrix[i][j] == 999999){
                    file << "INF";
                } else {
                    file << matrix[i][j];
                }
                
                if(j < size - 1) file << ",";
            }
            file << "\n";
        }
        
        file << "\n";
    }
    
    file.close();
    cout << "Rutas guardadas exitosamente en " << filename << "\n";
}

void FileManager::generateReport(string filename, 
                                 Node<User>* userRoot, 
                                 TransportNetwork& network,
                                 string reportType){
    ofstream file(filename, ios::app);
    
    if(!file.is_open()){
        cout << "Error al crear/abrir el archivo " << filename << "\n";
        return;
    }
    
    file << "\n========================================\n";
    file << "REPORTE GENERADO: " << getCurrentDateTime() << "\n";
    file << "========================================\n\n";
    
    if(reportType == "users" || reportType == "all"){
        file << "--- LISTADO DE USUARIOS ---\n";
        generateUserReport(file, userRoot);
        file << "\n";
    }
    
    if(reportType == "network" || reportType == "all"){
        file << "--- RED DE TRANSPORTE ---\n";
        generateNetworkReport(file, network);
        file << "\n";
    }
    
    if(reportType == "mst" || reportType == "all"){
        file << "--- ÁRBOL DE EXPANSIÓN MÍNIMA (KRUSKAL) ---\n";
        generateMSTReport(file, network);
        file << "\n";
    }
    
    file.close();
    cout << "Reporte generado exitosamente en " << filename << "\n";
}

void FileManager::generateUserReport(ofstream& file, Node<User>* root){
    if(!root) return;
    
    generateUserReport(file, root->left);
    
    file << "ID: " << root->data.getID() 
         << " | Nombre: " << root->data.getName()
         << " | Edad: " << root->data.getAge()
         << " | Saldo: " << root->data.getBalance() << "\n";
    
    generateUserReport(file, root->right);
}

void FileManager::generateNetworkReport(ofstream& file, TransportNetwork& network){
    vector<string> stops = network.getAllStops();
    
    file << "Total de paradas en la red: " << stops.size() << "\n";
    file << "Paradas:\n";
    
    for(size_t i = 0; i < stops.size(); i++){
        file << "  " << (i + 1) << ". " << stops[i] << "\n";
    }
}

void FileManager::generateMSTReport(ofstream& file, TransportNetwork& network){
    vector<Edge> mst = network.kruskal();    
    int totalWeight = 0;
    
    file << "Conexiones del árbol de expansión mínima:\n";
    for(const auto& edge : mst){
        file << "  " << edge.source << " <-> " << edge.destination
        << " (Distancia: " << edge.weight << ")\n";
        totalWeight += edge.weight;
    }
    
    file << "\nPeso total del MST: " << totalWeight << "\n";
}

string FileManager::getCurrentDateTime(){
    time_t now = time(0);
    char buffer[80];
    struct tm* timeinfo = localtime(&now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return string(buffer);
}