#include <iostream>
#include "UserTree.h"
#include "TransportNetwork.h"
#include "FileManager.h"
#include "User.h"
#include "Node.h"

using namespace std;

void testUserTree(UserTree<User>& tree, Node<User>*& root);
void testTransportNetwork(TransportNetwork& network);

int main(){
    cout << "=== PRUEBA DE CARGA DE ARCHIVOS ===\n\n";
    
    // Crear estructuras
    UserTree<User> userTree;
    Node<User>* root = nullptr;
    TransportNetwork network;
    FileManager fileManager;
    
    // --- PRUEBA 1: Cargar Usuarios ---
    cout << "1. Cargando usuarios desde usuarios.txt...\n";
    fileManager.loadUsers("../Persistance-Files/users.txt", userTree, root);
    cout << "\n";
    
    // --- PRUEBA 2: Mostrar Usuarios ---
    cout << "2. Mostrando usuarios cargados (In-Order):\n";
    cout << "-------------------------------------------\n";
    if(root){
        userTree.inOrder(root);
    } else {
        cout << "No hay usuarios cargados.\n";
    }
    cout << "\n";
    
    // --- PRUEBA 3: Cargar Rutas ---
    cout << "3. Cargando rutas desde routes.txt...\n";
    fileManager.loadRoutes("../Persistance-Files/routes.txt", network);
    cout << "Carga de rutas completada.\n\n";
    
    // --- PRUEBA 4: Mostrar Red de Transporte ---
    cout << "4. Mostrando red de transporte:\n";
    cout << "-------------------------------------------\n";
    try {
        network.printGraph();
    } catch(const exception& e) {
        cout << "Error al mostrar red: " << e.what() << "\n";
    }
    cout << "\n";
    
    // --- PRUEBA 5: Probar Dijkstra ---
    cout << "5. Probando algoritmo de Dijkstra:\n";
    cout << "-------------------------------------------\n";
    try {
        vector<string> allStops = network.getAllStops();
        cout << "Total de paradas en la red: " << allStops.size() << "\n";
        
        if(allStops.size() >= 2){
            cout << "Calculando ruta más corta de " << allStops[0] << " a " << allStops[1] << ":\n";
            vector<string> path = network.dijkstra(allStops[0], allStops[1]);
            
            if(!path.empty()){
                for(size_t i = 0; i < path.size(); i++){
                    cout << path[i];
                    if(i < path.size() - 1) cout << " -> ";
                }
                cout << "\n";
            } else {
                cout << "No se encontró una ruta.\n";
            }
        } else {
            cout << "No hay suficientes paradas para calcular ruta.\n";
        }
    } catch(const exception& e) {
        cout << "Error en Dijkstra: " << e.what() << "\n";
    }
    cout << "\n";
    
    // --- PRUEBA 6: Probar BFS ---
    cout << "6. Recorrido BFS desde primera parada:\n";
    cout << "-------------------------------------------\n";
    try {
        vector<string> allStops = network.getAllStops();
        if(!allStops.empty()){
            vector<string> bfsResult = network.BFS(allStops[0]);
            cout << "BFS desde " << allStops[0] << ": ";
            for(size_t i = 0; i < bfsResult.size(); i++){
                cout << bfsResult[i];
                if(i < bfsResult.size() - 1) cout << " -> ";
            }
            cout << "\n";
        } else {
            cout << "No hay paradas disponibles.\n";
        }
    } catch(const exception& e) {
        cout << "Error en BFS: " << e.what() << "\n";
    }
    cout << "\n";
    
    // --- PRUEBA 7: Probar DFS ---
    cout << "7. Recorrido DFS desde primera parada:\n";
    cout << "-------------------------------------------\n";
    try {
        vector<string> allStops = network.getAllStops();
        if(!allStops.empty()){
            vector<string> dfsResult = network.DFS(allStops[0]);
            cout << "DFS desde " << allStops[0] << ": ";
            for(size_t i = 0; i < dfsResult.size(); i++){
                cout << dfsResult[i];
                if(i < dfsResult.size() - 1) cout << " -> ";
            }
            cout << "\n";
        } else {
            cout << "No hay paradas disponibles.\n";
        }
    } catch(const exception& e) {
        cout << "Error en DFS: " << e.what() << "\n";
    }
    cout << "\n";
    
    // --- PRUEBA 8: Probar Kruskal (MST) ---
    cout << "8. Árbol de Expansión Mínima (Kruskal):\n";
    cout << "-------------------------------------------\n";
    try {
        vector<Edge> mst = network.kruskal();
        
        if(mst.empty()){
            cout << "No se pudo generar el MST (red vacía o desconectada).\n";
        } else {
            int totalWeight = 0;
            for(const auto& edge : mst){
                cout << edge.source << " <-> " << edge.destination 
                     << " (Peso: " << edge.weight << ")\n";
                totalWeight += edge.weight;
            }
            cout << "Peso total del MST: " << totalWeight << "\n";
        }
    } catch(const exception& e) {
        cout << "Error en Kruskal: " << e.what() << "\n";
    }
    cout << "\n";
    
    // --- PRUEBA 9: Guardar datos ---
    cout << "9. Guardando datos en archivos de respaldo:\n";
    cout << "-------------------------------------------\n";
    try {
        fileManager.saveUsers("usuarios_backup.txt", root);
        fileManager.saveRoutes("rutas_backup.txt", network);
        cout << "Datos guardados exitosamente.\n";
    } catch(const exception& e) {
        cout << "Error al guardar: " << e.what() << "\n";
    }
    cout << "\n";
    
    // --- PRUEBA 10: Generar reporte ---
    cout << "10. Generando reporte completo:\n";
    cout << "-------------------------------------------\n";
    try {
        fileManager.generateReport("reports.txt", root, network, "all");
        cout << "Reporte generado exitosamente.\n";
    } catch(const exception& e) {
        cout << "Error al generar reporte: " << e.what() << "\n";
    }
    cout << "\n";
    
    cout << "=== PRUEBAS COMPLETADAS ===\n";
    cout << "Revisa los archivos generados:\n";
    cout << "  - usuarios_backup.txt\n";
    cout << "  - rutas_backup.txt\n";
    cout << "  - reports.txt\n";
    
    return 0;
}