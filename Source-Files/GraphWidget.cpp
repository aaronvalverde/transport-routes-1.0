#include "GraphWidget.h"
#include <QPainter>
#include <QFont>
#include <cmath>
#include <random>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GraphWidget::GraphWidget(QWidget *parent)
    : QWidget(parent), network(nullptr)
{
    setMinimumSize(800, 600);
}

void GraphWidget::setNetwork(TransportNetwork* net)
{
    network = net;
    updateGraph();
}

void GraphWidget::updateGraph()
{
    if(network){
        calculateNodePositions();
        update();
    }
}

void GraphWidget::calculateNodePositions()
{
    nodePositions.clear();
    
    if(!network) return;
    
    int numNodes = network->getSize();
    if(numNodes == 0) return;
   
    int marginLeft = 100;  
    int marginRight = 180;
    int marginTop = 80;     
    int marginBottom = 60; 
    
    int availableWidth = width() - marginLeft - marginRight;
    int availableHeight = height() - marginTop - marginBottom;
    
    int cols = static_cast<int>(std::ceil(std::sqrt(numNodes * 1.6)));
    int rows = static_cast<int>(std::ceil(static_cast<double>(numNodes) / cols));
    
    double spacingX = availableWidth / static_cast<double>(cols);
    double spacingY = availableHeight / static_cast<double>(rows);
    
    spacingX = std::max(spacingX, 180.0);
    spacingY = std::max(spacingY, 180.0);
    
    std::mt19937 gen(42);
    std::uniform_real_distribution<> offsetX(-spacingX * 0.15, spacingX * 0.15);
    std::uniform_real_distribution<> offsetY(-spacingY * 0.15, spacingY * 0.15);
    
    for(int i = 0; i < numNodes; i++){
        int row = i / cols;
        int col = i % cols;
        
        double baseX = marginLeft + (col + 0.5) * spacingX;
        double baseY = marginTop + (row + 0.5) * spacingY;
        
        double x = baseX + offsetX(gen);
        double y = baseY + offsetY(gen);
        
        nodePositions.push_back(QPoint(static_cast<int>(x), static_cast<int>(y)));
    }
}

void GraphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    if(!network || network->getSize() == 0){
        painter.setPen(QColor(226, 232, 240));
        painter.setFont(QFont("Segoe UI", 12));
        painter.drawText(rect(), Qt::AlignCenter, "No hay red de transporte cargada");
        return;
    }
    
    vector<string> stops = network->getAllStops();
    int size = network->getSize();
    
    int** adjMatrix = network->getMatrix();
    
    for(int i = 0; i < size; i++){
        for(int j = i + 1; j < size; j++){
            int weight = adjMatrix[i][j];
            
            if(weight != 999999 && weight != 0){
                drawEdge(painter, nodePositions[i], nodePositions[j], weight);
            }
        }
    }
    
    for(int i = 0; i < size; i++){
        QString label = QString::fromStdString(stops[i]);
        drawNode(painter, nodePositions[i], i + 1, label);
    }
    
    painter.setPen(QColor(234, 179, 8)); 
    painter.setFont(QFont("Segoe UI", 14, QFont::Bold));
    painter.drawText(10, 25, "Visualización de la Red de Transporte");
}

void GraphWidget::drawNode(QPainter& painter, QPoint pos, int index, QString label)
{
    int nodeRadius = 25;
    
    QRadialGradient gradient(pos, nodeRadius);
    gradient.setColorAt(0, QColor(100, 150, 255));
    gradient.setColorAt(1, QColor(50, 100, 200));
    
    painter.setBrush(gradient);
    painter.setPen(QPen(QColor(30, 60, 150), 2));
    painter.drawEllipse(pos, nodeRadius, nodeRadius);
    
    painter.setPen(Qt::white);
    painter.setFont(QFont("Segoe UI", 11, QFont::Bold));
    painter.drawText(QRect(pos.x() - nodeRadius, pos.y() - nodeRadius, 
                           nodeRadius * 2, nodeRadius * 2), 
                    Qt::AlignCenter, QString::number(index));
    
    painter.setFont(QFont("Segoe UI", 9));
    
    int labelY = pos.y() + nodeRadius + 15;
    
    QFontMetrics fm(painter.font());
    QRect textRect = fm.boundingRect(label);
    textRect.moveCenter(QPoint(pos.x(), labelY));
    textRect.adjust(-4, -2, 4, 2);
    
    painter.setBrush(QColor(15, 23, 42, 230));
    painter.setPen(QPen(QColor(71, 85, 105), 1));
    painter.drawRoundedRect(textRect, 3, 3);
    
    painter.setPen(QColor(226, 232, 240)); 
    painter.drawText(textRect, Qt::AlignCenter, label);
}

void GraphWidget::drawEdge(QPainter& painter, QPoint from, QPoint to, int weight)
{
    int nodeRadius = 25;
    
    double dx = to.x() - from.x();
    double dy = to.y() - from.y();
    double length = sqrt(dx * dx + dy * dy);
    
    if(length == 0) return;
    
    dx /= length;
    dy /= length;
    
    QPoint start(from.x() + dx * nodeRadius, from.y() + dy * nodeRadius);
    QPoint end(to.x() - dx * nodeRadius, to.y() - dy * nodeRadius);
    
    painter.setPen(QPen(QColor(71, 85, 105), 2));
    painter.drawLine(start, end);
    
    if(weight > 0 && weight < 999999){
        QPoint mid((start.x() + end.x()) / 2, (start.y() + end.y()) / 2);
        
        QString weightText = QString::number(weight);
        painter.setFont(QFont("Segoe UI", 9, QFont::Bold));
        QFontMetrics fm(painter.font());
        QRect textRect = fm.boundingRect(weightText);
        textRect.moveCenter(mid);
        textRect.adjust(-3, -2, 3, 2);
        
        painter.setBrush(QColor(15, 23, 42));
        painter.setPen(QPen(QColor(234, 179, 8), 1));
        painter.drawRoundedRect(textRect, 2, 2);
        
        painter.setPen(QColor(234, 179, 8)); 
        painter.drawText(textRect, Qt::AlignCenter, weightText);
    }
}