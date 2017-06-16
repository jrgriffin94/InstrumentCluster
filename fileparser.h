#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <QWaitCondition>
#include <QTextStream>
#include <QDataStream>
#include <QThread>
#include <QMutex>
#include <QFile>

class FileParser : public QThread
{
    Q_OBJECT

public:
    FileParser(QObject *parent = 0);
    ~FileParser();

    bool parse();

signals:
    void parsedData(const QByteArray &data);
    void setSOC(const double SOC);

protected:
    void run() override;

private:
    QWaitCondition condition;
    QMutex mutex;
    bool abort;
    // QFile file;

};

#endif // FILEPARSER_H
