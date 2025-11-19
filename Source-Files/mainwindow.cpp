#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GraphWidget.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QSplitter>
#include <QVBoxLayout>
#include <QGroupBox>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    root = nullptr;
    
    graphWidget = new GraphWidget(this);
    graphWidget->setMinimumSize(400, 400);
    
    QWidget* redTab = nullptr;
    for(int i = 0; i < ui->tabWidget->count(); i++){
        if(ui->tabWidget->tabText(i).contains("Red") || 
           ui->tabWidget->tabText(i).contains("Transporte")){
            redTab = ui->tabWidget->widget(i);
            break;
        }
    }
    
    if(redTab){
        QLayout* oldLayout = redTab->layout();
        if(oldLayout){
            QLayoutItem* item;
            while((item = oldLayout->takeAt(0)) != nullptr){
                if(item->widget()){
                    item->widget()->setParent(nullptr);
                }
                delete item;
            }
            delete oldLayout;
        }
        
        QVBoxLayout* mainLayout = new QVBoxLayout(redTab);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);
        
        QSplitter* splitter = new QSplitter(Qt::Horizontal, redTab);
        
        QWidget* leftPanel = new QWidget(splitter);
        QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
        leftLayout->setContentsMargins(5, 5, 5, 5);
        
        QGroupBox* graphGroup = new QGroupBox("Visualización de la Red", leftPanel);
        QVBoxLayout* graphLayout = new QVBoxLayout(graphGroup);
        graphLayout->setContentsMargins(5, 5, 5, 5);
        graphLayout->addWidget(graphWidget);
        
        leftLayout->addWidget(graphGroup);
        
        QWidget* rightPanel = new QWidget(splitter);
        QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
        rightLayout->setContentsMargins(5, 5, 5, 5);
        rightLayout->setSpacing(10);
        
        QGroupBox* selectionGroup = new QGroupBox("Selección de Paradas", rightPanel);
        QVBoxLayout* selectionLayout = new QVBoxLayout(selectionGroup);
        selectionLayout->setSpacing(8);
        
        QLabel* lblOrigen = new QLabel("Origen:", selectionGroup);
        selectionLayout->addWidget(lblOrigen);
        selectionLayout->addWidget(ui->cmbOrigen);
        
        selectionLayout->addSpacing(5);
        
        QLabel* lblDestino = new QLabel("Destino:", selectionGroup);
        selectionLayout->addWidget(lblDestino);
        selectionLayout->addWidget(ui->cmbDestino);
        
        rightLayout->addWidget(selectionGroup);
        
        QGroupBox* operationsGroup = new QGroupBox("Operaciones sobre el Grafo", rightPanel);
        QVBoxLayout* operationsLayout = new QVBoxLayout(operationsGroup);
        operationsLayout->setSpacing(8);
        
        operationsLayout->addWidget(ui->btnCalcularRutaMasCorta);
        operationsLayout->addWidget(ui->btnRecorridoAncho);
        operationsLayout->addWidget(ui->btnRecorridoProfundidad);
        operationsLayout->addWidget(ui->btnArbolExpansionMinima);
        
        rightLayout->addWidget(operationsGroup);
        
        QGroupBox* resultsGroup = new QGroupBox("Resultados", rightPanel);
        QVBoxLayout* resultsLayout = new QVBoxLayout(resultsGroup);
        resultsLayout->setContentsMargins(5, 5, 5, 5);
        resultsLayout->addWidget(ui->txtResultadosRutas);
        
        rightLayout->addWidget(resultsGroup);
        
        rightLayout->setStretch(0, 0); 
        rightLayout->setStretch(1, 0); 
        rightLayout->setStretch(2, 1); 
        
        splitter->addWidget(leftPanel);
        splitter->addWidget(rightPanel);
        
        splitter->setStretchFactor(0, 60);
        splitter->setStretchFactor(1, 40);
        
        mainLayout->addWidget(splitter);
    }
    
    loadInitialData();
    
    connect(ui->btnRegistrar, &QPushButton::clicked, this, &MainWindow::onRegistrarUsuario);
    connect(ui->btnBuscar, &QPushButton::clicked, this, &MainWindow::onBuscarUsuario);
    connect(ui->btnEliminar, &QPushButton::clicked, this, &MainWindow::onEliminarUsuario);
    connect(ui->btnPreOrden, &QPushButton::clicked, this, &MainWindow::onPreOrden);
    connect(ui->btnEnOrden, &QPushButton::clicked, this, &MainWindow::onEnOrden);
    connect(ui->btnPostOrden, &QPushButton::clicked, this, &MainWindow::onPostOrden);
    connect(ui->btnOrdenarAlfabeticamente, &QPushButton::clicked, this, &MainWindow::onOrdenarAlfabeticamente);
    
    connect(ui->btnCalcularRutaMasCorta, &QPushButton::clicked, this, &MainWindow::onCalcularRutaMasCorta);
    connect(ui->btnRecorridoAncho, &QPushButton::clicked, this, &MainWindow::onRecorridoBFS);
    connect(ui->btnRecorridoProfundidad, &QPushButton::clicked, this, &MainWindow::onRecorridoDFS);
    connect(ui->btnArbolExpansionMinima, &QPushButton::clicked, this, &MainWindow::onArbolExpansionMinima);
    
    connect(ui->btnReporteUsuarios, &QPushButton::clicked, this, &MainWindow::onReporteUsuarios);
    connect(ui->btnReporteRutas, &QPushButton::clicked, this, &MainWindow::onReporteRutas);
    connect(ui->btnReporteCompleto, &QPushButton::clicked, this, &MainWindow::onReporteCompleto);
    connect(ui->btnExportarReporte, &QPushButton::clicked, this, &MainWindow::onExportarReporte);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadInitialData()
{
    ui->txtResultadosUsuarios->append("=== CARGANDO DATOS ===\n");
    
    fileManager.loadUsers("../Persistance-Files/users.txt", userTree, root);
    
    if(root){
        int userCount = 0;
        countUsers(root, userCount);
        ui->txtResultadosUsuarios->append(QString("✓ %1 usuarios cargados\n").arg(userCount));
    } else {
        ui->txtResultadosUsuarios->append("✗ No se cargaron usuarios\n");
    }
    
    fileManager.loadRoutes("../Persistance-Files/routes.txt", network);
    
    int paradasCount = network.getAllStops().size();
    if(paradasCount > 0){
        ui->txtResultadosUsuarios->append(QString("✓ Red cargada: %1 paradas\n").arg(paradasCount));
    } else {
        ui->txtResultadosUsuarios->append("✗ No se cargaron rutas\n");
    }
    
    updateStopsComboBox();
    
    updateMatrizAdyacencia();
    
    ui->statusbar->showMessage("Carga de datos completada", 3000);
}

void MainWindow::updateStopsComboBox()
{
    ui->cmbOrigen->clear();
    ui->cmbDestino->clear();
    
    vector<string> stops = network.getAllStops();
    
    for(const auto& stop : stops){
        ui->cmbOrigen->addItem(QString::fromStdString(stop));
        ui->cmbDestino->addItem(QString::fromStdString(stop));
    }
}

void MainWindow::updateMatrizAdyacencia()
{
    if(network.getSize() == 0){
        graphWidget->setNetwork(nullptr);
        return;
    }
    
    graphWidget->setNetwork(&network);
}

void MainWindow::onRegistrarUsuario()
{
    long id = ui->txtIdentificacion->text().toLong();
    QString nombre = ui->txtNombre->text();
    int edad = ui->spinEdad->value();
    long saldo = ui->spinSaldo->value();
    
    if(id == 0 || nombre.isEmpty()){
        QMessageBox::warning(this, "Error", "Por favor complete todos los campos.");
        return;
    }
    
    User* newUser = new User(id, nombre.toStdString(), edad, saldo);
    
    if(userTree.find(root, newUser)){
        QMessageBox::warning(this, "Error", "Ya existe un usuario con esa identificación.");
        delete newUser;
        return;
    }
    
    userTree.add(root, newUser);
    
    ui->txtResultadosUsuarios->append("✓ Usuario registrado exitosamente:");
    ui->txtResultadosUsuarios->append(QString("  ID: %1 | Nombre: %2 | Edad: %3 | Saldo: ₡%4\n")
                                      .arg(id).arg(nombre).arg(edad).arg(saldo));
    
    ui->txtIdentificacion->clear();
    ui->txtNombre->clear();
    ui->spinEdad->setValue(1);
    ui->spinSaldo->setValue(0);
    
    fileManager.saveUsers("../Persistance-Files/users.txt", root);
}

void MainWindow::onBuscarUsuario()
{
    long id = ui->txtIdentificacion->text().toLong();
    
    if(id == 0){
        QMessageBox::warning(this, "Error", "Ingrese una identificación válida.");
        return;
    }
    
    User tempUser(id, "", 0, 0);
    Node<User>* found = userTree.find(root, &tempUser);
    
    if(found){
        ui->txtResultadosUsuarios->append("✓ Usuario encontrado:");
        ui->txtResultadosUsuarios->append(QString("  ID: %1").arg(found->data.getID()));
        ui->txtResultadosUsuarios->append(QString("  Nombre: %1").arg(QString::fromStdString(found->data.getName())));
        ui->txtResultadosUsuarios->append(QString("  Edad: %1").arg(found->data.getAge()));
        ui->txtResultadosUsuarios->append(QString("  Saldo: ₡%1\n").arg(found->data.getBalance()));
        
        ui->txtNombre->setText(QString::fromStdString(found->data.getName()));
        ui->spinEdad->setValue(found->data.getAge());
        ui->spinSaldo->setValue(found->data.getBalance());
    } else {
        QMessageBox::information(this, "No encontrado", "No existe un usuario con esa identificación.");
    }
}

void MainWindow::onEliminarUsuario()
{
    long id = ui->txtIdentificacion->text().toLong();
    
    if(id == 0){
        QMessageBox::warning(this, "Error", "Ingrese una identificación válida.");
        return;
    }
    
    User tempUser(id, "", 0, 0);
    Node<User>* found = userTree.find(root, &tempUser);
    
    if(!found){
        QMessageBox::warning(this, "Error", "No existe un usuario con esa identificación.");
        return;
    }
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmar", 
                                   QString("¿Desea eliminar el usuario %1?")
                                   .arg(QString::fromStdString(found->data.getName())),
                                   QMessageBox::Yes|QMessageBox::No);
    
    if(reply == QMessageBox::Yes){
        userTree.remove(root, &tempUser);
        ui->txtResultadosUsuarios->append(QString("✓ Usuario eliminado: ID %1\n").arg(id));
        
        ui->txtIdentificacion->clear();
        ui->txtNombre->clear();
        ui->spinEdad->setValue(1);
        ui->spinSaldo->setValue(0);
        
        fileManager.saveUsers("../Persistance-Files/users.txt", root);
    }
}

void MainWindow::onPreOrden()
{
    ui->txtResultadosUsuarios->clear();
    ui->txtResultadosUsuarios->append("Recorrido Pre-Orden\n\n");
    
    if(!root){
        ui->txtResultadosUsuarios->append("No hay usuarios registrados.");
        return;
    }
    
    printPreOrder(root);
}

void MainWindow::onEnOrden()
{
    ui->txtResultadosUsuarios->clear();
    ui->txtResultadosUsuarios->append("Recorrido En-Orden\n\n");
    
    if(!root){
        ui->txtResultadosUsuarios->append("No hay usuarios registrados.");
        return;
    }
    
    printInOrder(root);
}

void MainWindow::onPostOrden()
{
    ui->txtResultadosUsuarios->clear();
    ui->txtResultadosUsuarios->append("Recorrido Post-Orden\n\n");
    
    if(!root){
        ui->txtResultadosUsuarios->append("No hay usuarios registrados.");
        return;
    }
    
    printPostOrder(root);
}

void MainWindow::onOrdenarAlfabeticamente()
{
    ui->txtResultadosUsuarios->clear();
    ui->txtResultadosUsuarios->append("Usuarios Ordenados Alfabeticamente\n\n");
    
    if(!root){
        ui->txtResultadosUsuarios->append("No hay usuarios registrados.");
        return;
    }
    
    vector<User> users;
    collectUsers(root, users);
    
    sort(users.begin(), users.end(), [](const User& a, const User& b){
        return a.getName() < b.getName();
    });
    
    for(const auto& user : users){
        ui->txtResultadosUsuarios->append(QString("ID: %1 | Nombre: %2 | Edad: %3 | Saldo: ₡%4")
                                          .arg(user.getID())
                                          .arg(QString::fromStdString(user.getName()))
                                          .arg(user.getAge())
                                          .arg(user.getBalance()));
    }
}

void MainWindow::onCalcularRutaMasCorta()
{
    QString origen = ui->cmbOrigen->currentText();
    QString destino = ui->cmbDestino->currentText();
    
    if(origen.isEmpty() || destino.isEmpty()){
        QMessageBox::warning(this, "Error", "Seleccione origen y destino.");
        return;
    }
    
    ui->txtResultadosRutas->clear();
    ui->txtResultadosRutas->append("Ruta más corta (Dijkstra)\n\n");
    ui->txtResultadosRutas->append(QString("Origen: %1").arg(origen));
    ui->txtResultadosRutas->append(QString("Destino: %1\n").arg(destino));
    
    vector<string> path = network.dijkstra(origen.toStdString(), destino.toStdString());
    
    if(path.empty()){
        ui->txtResultadosRutas->append("No se encontró una ruta.");
    } else {
        QString route = "Ruta: ";
        for(size_t i = 0; i < path.size(); i++){
            route += QString::fromStdString(path[i]);
            if(i < path.size() - 1) route += " → ";
        }
        ui->txtResultadosRutas->append(route);
    }
}

void MainWindow::onRecorridoBFS()
{
    QString inicio = ui->cmbOrigen->currentText();
    
    if(inicio.isEmpty()){
        QMessageBox::warning(this, "Error", "Seleccione una estación de inicio.");
        return;
    }
    
    ui->txtResultadosRutas->clear();
    ui->txtResultadosRutas->append("Recorrido en Ancho (BFS)\n\n");
    ui->txtResultadosRutas->append(QString("Inicio: %1\n").arg(inicio));
    
    vector<string> result = network.BFS(inicio.toStdString());
    
    QString output = "Recorrido: ";
    for(size_t i = 0; i < result.size(); i++){
        output += QString::fromStdString(result[i]);
        if(i < result.size() - 1) output += " → ";
    }
    
    ui->txtResultadosRutas->append(output);
}

void MainWindow::onRecorridoDFS()
{
    QString inicio = ui->cmbOrigen->currentText();
    
    if(inicio.isEmpty()){
        QMessageBox::warning(this, "Error", "Seleccione una estación de inicio.");
        return;
    }
    
    ui->txtResultadosRutas->clear();
    ui->txtResultadosRutas->append("Recorrido en Profundidad (DFS)\n\n");
    ui->txtResultadosRutas->append(QString("Inicio: %1\n").arg(inicio));
    
    vector<string> result = network.DFS(inicio.toStdString());
    
    QString output = "Recorrido: ";
    for(size_t i = 0; i < result.size(); i++){
        output += QString::fromStdString(result[i]);
        if(i < result.size() - 1) output += " → ";
    }
    
    ui->txtResultadosRutas->append(output);
}

void MainWindow::onArbolExpansionMinima()
{
    ui->txtResultadosRutas->clear();
    ui->txtResultadosRutas->append("Árbol de Expansión Mínima (Kruskal)\n\n");
    
    vector<Edge> mst = network.kruskal();
    
    if(mst.empty()){
        ui->txtResultadosRutas->append("No se pudo generar el MST.");
        return;
    }
    
    int totalWeight = 0;
    
    for(const auto& edge : mst){
        ui->txtResultadosRutas->append(QString("%1 ↔ %2 (Peso: %3)")
                                       .arg(QString::fromStdString(edge.source))
                                       .arg(QString::fromStdString(edge.destination))
                                       .arg(edge.weight));
        totalWeight += edge.weight;
    }
    
    ui->txtResultadosRutas->append(QString("\nPeso total del MST: %1").arg(totalWeight));
}

void MainWindow::onReporteUsuarios()
{
    ui->txtReportes->clear();
    ui->txtReportes->append("Reporte de Usuarios:");
    
    if(!root){
        ui->txtReportes->append("No hay usuarios registrados.");
        return;
    }
    
    printInOrderReport(root);
}

void MainWindow::onReporteRutas()
{
    ui->txtReportes->clear();
    ui->txtReportes->append("Reporte de Red de Transporte\n\n");
    
    vector<string> stops = network.getAllStops();
    
    ui->txtReportes->append(QString("-Total de paradas: %1\n").arg(stops.size()));
    ui->txtReportes->append("\n-Paradas:");
    
    for(size_t i = 0; i < stops.size(); i++){
        ui->txtReportes->append(QString("  %1. %2").arg(i+1).arg(QString::fromStdString(stops[i])));
    }
}

void MainWindow::onReporteCompleto()
{
    ui->txtReportes->clear();
    ui->txtReportes->append("Reporte Completo del Sistema\n\n");
    
    ui->txtReportes->append("-Usuarios Reistrados: \n");
    if(root){
        printInOrderReport(root);
    } else {
        ui->txtReportes->append("No hay usuarios registrados.\n");
    }
    
    ui->txtReportes->append("\nRed de Transporte:\n");
    vector<string> stops = network.getAllStops();
    ui->txtReportes->append(QString("-Total de paradas: %1\n").arg(stops.size()));
    
    ui->txtReportes->append("\n-Árbol de Expansión Mínima:\n");
    vector<Edge> mst = network.kruskal();
    int totalWeight = 0;
    
    for(const auto& edge : mst){
        ui->txtReportes->append(QString("%1 ↔ %2 (Peso: %3)")
                                .arg(QString::fromStdString(edge.source))
                                .arg(QString::fromStdString(edge.destination))
                                .arg(edge.weight));
        totalWeight += edge.weight;
    }
    
    ui->txtReportes->append(QString("\nPeso total del MST: %1").arg(totalWeight));
}

void MainWindow::onExportarReporte()
{
    fileManager.generateReport("reports.txt", root, network, "all");
    QMessageBox::information(this, "Éxito", "Reporte exportado a reports.txt");
}

void MainWindow::printPreOrder(Node<User>* node)
{
    if(!node) return;
    
    ui->txtResultadosUsuarios->append(QString("ID: %1 | Nombre: %2 | Edad: %3 | Saldo: ₡%4")
                                      .arg(node->data.getID())
                                      .arg(QString::fromStdString(node->data.getName()))
                                      .arg(node->data.getAge())
                                      .arg(node->data.getBalance()));
    
    printPreOrder(node->left);
    printPreOrder(node->right);
}

void MainWindow::printInOrder(Node<User>* node)
{
    if(!node) return;
    
    printInOrder(node->left);
    
    ui->txtResultadosUsuarios->append(QString("ID: %1 | Nombre: %2 | Edad: %3 | Saldo: ₡%4")
                                      .arg(node->data.getID())
                                      .arg(QString::fromStdString(node->data.getName()))
                                      .arg(node->data.getAge())
                                      .arg(node->data.getBalance()));
    
    printInOrder(node->right);
}

void MainWindow::printPostOrder(Node<User>* node)
{
    if(!node) return;
    
    printPostOrder(node->left);
    printPostOrder(node->right);
    
    ui->txtResultadosUsuarios->append(QString("ID: %1 | Nombre: %2 | Edad: %3 | Saldo: ₡%4")
                                      .arg(node->data.getID())
                                      .arg(QString::fromStdString(node->data.getName()))
                                      .arg(node->data.getAge())
                                      .arg(node->data.getBalance()));
}

void MainWindow::printInOrderReport(Node<User>* node)
{
    if(!node) return;
    
    printInOrderReport(node->left);
    
    ui->txtReportes->append(QString("ID: %1 | Nombre: %2 | Edad: %3 | Saldo: ₡%4")
                            .arg(node->data.getID())
                            .arg(QString::fromStdString(node->data.getName()))
                            .arg(node->data.getAge())
                            .arg(node->data.getBalance()));
    
    printInOrderReport(node->right);
}

void MainWindow::collectUsers(Node<User>* node, vector<User>& users)
{
    if(!node) return;
    
    users.push_back(node->data);
    collectUsers(node->left, users);
    collectUsers(node->right, users);
}

void MainWindow::countUsers(Node<User>* node, int& count)
{
    if(!node) return;
    count++;
    countUsers(node->left, count);
    countUsers(node->right, count);
}