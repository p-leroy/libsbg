#include "sbgconnection.h"
#include "ui_sbgconnection.h"

SbgConnection::SbgConnection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SbgConnection)
{
    ui->setupUi(this);

    connect( this->ui->iPLineEdit, SIGNAL(editingFinished()), this, SLOT(updateSbgIp()) );
    connect( this->ui->pushButton_connectToSbg, SIGNAL(clicked(bool)), this, SLOT(startSbgNew()) );

    readSettings();
}

SbgConnection::~SbgConnection()
{
    writeSettings();
    delete ui;
}

void SbgConnection::readSettings()
{
    QSettings settings("ietr", "libsbg");

    sbgIp0 = settings.value("sbgIp0", 0).toUInt();
    sbgIp1 = settings.value("sbgIp1", 0).toUInt();
    sbgIp2 = settings.value("sbgIp2", 0).toUInt();
    sbgIp3 = settings.value("sbgIp3", 0).toUInt();

    this->ui->iPLineEdit->setText( QString::number(sbgIp3) + "."
                                   + QString::number(sbgIp2) + "."
                                   + QString::number(sbgIp1) + "."
                                   + QString::number(sbgIp0));
}

void SbgConnection::writeSettings()
{
    QSettings settings("ietr", "libsbg");

    settings.setValue("sbgIp0", sbgIp0);
    settings.setValue("sbgIp1", sbgIp1);
    settings.setValue("sbgIp2", sbgIp2);
    settings.setValue("sbgIp3", sbgIp3);
}

void SbgConnection::updateSbgIp()
{
    QString ipAsText;
    QStringList ipAsList;
    int nbPointsInString;
    int k;
    int value;
    bool ipAsTextWellFormed = true;

    // check the QLineEdit content
    ipAsText =  this->ui->iPLineEdit->text();
    nbPointsInString =  ipAsText.count(".");

    if (nbPointsInString==3)
    {
        ipAsList =  ipAsText.split(".");
        for (k=0; k<nbPointsInString+1; k++)
        {
            value = ipAsList.at(k).toInt();
            if ( (value<0) || (value>255) )
            {
                ipAsTextWellFormed = false;
            }
        }
    }
    else
    {
        ipAsTextWellFormed = false;
    }

    // perform actions depending on the content of QLineEdit
    if (ipAsTextWellFormed)
    {
        sbgIp0 = (unsigned char) ipAsList.at(3).toInt();
        sbgIp1 = (unsigned char) ipAsList.at(2).toInt();
        sbgIp2 = (unsigned char) ipAsList.at(1).toInt();
        sbgIp3 = (unsigned char) ipAsList.at(0).toInt();
    }
    else
    {
        emit this->sendMessage("ERR *** IP address not valid");
    }
}

void SbgConnection::startSbgNew()
{
    static bool threadRunning = false;

    if (threadRunning)
    {
        emit sig_closeSbgConnection();
        this->ui->pushButton_connectToSbg->setText("Connect to SBG");
        threadRunning = false;
    }
    else
    {
        thread = new QThread;
        sbgNew = new SbgNew();
        sbgNew->moveToThread(thread);

        connect(thread, SIGNAL(started()),  sbgNew, SLOT(initSbg()));
        connect(sbgNew, SIGNAL(finished()), thread, SLOT(quit()));
        connect(sbgNew, SIGNAL(finished()), sbgNew, SLOT(deleteLater()));
        // SBG EKINOX
        connect(this, SIGNAL(sig_sbgIpChanged(unsigned char, QByteArray)),
                this->sbgNew, SLOT(updateSbgIp(unsigned char,unsigned char,unsigned char,unsigned char)), Qt::DirectConnection);
        connect(this, SIGNAL(sig_closeSbgConnection()),
                this->sbgNew, SLOT(closeSbgConnection()), Qt::DirectConnection);

        //****************
        // forward signals
        connect( this->sbgNew, SIGNAL(sendMessage(QString)),                this, SLOT(forwardMessage(QString)) );
        connect( this->sbgNew, SIGNAL(newSbgEcomLogStatus(QByteArray)),     this, SLOT(forwardNewSbgEcomLogStatus(QByteArray)) );
        connect( this->sbgNew, SIGNAL(newSbgEcomLogEkfEuler(QByteArray)),   this, SLOT(forwardNewSbgEcomLogEkfEuler(QByteArray)) );
        connect( this->sbgNew, SIGNAL(newSbgEcomLogEkfNav(QByteArray)),     this, SLOT(forwardNewSbgEcomLogEkfNav(QByteArray)) );
        connect( this->sbgNew, SIGNAL(newSbgEcomLogEventB(QByteArray)),     this, SLOT(forwardNewSbgEcomLogEventB(QByteArray)) );
        connect( this->sbgNew, SIGNAL(newSbgEcomLogGPS1Vel(QByteArray)),    this, SLOT(forwardNewSbgEcomLogGPS1Vel(QByteArray)) );
        connect( this->sbgNew, SIGNAL(newSbgEcomLogGPS1Pos(QByteArray)),    this, SLOT(forwardNewSbgEcomLogGPS1Pos(QByteArray)) );
        connect( this->sbgNew, SIGNAL(newSbgEcomLogGPS1Hdt(QByteArray)),    this, SLOT(forwardNewSbgEcomLogGPS1Hdt(QByteArray)) );

        this->ui->pushButton_connectToSbg->setText("Disconnect from SBG");

        SbgNew::sbgIp0 = sbgIp0;
        SbgNew::sbgIp1 = sbgIp1;
        SbgNew::sbgIp2 = sbgIp2;
        SbgNew::sbgIp3 = sbgIp3;

        thread->start();
        threadRunning = true;
    }
}