#include "fileparser.h"

FileParser::FileParser(QObject *parent)
    : QThread(parent)
{
    abort = false;
}

FileParser::~FileParser()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

bool FileParser::parse()
{
    if (!isRunning()) {
        start(LowPriority);
    }

    return true;
}

void FileParser::run()
{
    QFile file("../InstrumentCluster/Data/GeneratedNoisyCoordinateDat.csv");

    QTextStream out(stdout);

    if (!file.open(QIODevice::ReadOnly)) {
        out << "Can't open file" << endl;
        out << file.errorString() << endl;
        mutex.lock();
        condition.wait(&mutex);
        mutex.unlock();
    }

    QByteArray line = file.readLine();

    forever
    {
        if (abort)
            return;

        if (!file.atEnd()) {
            line = file.readLine();

            QString SOCstr = line.split(',')[3];
            QString RPMstr = line.split(',')[5];
            QString MtrTempStr = line.split(',')[6];

            int SOC = SOCstr.toDouble();
            int RPM = RPMstr.toDouble();
            double MtrTemp = MtrTempStr.toDouble();

            emit setSOC(SOC);
            emit setRPM(RPM);
            emit setMtrTemp(MtrTemp);
        }
        else
        {
            out << "Finished Parsing File" << endl;
            file.close();
            mutex.lock();
            condition.wait(&mutex);
            mutex.unlock();
        }

        msleep(5);
    }

    file.close();
}
