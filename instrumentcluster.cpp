#include "instrumentcluster.h"
#include "ui_instrumentcluster.h"

InstrumentCluster::InstrumentCluster(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InstrumentCluster)
{
    ui->setupUi(this);

    connect(&thread, SIGNAL(setSOC(double)), this, SLOT(receiveSOC(double)));

    thread.parse();
}

InstrumentCluster::~InstrumentCluster()
{
    delete ui;
}

void InstrumentCluster::receiveSOC(const double &SOC)
{
    QTextStream out(stdout);
    ui->stateOfCharge->setValue(int(SOC));
    //out << SOC << endl;
}


