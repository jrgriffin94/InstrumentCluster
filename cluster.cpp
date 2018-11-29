#include "cluster.h"
#include "ui_cluster.h"

#define NUM_LAPS    8

Cluster::Cluster(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Cluster)
{
    QTextStream out(stdout);

    ui->setupUi(this);

    QRect rpm_rect = QRect(20, 20, 700, 330);
    rpm_gauge = new RpmGauge(this, rpm_rect);
    rpm_gauge->setObjectName(QStringLiteral("rpmGauge"));
    rpm_gauge->setGeometry( rpm_rect );

    QRect soc_rect = QRect(590, 400, 200, 50);
    soc_gauge = new SocGauge(this, soc_rect, NUM_LAPS);
    soc_gauge->setObjectName(QStringLiteral("socGauge"));
    soc_gauge->setGeometry( soc_rect );

    QRect lt_rect = QRect(650, 20, 200, 300);
    lt_panel = new LapTimePanel(this, lt_rect);
    lt_panel->setObjectName(QStringLiteral("ltPanel"));
    lt_panel->setGeometry( lt_rect );

    QPalette pal;
    pal.setColor(QPalette::Background, Qt::black);
    pal.setColor(QPalette::Foreground, Qt::white);

    ui->motorTemp->setPalette(pal);
    ui->motorTemp->setSegmentStyle(QLCDNumber::Flat);
    ui->motorTemp->setFrameStyle(QFrame::NoFrame);

    ui->mtrCntrlTemp->setPalette(pal);
    ui->mtrCntrlTemp->setSegmentStyle(QLCDNumber::Flat);
    ui->mtrCntrlTemp->setFrameStyle(QFrame::NoFrame);

    ui->highBattTemp->setPalette(pal);
    ui->highBattTemp->setSegmentStyle(QLCDNumber::Flat);
    ui->highBattTemp->setFrameStyle(QFrame::NoFrame);

    this->tempFiltVal   = 512;
    this->motorTemp     = 0;
    this->mtrCntrlTemp  = 0;
    this->highBattTemp  = 0;

    connect(&thread_CAN, SIGNAL(setRPM(int)), this, SLOT(receiveRPM(int)));
    //connect(&thread_CAN, SIGNAL(setRPM(int)), &thread_kf, SLOT(receiveRPM(int)));
    connect(&thread_CAN, SIGNAL(setSOC(int)), this, SLOT(receiveSOC(int)));
    connect(&thread_CAN, SIGNAL(setMtrTemp(int)), this, SLOT(receiveMtrTemp(int)));
    connect(&thread_CAN, SIGNAL(setMtrCntrlTemp(int)), this, SLOT(receiveMtrCntrlTemp(int)));
    connect(&thread_CAN, SIGNAL(setHighBattTemp(int)), this, SLOT(receiveHighBattTemp(int)));

    connect(&thread_GPS, SIGNAL(newLatitude(double)), &thread_kf, SLOT(setLatitude(double)));
    connect(&thread_GPS, SIGNAL(newLongitude(double)), &thread_kf, SLOT(setLongitude(double)));
    connect(&thread_GPS, SIGNAL(coordsUpdated()), &thread_kf, SLOT(runIteration()));
    //connect(&thread_fp_GPS, SIGNAL(newLatitude(double)), &thread_kf, SLOT(setLatitude(double)));
    //connect(&thread_fp_GPS, SIGNAL(newLongitude(double)), &thread_kf, SLOT(setLongitude(double)));
    //connect(&thread_fp_GPS, SIGNAL(coordsUpdated()), &thread_kf, SLOT(runIteration()));

    connect(&thread_kf, SIGNAL(newLatitude(double)), &thread_LT, SLOT(receiveLatitude(double)));
    connect(&thread_kf, SIGNAL(newLongitude(double)), &thread_LT, SLOT(receiveLongitude(double)));

    connect(&thread_LT, SIGNAL(newLapNumber(int)), this, SLOT(receiveLapNumber(int)));
    connect(&thread_LT, SIGNAL(newLastLapTime(int)), this, SLOT(receiveLastLapTime(int)));
    connect(&thread_LT, SIGNAL(newBestLapTime(int)), this, SLOT(receiveBestLapTime(int)));
    connect(&thread_LT, SIGNAL(startOfLap()),

    thread_kf.init();
    thread_CAN.init();
    thread_GPS.init();
    thread_LT.init();
    //thread_fp_GPS.parse();
}

Cluster::~Cluster()
{
    delete &thread_CAN;
    delete &thread_GPS;
    delete &thread_LT;
    delete &thread_kf;
    delete ui;
}

void Cluster::receiveSOC(const int &SOC)
{
    soc_gauge->setSoc(SOC);
    //soc_gauge->setSoc(18);
}

void Cluster::receiveRPM(const int &RPM)
{
    thread_LT.receiveRPM( RPM );
    //thread_LT.receiveRPM( 1310 );

    rpm_gauge->setRpm( RPM );
    //rpm_gauge->setRpm( 8500 );
}

void Cluster::receiveMtrTemp(const int &MtrTemp)
{
    motorTemp = (tempFiltVal * motorTemp + (1024 - tempFiltVal) * MtrTemp) >> 10;
    ui->motorTemp->display(motorTemp);
}

void Cluster::receiveMtrCntrlTemp(const int &MtrCntrlTemp)
{
    mtrCntrlTemp = (tempFiltVal * mtrCntrlTemp + (1024 - tempFiltVal) * MtrCntrlTemp) >> 10;
    ui->mtrCntrlTemp->display(mtrCntrlTemp);
}

void Cluster::receiveHighBattTemp(const int &HighBattTemp)
{
    highBattTemp = (tempFiltVal * highBattTemp + (1024 - tempFiltVal) * HighBattTemp) >> 10;
    ui->highBattTemp->display(highBattTemp);
}

void Cluster::receiveLapNumber(const int &lap_num)
{
    lt_panel->setLapNumber(lap_num);
}

void Cluster::receiveLastLapTime(const int &time)
{
    lt_panel->setLastTime(time);
}

void Cluster::receiveBestLapTime(const int &time)
{
    lt_panel->setBestTime(time);
}


