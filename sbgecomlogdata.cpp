#include "sbgecomlogdata.h"
#include "ui_sbgecomlogdata.h"

SbgEcomLogData::SbgEcomLogData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SbgEcomLogData)
{
    ui->setupUi(this);
}

SbgEcomLogData::~SbgEcomLogData()
{
    delete ui;
}

void SbgEcomLogData::updateSbgEcomLogEkfEuler( QByteArray data )
{
    SbgLogEkfEulerData *sbgLog;

    sbgLog = (SbgLogEkfEulerData*) data.data();

    this->ui->label_EkfEulerTimeStamp->setText( QString::number(sbgLog->timeStamp/1000000., 'f', 6) );
    this->ui->label_roll_2->setText(  QString::number(sbgLog->euler[0] * 180.0 / SBG_PI, 'f', 2) );
    this->ui->label_pitch_2->setText( QString::number(sbgLog->euler[1] * 180.0 / SBG_PI, 'f', 2) );
    this->ui->label_yaw_2->setText(   QString::number(sbgLog->euler[2] * 180.0 / SBG_PI, 'f', 2) );
}

void SbgEcomLogData::updateSbgEcomLogEkfNav( QByteArray data )
{
    SbgLogEkfNavData *sbgLog;

    sbgLog = (SbgLogEkfNavData*) data.data();

    this->ui->label_EkfNavTimeStamp->setText( QString::number(sbgLog->timeStamp/1000000., 'f', 6) );
    this->ui->velocity0Label_2->setText( QString::number(sbgLog->velocity[0], 'f', 2) );
    this->ui->velocity1Label_2->setText( QString::number(sbgLog->velocity[1], 'f', 2) );
    this->ui->velocity2Label_2->setText( QString::number(sbgLog->velocity[2], 'f', 2) );
    this->ui->position0Label_2->setText( QString::number(sbgLog->position[0], 'f', 2) );
    this->ui->position1Label_2->setText( QString::number(sbgLog->position[1], 'f', 2) );
    this->ui->position2Label_2->setText( QString::number(sbgLog->position[2], 'f', 2) );
    this->ui->undulationLabel_2->setText( QString::number(sbgLog->undulation, 'f', 2) );
}

void SbgEcomLogData::updateSbgEcomLogEventB( QByteArray data )
{
    SbgLogEvent *sbgLog;

    sbgLog = (SbgLogEvent*) data.data();

    static quint32 previousTimeStamp;
    static quint32 deltaTimeStamps;
    double deltaTimeStamps_ms;

    deltaTimeStamps = sbgLog->timeStamp - previousTimeStamp;
    previousTimeStamp = sbgLog->timeStamp;
    deltaTimeStamps_ms = ((double) deltaTimeStamps) / 1000.;

    this->ui->label_TimeStamp->setText(   QString::number(sbgLog->timeStamp/1000000.,'f',6)   );
    this->ui->label_status->setText(      QString::number(sbgLog->status)      );
    this->ui->label_timeOffset0->setText( QString::number(sbgLog->timeOffset0) );
    this->ui->label_timeOffset1->setText( QString::number(sbgLog->timeOffset1) );
    this->ui->label_timeOffset2->setText( QString::number(sbgLog->timeOffset2) );
    this->ui->label_timeOffset3->setText( QString::number(sbgLog->timeOffset3) );
    this->ui->label_deltaTimeStamp->setText( QString::number( deltaTimeStamps_ms ) );
}
