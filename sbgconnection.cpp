#include "sbgconnection.h"
#include "ui_sbgconnection.h"

SbgConnection::SbgConnection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SbgConnection)
{
    ui->setupUi(this);

    threadRunning = false;

    connect( this->ui->iPLineEdit, SIGNAL(editingFinished()), this, SLOT(updateSbgIp()) );
    connect( this->ui->pushButton_connectToSbg, SIGNAL(toggled(bool)), this, SLOT(sbgConnectionRequested(bool)) );

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
    int port;

    sbgIp0 = settings.value("sbgIp0", 0).toUInt();
    sbgIp1 = settings.value("sbgIp1", 0).toUInt();
    sbgIp2 = settings.value("sbgIp2", 0).toUInt();
    sbgIp3 = settings.value("sbgIp3", 0).toUInt();

    this->ui->iPLineEdit->setText( QString::number(sbgIp3) + "."
                                   + QString::number(sbgIp2) + "."
                                   + QString::number(sbgIp1) + "."
                                   + QString::number(sbgIp0));
    port = settings.value("inputPort").toInt();
    this->ui->inputPortSpinBox->setValue(port);
    SbgNew::setLocalPort(static_cast<uint32>(port));

    port = settings.value("outputPort").toInt();
    this->ui->outputPortSpinBox->setValue(port);
    SbgNew::setRemotePort(static_cast<uint32>(port));
}

void SbgConnection::writeSettings()
{
    QSettings settings("ietr", "libsbg");

    settings.setValue("sbgIp0", sbgIp0);
    settings.setValue("sbgIp1", sbgIp1);
    settings.setValue("sbgIp2", sbgIp2);
    settings.setValue("sbgIp3", sbgIp3);

    settings.setValue("inputPort", this->ui->inputPortSpinBox->value());
    settings.setValue("outputPort", this->ui->outputPortSpinBox->value());
}

bool SbgConnection::pushButtonConnectToSbgIsChecked()
{
    return this->ui->pushButton_connectToSbg->isChecked();
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
        emit this->sendMessage("ERR *** IP address not valid", LEVEL_ERR);
    }
}

void SbgConnection::toggleSbgNew()
{
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
        connect(this, SIGNAL(sig_closeSbgConnection()),
                this->sbgNew, SLOT(closeSbgConnection()), Qt::DirectConnection);

        //****************
        // forward signals
        connect( this->sbgNew, SIGNAL(newMessage(QString, unsigned char)),
                 this, SLOT(forwardMessage(QString, unsigned char)) );
        connect( this->sbgNew, SIGNAL(isReady(bool)),                       this, SLOT(forwardIsReady(bool)));
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

void SbgConnection::setConnectToSbg(bool state)
{
    if (state!=ui->pushButton_connectToSbg->isChecked())
        ui->pushButton_connectToSbg->setChecked(state);
}

void SbgConnection::sbgConnectionRequested(bool state)
{
    if (state!=threadRunning)
    {
        toggleSbgNew();
        this->ui->pushButton_connectToSbg->setChecked( state );
        if (state == true)
            this->ui->pushButton_connectToSbg->setText("Disconnect");
        else
            this->ui->pushButton_connectToSbg->setText("Connect");
    }
    else
        emit isReady( true );
}
