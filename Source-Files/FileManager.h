#include <iostream> 
#include <fstream>
#include <iostream>
#include <limits>
#include <algorithm>
#include <queue>
#include <stack>
#include <string>
#include "UserTree.h"
#include "User.h"
#include "TransportNetwork.h"

using namespace std; 

class FileManager {
    public:
    void loadUsers(string filename, UserTree<User>& tree, Node<User>*& root);
    void saveUsers(string filename, Node<User>* root);
    void saveUsersRecursive(ofstream& file, Node<User>* root);
    void loadRoutes(string filename, TransportNetwork& network);
    void saveRoutes(string filename, TransportNetwork& network);
    void generateReport(string filename, Node<User>* userRoot, TransportNetwork& network, string reportType);
    void generateUserReport(ofstream& file, Node<User>* root);
    void generateNetworkReport(ofstream& file, TransportNetwork& network);
    void generateMSTReport(ofstream& file, TransportNetwork& network);
    string getCurrentDateTime();
};