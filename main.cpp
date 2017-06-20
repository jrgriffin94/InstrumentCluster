#include "cluster.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Cluster w;
    w.show();

    return a.exec();
}
