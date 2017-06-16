#include "fileparser.h"

FileParser::FileParser(QObject *parent)
    : QThread(parent)
{
    //QFile file("C:\Users\Joseph Griffin\Documents\InstrumentCloster\Data\GeneratedNoisyCoordinateDat.csv");
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
    QFile file("../InstrumentCloster/Data/GeneratedNoisyCoordinateDat.csv");

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
            double SOC = SOCstr.toDouble();
            emit setSOC(SOC);
        }
        else
        {
            out << "Finished Parsing File" << endl;
            file.close();
            mutex.lock();
            condition.wait(&mutex);
            mutex.unlock();
        }

        usleep(1);
    }

    file.close();
}
