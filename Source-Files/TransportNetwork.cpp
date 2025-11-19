#include "TransportNetwork.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <queue>
#include <stack>

using namespace std;

TransportNetwork::TransportNetwork(){
    size = 0;
    adjMatrix = nullptr;
}

TransportNetwork::~TransportNetwork(){
    if(adjMatrix != nullptr){
        for(int i = 0; i < size; i++){
            delete[] adjMatrix[i];
        }
        delete[] adjMatrix;
    }
}

void TransportNetwork::addRoute(Route route){
    routes.push_back(route);
    
    // Actualizar stops únicos
    vector<string> routeStops = route.getStops();
    for(auto& stop : routeStops){
        if(find(stops.begin(), stops.end(), stop) == stops.end()){
            stops.push_back(stop);
        }
    }
    
    // Reconstruir matriz de adyacencia
    rebuildAdjMatrix();
}

void TransportNetwork::rebuildAdjMatrix(){
    // Liberar matriz anterior
    if(adjMatrix != nullptr){
        for(int i = 0; i < size; i++){
            delete[] adjMatrix[i];
        }
        delete[] adjMatrix;
    }
    
    size = stops.size();
    
    // Inicializar nueva matriz con infinito
    adjMatrix = new int*[size];
    for(int i = 0; i < size; i++){
        adjMatrix[i] = new int[size];
        for(int j = 0; j < size; j++){
            adjMatrix[i][j] = (i == j) ? 0 : INF;
        }
    }
    
    // Llenar matriz con datos de rutas
    for(auto& route : routes){
        int** routeMatrix = route.getMatrix();
        vector<string> routeStops = route.getStops();
        int routeSize = route.getSize();
        
        for(int i = 0; i < routeSize; i++){
            for(int j = 0; j < routeSize; j++){
                if(routeMatrix[i][j] != 0 && routeMatrix[i][j] != INF){
                    int globalI = getStopIndex(routeStops[i]);
                    int globalJ = getStopIndex(routeStops[j]);
                    
                    if(globalI != -1 && globalJ != -1){
                        if(adjMatrix[globalI][globalJ] == INF || 
                           routeMatrix[i][j] < adjMatrix[globalI][globalJ]){
                            adjMatrix[globalI][globalJ] = routeMatrix[i][j];
                        }
                    }
                }
            }
        }
    }
}

int TransportNetwork::getStopIndex(string stopName){
    for(int i = 0; i < stops.size(); i++){
        if(stops[i] == stopName){
            return i;
        }
    }
    return -1;
}

vector<string> TransportNetwork::dijkstra(string origin, string destination){
    vector<string> path;
    int start = getStopIndex(origin);
    int end = getStopIndex(destination);
    
    if(start == -1 || end == -1){
        cout << "Parada origen o destino no encontrada.\n";
        return path;
    }
    
    vector<int> dist(size, INF);
    vector<int> prev(size, -1);
    vector<bool> visited(size, false);
    
    dist[start] = 0;
    
    for(int count = 0; count < size - 1; count++){
        int minDist = INF;
        int u = -1;
        
        for(int i = 0; i < size; i++){
            if(!visited[i] && dist[i] < minDist){
                minDist = dist[i];
                u = i;
            }
        }
        
        if(u == -1) break;
        
        visited[u] = true;
        
        for(int v = 0; v < size; v++){
            if(!visited[v] && adjMatrix[u][v] != INF && 
               dist[u] != INF && dist[u] + adjMatrix[u][v] < dist[v]){
                dist[v] = dist[u] + adjMatrix[u][v];
                prev[v] = u;
            }
        }
    }
    
    if(dist[end] == INF){
        cout << "No existe ruta entre " << origin << " y " << destination << ".\n";
        return path;
    }
    
    // Reconstruir camino
    int current = end;
    while(current != -1){
        path.insert(path.begin(), stops[current]);
        current = prev[current];
    }
    
    cout << "Distancia total: " << dist[end] << "\n";
    
    return path;
}

vector<string> TransportNetwork::BFS(string start){
    vector<string> result;
    int startIdx = getStopIndex(start);
    
    if(startIdx == -1){
        cout << "Parada de inicio no encontrada.\n";
        return result;
    }
    
    vector<bool> visited(size, false);
    queue<int> q;
    
    visited[startIdx] = true;
    q.push(startIdx);
    
    while(!q.empty()){
        int current = q.front();
        q.pop();
        result.push_back(stops[current]);
        
        for(int i = 0; i < size; i++){
            if(!visited[i] && adjMatrix[current][i] != INF && adjMatrix[current][i] != 0){
                visited[i] = true;
                q.push(i);
            }
        }
    }
    
    return result;
}

vector<string> TransportNetwork::DFS(string start){
    vector<string> result;
    int startIdx = getStopIndex(start);
    
    if(startIdx == -1){
        cout << "Parada de inicio no encontrada.\n";
        return result;
    }
    
    vector<bool> visited(size, false);
    stack<int> s;
    
    s.push(startIdx);
    
    while(!s.empty()){
        int current = s.top();
        s.pop();
        
        if(!visited[current]){
            visited[current] = true;
            result.push_back(stops[current]);
            
            for(int i = size - 1; i >= 0; i--){
                if(!visited[i] && adjMatrix[current][i] != INF && adjMatrix[current][i] != 0){
                    s.push(i);
                }
            }
        }
    }
    
    return result;
}

class DisjointSet {
private:
    vector<int> parent, rank;
    
public:
    DisjointSet(int n){
        parent.resize(n);
        rank.resize(n, 0);
        for(int i = 0; i < n; i++){
            parent[i] = i;
        }
    }
    
    int find(int x){
        if(parent[x] != x){
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    void unite(int x, int y){
        int rootX = find(x);
        int rootY = find(y);
        
        if(rootX != rootY){
            if(rank[rootX] < rank[rootY]){
                parent[rootX] = rootY;
            } else if(rank[rootX] > rank[rootY]){
                parent[rootY] = rootX;
            } else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
        }
    }
};

vector<Edge> TransportNetwork::kruskal(){
    vector<Edge> mst;
    vector<Edge> edges;
    
    // Recolectar todas las aristas
    for(int i = 0; i < size; i++){
        for(int j = i + 1; j < size; j++){
            if(adjMatrix[i][j] != INF && adjMatrix[i][j] != 0){
                edges.push_back(Edge(stops[i], stops[j], adjMatrix[i][j]));
            }
        }
    }
    
    // Ordenar aristas por peso
    sort(edges.begin(), edges.end(), [](Edge& a, Edge& b){
        return a.weight < b.weight;
    });
    
    DisjointSet ds(size);
    
    for(auto& edge : edges){
        int u = getStopIndex(edge.source);
        int v = getStopIndex(edge.destination);
        
        if(ds.find(u) != ds.find(v)){
            ds.unite(u, v);
            mst.push_back(edge);
        }
    }
    
    return mst;
}

vector<Edge> TransportNetwork::prim(){
    vector<Edge> mst;
    
    if(size == 0) return mst;
    
    vector<bool> inMST(size, false);
    vector<int> key(size, INF);
    vector<int> parent(size, -1);
    
    key[0] = 0;
    
    for(int count = 0; count < size - 1; count++){
        int minKey = INF;
        int u = -1;
        
        for(int i = 0; i < size; i++){
            if(!inMST[i] && key[i] < minKey){
                minKey = key[i];
                u = i;
            }
        }
        
        if(u == -1) break;
        
        inMST[u] = true;
        
        for(int v = 0; v < size; v++){
            if(adjMatrix[u][v] != INF && !inMST[v] && adjMatrix[u][v] < key[v]){
                parent[v] = u;
                key[v] = adjMatrix[u][v];
            }
        }
    }
    
    for(int i = 1; i < size; i++){
        if(parent[i] != -1){
            mst.push_back(Edge(stops[parent[i]], stops[i], adjMatrix[parent[i]][i]));
        }
    }
    
    return mst;
}

void TransportNetwork::printGraph(){
    cout << "\n=== Red de Transporte ===\n";
    cout << "Total de paradas: " << size << "\n\n";
    
    cout << "Matriz de Adyacencia:\n";
    cout << "     ";
    for(int i = 0; i < size; i++){
        cout << stops[i].substr(0, 4) << " ";
    }
    cout << "\n";
    
    for(int i = 0; i < size; i++){
        cout << stops[i].substr(0, 4) << " ";
        for(int j = 0; j < size; j++){
            if(adjMatrix[i][j] == INF){
                cout << " INF ";
            } else {
                cout << " " << adjMatrix[i][j] << "   ";
            }
        }
        cout << "\n";
    }
}

vector<Route> TransportNetwork::getRoutes(){
    return routes;
}

vector<string> TransportNetwork::getAllStops(){
    return stops;
}

int TransportNetwork::getSize(){
    return size;
}