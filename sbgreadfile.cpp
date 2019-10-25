#include "sbgreadfile.h"
#include "ui_sbgreadfile.h"

SbgReadFile* SbgReadFile::sbgReadFile = nullptr;
bool SbgReadFile::continueExecution = true;
SbgInterface SbgReadFile::sbgInterface;

unsigned int SbgReadFile::sbgEComLogStatus;
unsigned int SbgReadFile::sbgEComLogEkfEuler;
unsigned int SbgReadFile::sbgEComLogEkfQuat;
unsigned int SbgReadFile::sbgEComLogEkfNav;
unsigned int SbgReadFile::sbgEComLogEventB;
unsigned int SbgReadFile::sbgLogUtcData;
// GPS
unsigned int SbgReadFile::sbgLogGpsPos;
bool SbgReadFile::storeGpsPos;
unsigned int SbgReadFile::sbgLogGpsVel;
unsigned int SbgReadFile::sbgLogGpsHdt;

uint32 SbgReadFile::cursorPosition;
int SbgReadFile::delta;
int SbgReadFile::fileSize;

QFile *SbgReadFile::sbgEComLogEkfEuler_File = nullptr;
QTextStream *SbgReadFile::sbgEComLogEkfEuler_Strm = nullptr;
QFile *SbgReadFile::sbgEComLogEkfNav_File = nullptr;
QTextStream *SbgReadFile::sbgEComLogEkfNav_Strm = nullptr;
QFile *SbgReadFile::sbgEComLogEventB_File = nullptr;
QTextStream *SbgReadFile::sbgEComLogEventB_Strm = nullptr;
QFile *SbgReadFile::sbgEComLogStatus_File = nullptr;
QTextStream *SbgReadFile::sbgEComLogStatus_Strm = nullptr;
QFile *SbgReadFile::sbgLogUtcData_File = nullptr;
QTextStream *SbgReadFile::sbgLogUtcData_Strm = nullptr;
// GPS
QFile *SbgReadFile::sbgLogGpsPos_File = nullptr;
QTextStream *SbgReadFile::sbgLogGpsPos_Strm = nullptr;
QFile *SbgReadFile::sbgLogGpsVel_File = nullptr;
QTextStream *SbgReadFile::sbgLogGpsVel_Strm = nullptr;
QFile *SbgReadFile::sbgLogGpsHdt_File = nullptr;
QTextStream *SbgReadFile::sbgLogGpsHdt_Strm = nullptr;

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
    sbgEComLogStatus_File = new QFile();
    sbgEComLogStatus_Strm = new QTextStream();
    sbgLogUtcData_File = new QFile();
    sbgLogUtcData_Strm = new QTextStream();
    // GPS
    sbgLogGpsPos_File = new QFile();
    sbgLogGpsPos_Strm = new QTextStream();
    sbgLogGpsVel_File = new QFile();
    sbgLogGpsVel_Strm = new QTextStream();
    sbgLogGpsHdt_File = new QFile();
    sbgLogGpsHdt_Strm = new QTextStream();

    initStat();
    readSettings();

    connect( ui->pushButton_openFile, SIGNAL(clicked(bool)),
             this, SLOT(chooseFile()) );
    connect( ui->pushButton_readFile, SIGNAL(clicked(bool)),
             this, SLOT(readFile()) );

    connect( this, SIGNAL(newMessage(QString,unsigned char)),
             this, SLOT(displayMessage(QString,unsigned char)) );

    connect( this->ui->checkBox_sbgLogGpsPos, SIGNAL(clicked(bool)), this, SLOT(updateStoreGpsPos(bool)) );

    storeGpsPos = false;
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
            << QString::number( log->position[0], 'g', 16 )
            << ' '
            << QString::number( log->position[1], 'g', 16 )
            << ' '
            << QString::number( log->position[2], 'g', 16 )
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

void SbgReadFile::storeSbgEComLogStatus(SbgLogStatusData *log)
{
    *(sbgEComLogStatus_Strm)
            << QString::number( log->timeStamp )
            << ' '
            << QString::number( log->generalStatus )
            << ' '
            << QString::number( log->reserved1 )
            << ' '
            << QString::number( log->comStatus )
            << ' '
            << QString::number( log->aidingStatus )
            << ' '
            << QString::number( log->reserved2 )
            << ' '
            << QString::number( log->reserved3 )
            << ' '
            << endl;
}

void SbgReadFile::storeSbgLogUtcData( SbgLogUtcData *log )
{
    *(sbgLogUtcData_Strm)
            << QString::number( log->timeStamp )
            << ' '
            << QString::number( log->status )
            << ' '
            << QString::number( log->year )
            << ' '
            << QString::number( log->month )
            << ' '
            << QString::number( log->day )
            << ' '
            << QString::number( log->hour )
            << ' '
            << QString::number( log->minute )
            << ' '
            << QString::number( log->second )
            << ' '
            << QString::number( log->nanoSecond )
            << ' '
            << QString::number( log->gpsTimeOfWeek )
            << ' '
            << endl;
}

void SbgReadFile::storeSbgLogGpsPos(SbgLogGpsPos *log)
{
    *(sbgLogGpsPos_Strm)
            << QString::number( log->timeStamp )
            << ", "
            << QString::number( log->status )
            << ", "
            << QString::number( log->timeOfWeek )
            << ", "
            << QString::number( log->latitude, 'g', 16 )
            << ", "
            << QString::number( log->longitude, 'g', 16 )
            << ", "
            << QString::number( log->altitude, 'g', 16 )
            << ", "
            << QString::number( log->undulation )
            << ", "
            << QString::number( log->latitudeAccuracy )
            << ", "
            << QString::number( log->longitudeAccuracy )
            << ", "
            << QString::number( log->altitudeAccuracy )
            << ", "
            << QString::number( log->numSvUsed )
            << ", "
            << QString::number( log->baseStationId )
            << ", "
            << QString::number( log->differentialAge )
            << ", "
            << endl;
}

void SbgReadFile::storeSbgLogGpsVel(SbgLogGpsVel *log)
{
    *(sbgLogGpsVel_Strm)
            << QString::number( log->timeStamp )
            << ' '
            << QString::number( log->status )
            << ' '
            << QString::number( log->timeOfWeek )
            << ' '
            << QString::number( log->velocity[0] )
            << ' '
            << QString::number( log->velocity[1] )
            << ' '
            << QString::number( log->velocity[2] )
            << ' '
            << QString::number( log->velocityAcc[0] )
            << ' '
            << QString::number( log->velocityAcc[1] )
            << ' '
            << QString::number( log->velocityAcc[2] )
            << ' '
            << QString::number( log->course )
            << ' '
            << QString::number( log->courseAcc )
            << ' '
            << endl;
}

void SbgReadFile::storeSbgLogGpsHdt(SbgLogGpsHdt *log)
{
    *(sbgLogGpsHdt_Strm)
            << QString::number(log->timeStamp)
            << ' '
            << QString::number(log->status)
            << ' '
            << QString::number( log->timeOfWeek )
            << ' '
            << QString::number(log->heading)
            << ' '
            << QString::number( log->headingAccuracy)
            << ' '
            << QString::number(log->pitch)
            << ' '
            << QString::number(log->pitchAccuracy)
            << ' '
            << endl;
}

SbgErrorCode SbgReadFile::receiveLogFunc(SbgEComHandle *pHandle, SbgEComClass msgClass, SbgEComMsgId msg, const SbgBinaryLogData *pLogData, void *pUserArg)
{
    static uint32 nbSbgEComLogStatus = 0;

    if (msgClass != SBG_ECOM_CLASS_LOG_ECOM_0)
    {
        emit sbgReadFile->newMessage( "msgClass is not SBG_ECOM_CLASS_LOG_ECOM_0, unexpected message", LEVEL_ERR );
        return SBG_ERROR;
    }

    switch (msg)
    {
    case SBG_ECOM_LOG_STATUS:
        sbgEComLogStatus++;
        storeSbgEComLogStatus( (SbgLogStatusData*) pLogData );
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

    case SBG_ECOM_LOG_UTC_TIME:
        sbgLogUtcData++;
        storeSbgLogUtcData( (SbgLogUtcData*) pLogData );
        break;

        //*****
        //*****
        // GPS1
    case SBG_ECOM_LOG_GPS1_VEL:
        sbgLogGpsVel++;
        if ( storeGpsPos )
        {
            storeSbgLogGpsVel( (SbgLogGpsVel*) pLogData );
        }
        break;

    case SBG_ECOM_LOG_GPS1_POS:
        sbgLogGpsPos++;
        if ( storeGpsPos )
        {
            storeSbgLogGpsPos( (SbgLogGpsPos*) pLogData );
        }
        break;

    case SBG_ECOM_LOG_GPS1_HDT:
        sbgLogGpsHdt++;
        if ( storeGpsPos )
        {
            storeSbgLogGpsHdt( (SbgLogGpsHdt*) pLogData );
        }
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
            sbgEComSetReceiveLogCallback(&comHandle, receiveLogFunc, nullptr);

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
    sbgEComLogStatus_File->close();
    sbgLogUtcData_File->close();

    emit sbgReadFile->newMessage( "sbgPollingLoop *** end of sbgPollingLoop", LEVEL_OK);

    return retValue;
}

//******
//******
// SLOTS

int SbgReadFile::chooseFile(void)
{
    QString filenameWithAbsolutePath;
    int ret;

    ret = POSAR_MC_FAILED;

    if (QDir(dataStorageDirectory).exists())
    {
        filenameWithAbsolutePath = QFileDialog::getOpenFileName(nullptr,
                                                                "select a data file from the SBG Inertial Navigation System",
                                                                dataStorageDirectory,
                                                                "(*.bin)");
    }
    else
    {
        filenameWithAbsolutePath = QFileDialog::getOpenFileName(nullptr,
                                                                "select a data file from the SBG Inertial Navigation System",
                                                                QDir::homePath(),
                                                                "(*.bin)");
    }

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
    QString sbgEComLogStatus_filename;
    QString sbgLogUtcData_filename;
    QString sbgLogGpsPos_filename;
    QString sbgLogGpsVel_filename;
    QString sbgLogGpsHdt_filename;

    sbgEComLogEkfEuler_filename = dataStorageDirectory + "/sbgLogEkfEuler.dat";
    sbgEComLogEkfNav_filename = dataStorageDirectory + "/sbgLogEkfNav.dat";
    sbgEComLogEventB_filename = dataStorageDirectory + "/sbgLogEventB.dat";
    sbgEComLogStatus_filename = dataStorageDirectory + "/sbgLogStatus.dat";
    sbgLogUtcData_filename = dataStorageDirectory + "/sbgLogUtcData.dat";
    sbgLogGpsPos_filename = dataStorageDirectory + "/sbgLogGpsPos.csv";
    sbgLogGpsVel_filename = dataStorageDirectory + "/sbgLogGpsVel.dat";
    sbgLogGpsHdt_filename = dataStorageDirectory + "/sbgLogGpsHdt.dat";

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

    //*****************
    // SbgEComLogStatus
    sbgEComLogStatus_File->setFileName (sbgEComLogStatus_filename );
    if (sbgEComLogStatus_File->open(QIODevice::WriteOnly))
    {
        sbgEComLogStatus_Strm->setDevice( sbgEComLogStatus_File );
        newMessage("[ OK  ] SbgReadFile::readFile *** file created successfully: "
                   + sbgEComLogStatus_filename, LEVEL_OK );
        *(this->sbgEComLogStatus_Strm) << "timeStamp generalStatus reserved1 comStatus aidingStatus reserved2 reserved3" << endl;
    }
    else
        newMessage("[ ERR ]SbgReadFile::readFile *** impossible to create file: "
                   + sbgEComLogStatus_filename, LEVEL_ERR );

    //**************
    // SbgLogUtcData
    sbgLogUtcData_File->setFileName ( sbgLogUtcData_filename );
    if (sbgLogUtcData_File->open(QIODevice::WriteOnly))
    {
        sbgLogUtcData_Strm->setDevice( sbgLogUtcData_File );
        newMessage("[ OK  ] SbgReadFile::readFile *** file created successfully: "
                   + sbgLogUtcData_filename, LEVEL_OK );
        *(this->sbgLogUtcData_Strm) << "timeStamp status year month day hour minute second nanoSecond gpsTimeOfWeek" << endl;
    }
    else
        newMessage("[ ERR ]SbgReadFile::readFile *** impossible to create file: "
                   + sbgLogUtcData_filename, LEVEL_ERR );

    //*************
    // SbgLogGpsPos
    sbgLogGpsPos_File->setFileName ( sbgLogGpsPos_filename );
    if (sbgLogGpsPos_File->open(QIODevice::WriteOnly))
    {
        sbgLogGpsPos_Strm->setDevice( sbgLogGpsPos_File );
        newMessage("[ OK  ] SbgReadFile::readFile *** file created successfully: "
                   + sbgLogGpsPos_filename, LEVEL_OK );
        *(this->sbgLogGpsPos_Strm) << "timeStamp, status, timeOfWeek, latitude, longitude, altitude, undulation, "
                                      "latitudeAccuracy, longitudeAccuracy, altitudeAccuracy, numSvUsed, baseStationId, differentialAge," << endl;
    }
    else
        newMessage("[ ERR ]SbgReadFile::readFile *** impossible to create file: "
                   + sbgLogGpsPos_filename, LEVEL_ERR );

    //*************
    // SbgLogGpsVel
    sbgLogGpsVel_File->setFileName ( sbgLogGpsVel_filename );
    if (sbgLogGpsVel_File->open(QIODevice::WriteOnly))
    {
        sbgLogGpsVel_Strm->setDevice( sbgLogGpsVel_File );
        newMessage("[ OK  ] SbgReadFile::readFile *** file created successfully: "
                   + sbgLogGpsVel_filename, LEVEL_OK );
        *(this->sbgLogGpsVel_Strm) << "timeStamp status timeOfWeek velocityNorth velocityEast velocityDow "
                                      "velocityAccNorth velocityAccEast velocityAccDown course courseAcc" << endl;
    }
    else
        newMessage("[ ERR ]SbgReadFile::readFile *** impossible to create file: "
                   + sbgLogGpsVel_filename, LEVEL_ERR );

    //*************
    // SbgLogGpsHdt
    sbgLogGpsHdt_File->setFileName ( sbgLogGpsHdt_filename );
    if (sbgLogGpsHdt_File->open(QIODevice::WriteOnly))
    {
        sbgLogGpsHdt_Strm->setDevice( sbgLogGpsHdt_File );
        newMessage("[ OK  ] SbgReadFile::readFile *** file created successfully: "
                   + sbgLogGpsHdt_filename, LEVEL_OK );
        *(this->sbgLogGpsHdt_Strm) << "timeStamp status timeOfWeek heading headingAcc pitch pitchAcc" << endl;
    }
    else
        newMessage("[ ERR ]SbgReadFile::readFile *** impossible to create file: "
                   + sbgLogGpsHdt_filename, LEVEL_ERR );

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
    sbgLogUtcData = 0;
    // GPS
    sbgLogGpsPos = 0;
    sbgLogGpsVel = 0;
    sbgLogGpsHdt = 0;
    cursorPosition = 0;
    delta = 0;
}

void SbgReadFile::updateStat()
{
    ui->label_sbgEComLogStatus->setText( QString::number( sbgEComLogStatus ) );
    ui->label_sbgEComLogEkfEuler->setText( QString::number( sbgEComLogEkfEuler ) );
    ui->label_sbgEComLogEkfNav->setText( QString::number( sbgEComLogEkfNav ) );
    ui->label_sbgEComLogEventB->setText( QString::number( sbgEComLogEventB ) );
    ui->label_sbgLogUtcData->setText( QString::number( sbgLogUtcData ) );
    ui->label_position->setText( QString::number( cursorPosition ) );
    ui->label_delta->setText( QString::number( delta ) );
    // GPS
    ui->label_sbgLogGpsPos->setText(QString::number( sbgLogGpsPos ));
    ui->label_sbgLogGpsVel->setText(QString::number( sbgLogGpsVel ));
    ui->label_sbgLogGpsHdt->setText(QString::number( sbgLogGpsHdt ));
}

void SbgReadFile::updateStoreGpsPos( bool status )
{
    storeGpsPos = status;
}
