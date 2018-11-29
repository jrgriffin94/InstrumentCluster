#ifndef CLUSTER_H
#define CLUSTER_H

#include <QMainWindow>
#include "fileparser.h"
#include "alphakalmanfilter.h"
#include "canreader.h"
#include "gpsreader.h"
#include "rpmgauge.h"
#include "socgauge.h"
#include "laptiming.h"
#include "laptimepanel.h"

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
    void receiveMtrTemp(const int &MtrTemp);
    void receiveMtrCntrlTemp(const int &MtrCntrlTemp);
    void receiveHighBattTemp(const int &HighBattTemp);
    void receiveLapNumber(const int &lap_num);
    void receiveLastLapTime(const int &time);
    void receiveBestLapTime(const int &time);

private:
    Ui::Cluster         *ui;
    RpmGauge            *rpm_gauge;
    SocGauge            *soc_gauge;
    LapTimePanel        *lt_panel;
    //TempGauge           *temp_gauge;
    QWidget             *imageLabel;

    LapTiming           thread_LT;
    CanReader           thread_CAN;
    GpsReader           thread_GPS;
    FileParser          thread_fp_GPS;
    AlphaKalmanFilter   thread_kf;

    int                 tempFiltVal;
    int                 motorTemp;
    int                 mtrCntrlTemp;
    int                 highBattTemp;
};

#endif // CLUSTER_H
