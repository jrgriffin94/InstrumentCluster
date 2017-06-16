#ifndef INSTRUMENTCLUSTER_H
#define INSTRUMENTCLUSTER_H

#include <QMainWindow>
#include "fileparser.h"

namespace Ui {
class InstrumentCluster;
}

class InstrumentCluster : public QMainWindow
{
    Q_OBJECT

public:
    explicit InstrumentCluster(QWidget *parent = 0);
    ~InstrumentCluster();

private slots:
    void receiveSOC(const double &SOC);

private:
    Ui::InstrumentCluster *ui;
    FileParser thread;

};

#endif // INSTRUMENTCLUSTER_H
