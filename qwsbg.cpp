#include "qwsbg.h"
#include "ui_qwsbg.h"
#include <qthread.h>

QWSbg::QWSbg(QWidget *parent) :
    QWidget(parent),  ui(new Ui::QWSbg)
   {
    ui->setupUi(this);

    // STATUS
    connect( this->ui->sbgConnection, SIGNAL(newSbgEcomLogStatus(QByteArray)),
             this->ui->sbgEcomLogStatus, SLOT(updateSbgEcomLogStatus(QByteArray)));
    connect( this->ui->sbgConnection, SIGNAL(newSbgEcomLogGPS1Vel(QByteArray)),
             this->ui->sbgEcomLogStatus, SLOT(updateSbgEcomLogGps1Vel(QByteArray)));
    connect( this->ui->sbgConnection, SIGNAL(newSbgEcomLogGPS1Pos(QByteArray)),
             this->ui->sbgEcomLogStatus, SLOT(updateSbgEcomLogGps1Pos(QByteArray)));
    connect( this->ui->sbgConnection, SIGNAL(newSbgEcomLogGPS1Hdt(QByteArray)),
             this->ui->sbgEcomLogStatus, SLOT(updateSbgEcomLogGps1Hdt(QByteArray)));

    // DATA
    connect( this->ui->sbgConnection, SIGNAL(newSbgEcomLogEkfEuler(QByteArray)),
             this->ui->sbgEcomLogData, SLOT(updateSbgEcomLogEkfEuler(QByteArray)));
    connect( this->ui->sbgConnection, SIGNAL(newSbgEcomLogEkfNav(QByteArray)),
             this->ui->sbgEcomLogData, SLOT(updateSbgEcomLogEkfNav(QByteArray)));
    connect( this->ui->sbgConnection, SIGNAL(newSbgEcomLogEventB(QByteArray)),
             this->ui->sbgEcomLogData, SLOT(updateSbgEcomLogEventB(QByteArray)));
    }

QWSbg::~QWSbg()
{
    delete ui;
}
