#ifndef CLUSTER_H
#define CLUSTER_H

#include <QMainWindow>
#include "fileparser.h"

namespace Ui {
class Cluster;
}

class Cluster : public QMainWindow
{
    Q_OBJECT

public:
    explicit Cluster(QWidget *parent = 0);
    ~Cluster();

private slots:
    void receiveSOC(const int &SOC);
    void receiveRPM(const int &RPM);
    void receiveMtrTemp(const double &MtrTemp);

private:
    Ui::Cluster *ui;
    FileParser thread;
};

#endif // CLUSTER_H
