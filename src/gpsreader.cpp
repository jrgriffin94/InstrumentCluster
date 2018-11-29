#include "gpsreader.h"

GpsReader::GpsReader(QObject *parent)
    : QThread(parent){

    serialPort.setPortName("/dev/ttyS0");
    serialPort.setBaudRate(9600);
    if (!serialPort.open(QIODevice::ReadWrite))
    {
        //out << "Ouch! I'm broken!" << endl;
        //out << serialPort.error() << endl;
        return;
    }
    abort = false;
}

GpsReader::~GpsReader()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}


bool GpsReader::init()
{
    if (!isRunning()) {
        start(LowPriority);
    }

    return true;
}


void GpsReader::run()
{
    QTextStream out(stdout);

    // Change Baud Rate to 57600
    //serialPort.write("$PMTK251,57600*2C\r\n");
    //serialPort.setBaudRate(57600);
    //msleep(5000);

    // Change Update Rate to 200 ms
    serialPort.write( "$PMTK220,200*2C\r\n");
    msleep(1000);

    // Change Measurement Rate to 200 ms
    serialPort.write("$PMTK300,200,0,0,0,0*2F\r\n");
    msleep(1000);

    // Limit GPS Output Messages
    serialPort.write("$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n");
    msleep(1000);


    QString message;
    QString chk_msg;
    QStringList message_split;

    QString lat;
    QString lng;

    int checksum;
    double latitude;
    double longitude;
    double speed;
    double heading;

    serialPort.flush();

    forever
    {
        if ( this->abort )
            return;

        message = serialPort.readLine();
        message_split = message.split(',');

        if (message_split[0] == "$GPRMC")
        {
            chk_msg = message.mid(1, message.length()-6);

            // Checksum
            checksum = 0;
            for(int i = 0; i < chk_msg.length(); i++)
            {
                checksum = checksum ^ (uint8_t)(chk_msg[i].toLatin1());
            }

            if ( checksum == message.mid(message.length()-4, 2).toUInt(nullptr, 16))
            {
                if ( message_split[4] == 'N')
                    latitude = message_split[3].toDouble();
                else if ( message_split[4] == 'S')
                    latitude = -1 * message_split[3].toDouble();
                else
                    continue;

                if ( message_split[6] == 'E')
                    longitude = message_split[5].toDouble();
                else if ( message_split[6] == 'W')
                    longitude = -1 * message_split[5].toDouble();
                else
                    continue;

                speed   = message_split[7].toDouble();
                heading = message_split[8].toDouble();

                out << latitude << endl;
                out << longitude << endl;

                emit newLatitude(latitude);
                emit newLongitude(longitude);
                emit newSpeed(speed);
                emit newHeading(heading);
                emit coordsUpdated();
            }
        }

        msleep(100);
    }
}
