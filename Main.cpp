#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QPushButton btn("Transport Routes 1.0");
    btn.show();

    return app.exec();
}
