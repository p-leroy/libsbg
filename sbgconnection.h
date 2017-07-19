#ifndef SBGCONNECTION_H
#define SBGCONNECTION_H

#include <QWidget>

#include <QThread>
#include <QSettings>

#include "sbgnew.h"

#include "libsbg_global.h"

namespace Ui {
class SbgConnection;
}

class LIBSBGSHARED_EXPORT SbgConnection : public QWidget
{
    Q_OBJECT

public:
    explicit SbgConnection(QWidget *parent = 0);
    ~SbgConnection();
    void readSettings();
    void writeSettings();

signals:
    void sig_closeSbgConnection();
    void sendMessage(QString);
    void newSbgEcomLogStatus( QByteArray array );
    void newSbgEcomLogEkfEuler( QByteArray array );
    void newSbgEcomLogEkfNav( QByteArray array );
    void newSbgEcomLogEventB( QByteArray array );
    void newSbgEcomLogGPS1Vel( QByteArray array );
    void newSbgEcomLogGPS1Pos( QByteArray array );
    void newSbgEcomLogGPS1Hdt( QByteArray array );
    void isReady( bool );

private:
    Ui::SbgConnection *ui;
    QThread* thread;
    SbgNew* sbgNew;

    unsigned char sbgIp0;
    unsigned char sbgIp1;
    unsigned char sbgIp2;
    unsigned char sbgIp3;

    bool threadRunning;

public slots:
    void updateSbgIp();
    void toggleSbgNew();
    void sbgConnectionRequested(bool state);
    void forwardMessage(QString str) { emit sendMessage(str); }
    void forwardIsReady(bool state) { emit isReady(state); }
    void forwardNewSbgEcomLogStatus(    QByteArray array ) {emit newSbgEcomLogStatus(   array );}
    void forwardNewSbgEcomLogEkfEuler(  QByteArray array ) {emit newSbgEcomLogEkfEuler( array );}
    void forwardNewSbgEcomLogEkfNav(    QByteArray array ) {emit newSbgEcomLogEkfNav(   array );}
    void forwardNewSbgEcomLogEventB(    QByteArray array ) {emit newSbgEcomLogEventB(   array );}
    void forwardNewSbgEcomLogGPS1Vel(   QByteArray array ) {emit newSbgEcomLogGPS1Vel(  array );}
    void forwardNewSbgEcomLogGPS1Pos(   QByteArray array ) {emit newSbgEcomLogGPS1Pos(  array );}
    void forwardNewSbgEcomLogGPS1Hdt(   QByteArray array ) {emit newSbgEcomLogGPS1Hdt(  array );}
};

#endif // SBGCONNECTION_H
