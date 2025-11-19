#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <vector>
#include <string>
#include <cmath>
#include "TransportNetwork.h"

using namespace std;

class GraphWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit GraphWidget(QWidget *parent = nullptr);
    void setNetwork(TransportNetwork* net);
    void updateGraph();
    
protected:
    void paintEvent(QPaintEvent *event) override;
    
private:
    TransportNetwork* network;
    vector<QPoint> nodePositions;
    
    void calculateNodePositions();
    void drawNode(QPainter& painter, QPoint pos, int index, QString label);
    void drawEdge(QPainter& painter, QPoint from, QPoint to, int weight);
};

#endif 
