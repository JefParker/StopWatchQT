#include "mainwindow.h"
#include "qwindow.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":images/Stopwatch128.png"));

    MainWindow w;
    w.setWindowTitle("StopWatch");
    w.show();
    return a.exec();
}
