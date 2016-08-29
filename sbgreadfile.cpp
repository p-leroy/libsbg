#include "sbgreadfile.h"
#include "ui_sbgreadfile.h"

SbgReadFile* SbgReadFile::sbgReadFile = NULL;
bool SbgReadFile::continueExecution = true;
SbgInterface SbgReadFile::sbgInterface;

unsigned int SbgReadFile::sbgEComLogStatus;
unsigned int SbgReadFile::sbgEComLogEkfEuler;
unsigned int SbgReadFile::sbgEComLogEkfQuat;
unsigned int SbgReadFile::sbgEComLogEkfNav;
unsigned int SbgReadFile::sbgEComLogEventB;
unsigned int SbgReadFile::sbgEComLogGPS1Vel;
unsigned int SbgReadFile::sbgEComLogGPS1Pos;
unsigned int SbgReadFile::sbgEComLogGPS1Hdt;
uint32 SbgReadFile::cursorPosition;
int SbgReadFile::delta;
int SbgReadFile::fileSize;

QFile *SbgReadFile::sbgEComLogEkfEuler_File = NULL;
QTextStream *SbgReadFile::sbgEComLogEkfEuler_Strm = NULL;
QFile *SbgReadFile::sbgEComLogEkfNav_File = NULL;
QTextStream *SbgReadFile::sbgEComLogEkfNav_Strm = NULL;
QFile *SbgReadFile::sbgEComLogEventB_File = NULL;
QTextStream *SbgReadFile::sbgEComLogEventB_Strm = NULL;

SbgReadFile::SbgReadFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SbgReadFile)
{
    ui->setupUi(this);

    SbgReadFile::sbgReadFile = this;

    sbgEComLogEkfEuler_File = new QFile();
    sbgEComLogEkfEuler_Strm = new QTextStream();
    sbgEComLogEkfNav_File = new QFile();
    sbgEComLogEkfNav_Strm = new QTextStream();
    sbgEComLogEventB_File = new QFile();
    sbgEComLogEventB_Strm = new QTextStream();

    initStat();
    readSettings();

    connect( ui->pushButton_openFile, SIGNAL(clicked(bool)),
             this, SLOT(chooseFile()) );
    connect( ui->pushButton_readFile, SIGNAL(clicked(bool)),
             this, SLOT(readFile()) );

    connect( this, SIGNAL(newMessage(QString,unsigned char)),
             this, SLOT(displayMessage(QString,unsigned char)) );
}

SbgReadFile::~SbgReadFile()
{
    writeSettings();
    delete ui;
}

void SbgReadFile::readSettings()
{
    QSettings settings("ietr", "libsbg");

    dataStorageDirectory    = settings.value("sbgreadfile/dataStorageDirectory", QDir::homePath()).toString();
    dataFile                = settings.value("sbgreadfile/dataFile", "").toString();

    ui->label_currentDirectory->setText( dataStorageDirectory );
    ui->label_currentFile->setText( dataFile );
}

void SbgReadFile::writeSettings()
{
    QSettings settings("ietr", "libsbg");

    settings.setValue("sbgreadfile/dataStorageDirectory", dataStorageDirectory);
    settings.setValue("sbgreadfile/dataFile", dataFile);
}

void SbgReadFile::storeSbgEComLogEkfEuler( SbgLogEkfEulerData *log )
{
    *(sbgEComLogEkfEuler_Strm)
            << QString::number( log->timeStamp )
            << ' '
            << QString::number( log->euler[0] )
            << ' '
            << QString::number( log->euler[1] )
            << ' '
            << QString::number( log->euler[2] )
            << ' '
            << QString::number( log->eulerStdDev[0] )
            << ' '
            << QString::number( log->eulerStdDev[1] )
            << ' '
            << QString::number( log->eulerStdDev[2] )
            << ' '
            << QString::number( log->status )
            << endl;
}

void SbgReadFile::storeSbgEComLogEkfNav( SbgLogEkfNavData *log )
{
    *(sbgEComLogEkfNav_Strm)
            << QString::number( log->timeStamp )
            << ' '
            << QString::number( log->velocity[0] )
            << ' '
            << QString::number( log->velocity[1] )
            << ' '
            << QString::number( log->velocity[2] )
            << ' '
            << QString::number( log->velocityStdDev[0] )
            << ' '
            << QString::number( log->velocityStdDev[1] )
            << ' '
            << QString::number( log->velocityStdDev[2] )
            << ' '
            << QString::number( log->position[0] )
            << ' '
            << QString::number( log->position[1] )
            << ' '
            << QString::number( log->position[2] )
            << ' '
            << QString::number( log->undulation )
            << ' '
            << QString::number( log->positionStdDev[0] )
            << ' '
            << QString::number( log->positionStdDev[1] )
            << ' '
            << QString::number( log->positionStdDev[2] )
            << ' '
            << QString::number( log->status )
            << endl;
}

void SbgReadFile::storeSbgEComLogEventB( SbgLogEvent *log )
{
    *(sbgEComLogEventB_Strm)
            << QString::number( log->timeStamp )
            << ' '
            << QString::number( log->status )
            << ' '
            << QString::number( log->timeOffset0 )
            << ' '
            << QString::number( log->timeOffset1 )
            << ' '
            << QString::number( log->timeOffset2 )
            << ' '
            << QString::number( log->timeOffset3 )
            << ' '
            << endl;
}

SbgErrorCode SbgReadFile::onLogReceived(SbgEComHandle *pHandle, SbgEComCmdId logCmd, const SbgBinaryLogData *pLogData, void *pUserArg)
{
    static uint32 nbSbgEComLogStatus = 0;

    switch (logCmd)
    {

    case SBG_ECOM_LOG_STATUS:
        sbgEComLogStatus++;
        nbSbgEComLogStatus++;
        break;

    case SBG_ECOM_LOG_EKF_EULER:
        sbgEComLogEkfEuler++;
        storeSbgEComLogEkfEuler( (SbgLogEkfEulerData*) pLogData );
        break;

    case SBG_ECOM_LOG_EKF_QUAT:
        break;

    case SBG_ECOM_LOG_EKF_NAV:
        sbgEComLogEkfNav++;
        storeSbgEComLogEkfNav( (SbgLogEkfNavData*) pLogData );
        break;

    case SBG_ECOM_LOG_EVENT_B:
        sbgEComLogEventB++;
        storeSbgEComLogEventB( (SbgLogEvent*) pLogData );
        break;

    case SBG_ECOM_LOG_GPS1_VEL:
        break;

    case SBG_ECOM_LOG_GPS1_POS:
        break;

    case SBG_ECOM_LOG_GPS1_HDT:
        break;

    default:
        break;
    }

    cursorPosition = sbgInterfaceFileGetCursor( &sbgInterface );
    delta =  fileSize - cursorPosition;

    if ( nbSbgEComLogStatus == 10000 )
    {
        QApplication::processEvents();
        sbgReadFile->updateStat();
        nbSbgEComLogStatus = 0;
    }

    return SBG_NO_ERROR;
}

int SbgReadFile::sbgPollingLoop()
{
    SbgEComHandle comHandle;
    SbgErrorCode errorCode;
    int32 retValue = 0;
    QString filename;
    unsigned char numberOfSbgNotReady = 0;

    emit sbgReadFile->newMessage("SbgReadFile::sbgPollingLoop ***  try to open "
                              +  sbgReadFile->fileInfo.filePath(), LEVEL_OK );

    filename = sbgReadFile->dataStorageDirectory + "/" + sbgReadFile->dataFile;
    errorCode = sbgInterfaceFileOpen(&sbgInterface, filename.toLatin1().data() );

    continueExecution = true;

    //
    // Test that the interface has been created
    //
    if (errorCode == SBG_NO_ERROR)
    {
        emit sbgReadFile->newMessage("OK *** sbgInterfaceUdpCreate", LEVEL_OK);
        //
        // Create the sbgECom library and associate it with the created interfaces
        //
        errorCode = sbgEComInit(&comHandle, &sbgInterface);

        //
        // Test that the sbgECom has been initialized
        //
        if (errorCode == SBG_NO_ERROR)
        {
            emit sbgReadFile->newMessage( "sbgECom properly Initialized", LEVEL_OK );

            //
            // Define callbacks for received data
            //
            sbgEComSetReceiveCallback(&comHandle, onLogReceived, NULL);

            //
            // Loop until the user exist
            //
            while (continueExecution)
            {
                //
                // Try to read a frame
                //
                errorCode = sbgEComHandle(&comHandle);

                //
                // Test if we have to release some CPU (no frame received)
                //
                if (errorCode == SBG_NOT_READY)
                {     
                    if (sbgInterfaceFileGetCursor( &sbgInterface ) == 0)
                    {
                        emit sbgReadFile->newMessage( "SbgReadFile::sbgPollingLoop *** SBG_NOT_READY, cursor at 0", LEVEL_OK );
                        continueExecution = false;
                    }
                    else
                    {
                        //
                        // Release CPU
                        //
                        sbgSleep(100);
                        numberOfSbgNotReady++;
                        if (numberOfSbgNotReady == 10)
                        {
                            emit sbgReadFile->newMessage( "SbgReadFile::sbgPollingLoop *** too much SBG_NOT_READY", LEVEL_ERR );
                            continueExecution = false;
                        }
                    }
                }
                 else
                {
                    emit sbgReadFile->newMessage( "Error", LEVEL_ERR );
                }
            }
            emit sbgReadFile->newMessage( "sbgPollingLoop *** end of infinite loop", LEVEL_OK );

            //
            // Close the sbgEcom library
            //
            sbgEComClose(&comHandle);
            emit sbgReadFile->newMessage( "sbgPollingLoop *** connection closed", LEVEL_OK);
        }
        else
        {
            //
            // Unable to initialize the sbgECom
            //
            emit sbgReadFile->newMessage("ekinoxMinimal: Unable to initialize the sbgECom library", LEVEL_ERR);
            retValue = -1;
        }

        //
        // Close the interface
        //

        sbgInterfaceFileClose(&sbgInterface);
        emit sbgReadFile->newMessage("sbgPollingLoop *** sbgInterfaceFileClose", LEVEL_OK);

    }
    else
    {
        //
        // Unable to create the interface
        //
        emit sbgReadFile->newMessage("ERR *** sbgInterfaceUdpCreate *** unable to create the interface", LEVEL_ERR);
        emit sbgReadFile->newMessage("ekinoxMinimal: Unable to create the interface.", LEVEL_ERR);
        retValue = -1;
    }

    //
    // Returns -1 if we have an error
    //

    continueExecution = true;

    sbgReadFile->updateStat();
    sbgEComLogEkfEuler_Strm->flush();
    sbgEComLogEkfNav_Strm->flush();
    sbgEComLogEventB_Strm->flush();
    sbgEComLogEkfEuler_File->close();
    sbgEComLogEkfNav_File->close();
    sbgEComLogEventB_File->close();

    emit sbgReadFile->newMessage( "sbgPollingLoop *** end of sbgPollingLoop", LEVEL_OK);

    return retValue;
}

//******
//******
// SLOTS

int SbgReadFile::chooseFirmwareDirectory( const char *hexFilename )
{
    QString filenameWithAbsolutePath;
    QString hexDir;
    int ret;
    QFileInfo checkFile;

    ret = POSAR_MC_FAILED;

    hexDir = QFileDialog::getExistingDirectory(
                NULL,
                "choose hex files directory",
                QDir::homePath());

    if (hexDir.isEmpty())
    {
        ret = POSAR_MC_FAILED;
        emit newMessage("SbgReadFile::choseFirmwareDirectory *** ERR directory not valid", LEVEL_ERR);
    }
    else
    {
        emit newMessage("SbgReadFile::choseFirmwareDirectory *** selected directory: " + QString(hexDir), LEVEL_OK );
        dataStorageDirectory = hexDir;
        checkFile = QFileInfo( filenameWithAbsolutePath);
        if (checkFile.exists())
            ret = POSAR_MC_SUCCESSFUL;
        else
        {
            ret = POSAR_MC_FAILED;
            emit newMessage("SbgReadFile::choseFirmwareDirectory *** file does not exist: "
                            + filenameWithAbsolutePath, LEVEL_ERR);
        }
    }

    return ret;
}

int SbgReadFile::chooseFile(void)
{
    QString filenameWithAbsolutePath;
    int ret;

    ret = POSAR_MC_FAILED;

    filenameWithAbsolutePath = QFileDialog::getOpenFileName(NULL,
                                                            "select a data file from the SBG Inertial Navigation System",
                                                            QDir::homePath(),
                                                            "(*.bin)");

    if (filenameWithAbsolutePath.isEmpty())
    {
        emit newMessage("SbgReadFile::chooseFile *** ERR file not valid", LEVEL_ERR);
        ret = POSAR_MC_FAILED;
    }
    else
    {
        emit newMessage("SbgReadFile::chooseFile *** selected file: " + filenameWithAbsolutePath, LEVEL_OK );
        fileInfo = QFileInfo( filenameWithAbsolutePath );
        dataStorageDirectory = fileInfo.absolutePath();
        dataFile = fileInfo.fileName();
        ui->label_currentDirectory->setText( fileInfo.absoluteFilePath() );
        ui->label_currentFile->setText( fileInfo.fileName() );
        ui->label_size->setText( QString::number(fileInfo.size()) );
        ret = POSAR_MC_SUCCESSFUL;
    }

    return ret;
}

void SbgReadFile::readFile(void)
{
    QString sbgEComLogEkfEuler_filename;
    QString sbgEComLogEkfNav_filename;
    QString sbgEComLogEventB_filename;

    sbgEComLogEkfEuler_filename = dataStorageDirectory + "/sbgEComLogEkfEuler.dat";
    sbgEComLogEkfNav_filename = dataStorageDirectory + "/sbgEComLogEkfNav.dat";
    sbgEComLogEventB_filename = dataStorageDirectory + "/sbgEComLogEventB.dat";

    //************************************
    //************************************
    // create files for the extracted data

    //*******************
    // SbgEComLogEkfEuler
    sbgEComLogEkfEuler_File->setFileName (sbgEComLogEkfEuler_filename );
    if (sbgEComLogEkfEuler_File->open(QIODevice::WriteOnly))
    {
        sbgEComLogEkfEuler_Strm->setDevice( sbgEComLogEkfEuler_File );
        newMessage("[ OK  ] SbgReadFile::readFile *** file created successfully: "
                    + sbgEComLogEkfEuler_filename, LEVEL_OK );
        *(this->sbgEComLogEkfEuler_Strm) << "timeStamp roll pitch yaw rollStdDev pitchStdDev yawStdDev status" << endl;
    }
    else
        newMessage("[ ERR ]SbgReadFile::readFile *** impossible to create file: "
                    + sbgEComLogEkfEuler_filename, LEVEL_ERR );

    //*****************
    // SbgEComLogEkfNav
    sbgEComLogEkfNav_File->setFileName (sbgEComLogEkfNav_filename );
    if (sbgEComLogEkfNav_File->open(QIODevice::WriteOnly))
    {
        sbgEComLogEkfNav_Strm->setDevice( sbgEComLogEkfNav_File );
        newMessage("[ OK  ] SbgReadFile::readFile *** file created successfully: "
                    + sbgEComLogEkfNav_filename, LEVEL_OK );
        *(this->sbgEComLogEkfNav_Strm) << "timeStamp velNorth velEast velDown velNorth_StdDev velEast_StdDev velDown_StdDev "
                                       << "Lat Long Alt undulation Lat_StdDev Long_StdDev Alt_StdDev "
                                       << "status"
                                       << endl;
    }
    else
        newMessage("[ ERR ]SbgReadFile::readFile *** impossible to create file: "
                    + sbgEComLogEkfNav_filename, LEVEL_ERR );

    //*****************
    // SbgEComLogEventB
    sbgEComLogEventB_File->setFileName (sbgEComLogEventB_filename );
    if (sbgEComLogEventB_File->open(QIODevice::WriteOnly))
    {
        sbgEComLogEventB_Strm->setDevice( sbgEComLogEventB_File );
        newMessage("[ OK  ] SbgReadFile::readFile *** file created successfully: "
                    + sbgEComLogEventB_filename, LEVEL_OK );
        *(this->sbgEComLogEventB_Strm) << "timeStamp status timeoffset0 timeoffset1 timeoffset2 timeoffset3" << endl;
    }
    else
        newMessage("[ ERR ]SbgReadFile::readFile *** impossible to create file: "
                    + sbgEComLogEventB_filename, LEVEL_ERR );

    QApplication::processEvents();

    fileInfo = QFileInfo( dataStorageDirectory + "/" + dataFile );
    ui->label_size->setText( QString::number(fileInfo.size()) );
    fileSize = fileInfo.size();
    initStat();
    sbgPollingLoop();
}

void SbgReadFile::displayMessage(QString str, unsigned char level)
{
    QColor color;

    switch (level)
    {
    case LEVEL_ERR:
        color = Qt::red;
        break;
    case LEVEL_WARNING:
        color = Qt::blue;
        break;
    case LEVEL_OK:
        color = Qt::black;
        break;
    default:
        color = Qt::black;
        break;
    }

    ui->textEdit_console->setTextColor( color );
    ui->textEdit_console->append( str );
    QApplication::processEvents();
}

void SbgReadFile::initStat()
{
    sbgEComLogStatus = 0;
    sbgEComLogEkfEuler = 0;
    sbgEComLogEkfQuat = 0;
    sbgEComLogEkfNav = 0;
    sbgEComLogEventB = 0;
    sbgEComLogGPS1Vel = 0;
    sbgEComLogGPS1Pos = 0;
    sbgEComLogGPS1Hdt = 0;
    cursorPosition = 0;
    delta = 0;
}

void SbgReadFile::updateStat()
{
    ui->label_sbgEComLogStatus->setText( QString::number( sbgEComLogStatus ) );
    ui->label_sbgEComLogEkfEuler->setText( QString::number( sbgEComLogEkfEuler ) );
    ui->label_sbgEComLogEkfNav->setText( QString::number( sbgEComLogEkfNav ) );
    ui->label_sbgEComLogEventB->setText( QString::number( sbgEComLogEventB ) );
    ui->label_position->setText( QString::number( cursorPosition ) );
    ui->label_delta->setText( QString::number( delta ) );
}
