#include "instrumentcluster.h"
#include <QApplication>
#include <QThreadPool>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InstrumentCluster w;
    w.show();


    return a.exec();
}
