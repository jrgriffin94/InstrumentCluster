#include "cluster.h"
#include "ui_cluster.h"

Cluster::Cluster(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Cluster)
{
    ui->setupUi(this);

    connect(&thread, SIGNAL(setSOC(int)), this, SLOT(receiveSOC(int)));
    //connect(&thread, SIGNAL(setRPM(int)), ui->revPerMin, SLOT(display(int)));
    connect(&thread, SIGNAL(setRPM(int)), this, SLOT(receiveRPM(int)));
    connect(&thread, SIGNAL(setMtrTemp(double)), this, SLOT(receiveMtrTemp(double)));

    thread.parse();
}

Cluster::~Cluster()
{
    delete ui;
}

void Cluster::receiveSOC(const int &SOC)
{
    QTextStream out(stdout);
    ui->stateOfCharge->display(SOC);
}


void Cluster::receiveRPM(const int &RPM)
{
    ui->revPerMin->display(RPM);
}

void Cluster::receiveMtrTemp(const double &MtrTemp)
{
    ui->motorTemp->display(MtrTemp);
}

