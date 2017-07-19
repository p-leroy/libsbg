#ifndef SBGNEW_H
#define SBGNEW_H

#include <QWidget>
#include <QApplication>

#include <sbgEComLib.h>

class SbgNew : public QObject
{
    Q_OBJECT
public:
    explicit SbgNew(QObject *parent = 0);
    static SbgErrorCode onLogReceived(SbgEComHandle *pHandle, SbgEComCmdId logCmd, const SbgBinaryLogData *pLogData, void *pUserArg);
    static void setSbgNew(void *ptr);
    static void setReadDataTimer(void);
    static int sbgPollingLoop();
    static void updateSbgIp(unsigned char ip3, unsigned char ip2, unsigned char ip1, unsigned char ip0);
    static void delay(int millisecondsToWait);

    static bool continueExecution;
    static unsigned char sbgIp0;
    static unsigned char sbgIp1;
    static unsigned char sbgIp2;
    static unsigned char sbgIp3;

private:
    static SbgNew *sbgNew;

signals:
    void sendMessage(QString);
    void newSbgEcomLogStatus( QByteArray array );
    void newSbgEcomLogEkfEuler( QByteArray array );
    void newSbgEcomLogEkfNav( QByteArray array );
    void newSbgEcomLogEventB( QByteArray array );
    void newSbgEcomLogGPS1Vel( QByteArray array );
    void newSbgEcomLogGPS1Pos( QByteArray array );
    void newSbgEcomLogGPS1Hdt( QByteArray array );
    void finished();
    void isReady( bool );

public slots:
    void initSbg();
    void closeSbgConnection();

};

#endif // SBGNEW_H
