#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "UserTree.h"
#include "TransportNetwork.h"
#include "FileManager.h"
#include "User.h"
#include "Node.h"
#include <vector>
#include "GraphWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onRegistrarUsuario();
    void onBuscarUsuario();
    void onEliminarUsuario();
    void onPreOrden();
    void onEnOrden();
    void onPostOrden();
    void onOrdenarAlfabeticamente();
    
    void onCalcularRutaMasCorta();
    void onRecorridoBFS();
    void onRecorridoDFS();
    void onArbolExpansionMinima();
    
    void onReporteUsuarios();
    void onReporteRutas();
    void onReporteCompleto();
    void onExportarReporte();

private:
    Ui::MainWindow *ui;
    GraphWidget* graphWidget;
    
    UserTree<User> userTree;
    Node<User>* root;
    TransportNetwork network;
    FileManager fileManager;
    
    void loadInitialData();
    void updateStopsComboBox();
    void updateMatrizAdyacencia();
    
    void printPreOrder(Node<User>* node);
    void printInOrder(Node<User>* node);
    void printPostOrder(Node<User>* node);
    void printInOrderReport(Node<User>* node);
    void collectUsers(Node<User>* node, std::vector<User>& users);
    void countUsers(Node<User>* node, int& count);
};

#endif