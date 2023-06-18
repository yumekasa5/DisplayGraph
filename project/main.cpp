#include "MainWindow.h"
//#include "graph.h"

#include <QApplication>
#include <QStandardItemModel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

//    mModel = new QStandardItemModel(0, 3, this);
//    mModel
    w.show();
    return a.exec();
}
