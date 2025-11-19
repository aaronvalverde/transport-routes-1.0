#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile styleFile("styles.qss"); 
    if (styleFile.open(QFile::ReadOnly)) {
        QString style = QLatin1String(styleFile.readAll());
        app.setStyleSheet(style);
        styleFile.close();
    } else {
        qWarning("No se pudo cargar styles.qss");
    }

    MainWindow window;
    window.show();
    return app.exec();
}