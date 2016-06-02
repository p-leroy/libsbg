#include "sbgecomlogstatus.h"
#include "ui_sbgecomlogstatus.h"

QString solutionModeStr[5] = {
    "Uninitialized",
    "Vertical Gyro",
    "AHRS",
    "Velocity",
    "Position"
};

QString solutionModeToolTips[5] = {
    "The Kalman filter is not initialized and the returned data are all invalid.",
    "The Kalman filter only rely on a vertical reference to compute roll and pitch angles. Heading and navigation data drift freely.",
    "A heading reference is available, the Kalman filter provides full orientation but navigation data drift freely.",
    "The Kalman filter computes orientation and velocity. Position is freely integrated from velocity estimation",
    "Nominal mode, the Kalman filter computes all parameters (attitude, velocity, position). Absolute position is provided."
};

QString solutionStatusToolTips[14] = {
    "0", "1", "2", "3",
    "Set to 1 if Attitude data is reliable (Roll/Pitch error < 0.5°).", // 4
    "Set to 1 if Heading data is reliable (Heading error < 1°).",       // 5
    "Set to 1 if Velocity data is reliable (Velocity error < 1. 5m/s)", // 6
    "Set to 1 if Position data is reliable (Position error < 10 m).",   // 7
    "Set to 1 if vertical reference is used in solution (data used and valid since 3s).",
    "9",
    "Set to 1 if GPS velocity is used in solution (data used and valid since 3s).",
    "Set to 1 if GPS position is used in solution (data used and valid since 3s).",
    "Set to 1 if GPS course is used in solution (data used and valid since 3s).",
    "Set to 1 if GPS true heading is used in solution (data used and valid since 3s)."
};

QString clockStatusStr[4] = {
    "error",
    "free running",
    "steering",
    "valid"
};

QString clockStatusToolTips[4] = {
    "An error has occured on the clock estimation.",
    "The clock is only based on the internal crystal.",
    "A PPS has been detected and the clock is converging to it.",
    "The clock has converged to the PPS and is within 500ns."
};

QString uTCStatusStr[3] = {
    "invalid",
    "no leap sec.",
    "valid"
};

QString uTCStatusToolTips[3] = {
    "The UTC time is not known, we are just propagating the UTC time internally.",
    "We have received valid UTC time information but we don't' have the leap seconds information.",
    "We have received valid UTC time data with valid leap seconds."
};

QString gPSVelocityStatusStr[4] = {
    "Solution computed",
    "Insufficient obs.",
    "Internal error",
    "Velocity limit"
};

QString gPSVelocityTypeStr[4] = {
    "No solution",
    "Unknown type",
    "Doppler",
    "Differential"
};

QString gPSVelocityTypeToolTips[4] = {
    "No valid velocity solution available.",
    "An unknown solution type has been computed.",
    "A Doppler velocity has been computed.",
    "A velocity has been computed between two positions."
};

QString gPSPositionStatusStr[4] = {
    "Solution computed",
    "Insufficient obs.",
    "Internal error",
    "Height limit"
};

QString gPSPositionTypeStr[11] = {
    "No solution",
    "Unknown type",
    "Single point",
    "Pseudorange differential (DGPS)",
    "SBAS",
    "OMNISTAR",
    "RTK float",
    "RTK int",
    "PPP float",
    "PPP int",
    "Fixed location"
};

QString gPSPositionTypeToolTips[11] = {
    "No valid solution available.",
    "An unknown solution type has been computed.",
    "Single point solution position.",
    "Standard Pseudorange Differential Solution (DGPS).",
    "SBAS satellite used for differential corrections.",
    "Omnistar VBS Position (L1 sub-meter).",
    "Floating RTK ambiguity solution (20 cms RTK).",
    "Integer RTK ambiguity solution (2 cms RTK).",
    "Precise Point Positioning with float ambiguities.",
    "Precise Point Positioning with fixed ambiguities.",
    "Fixed location solution position."
};

QString gPSHeadingStatusStr[4] = {
    "Solution computed",
    "Insufficient obs.",
    "Internal error",
    "Height limit"
};

SbgEcomLogStatus::SbgEcomLogStatus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SbgEcomLogStatus)
{
    ui->setupUi(this);

    this->ui->gPS1GPSState->setToolTip( "GPS used in the solution (L1 / L2 / L5)" );
    this->ui->gPS1GLONASSState->setToolTip( "GLONASS used in the solution (L1 / L2)" );
}

SbgEcomLogStatus::~SbgEcomLogStatus()
{
    delete ui;
}

void SbgEcomLogStatus::update_GENERAL_STATUS( unsigned int generalStatus )
{
    setState( this->ui->mainPowerOKLabel_state,     (generalStatus >> 0) & 0x01 );
    setState( this->ui->iMUPowerOKLabel_state,      (generalStatus >> 1) & 0x01 );
    setState( this->ui->gPSPowerOKLabel_state,      (generalStatus >> 2) & 0x01 );
    setState( this->ui->settingsOKLabel_state,      (generalStatus >> 3) & 0x01 );
    setState( this->ui->temperatureOKLabel_state,   (generalStatus >> 4) & 0x01 );
    setState( this->ui->dataloggerOKLabel_state,    (generalStatus >> 5) & 0x01 );
}

void SbgEcomLogStatus::update_CLOCK_STATUS( unsigned int clockStatus )
{
    setState( this->ui->clockStableInputSate, (clockStatus >> 0) & 0x01 );
    this->ui->clockStatusLabel->setText( clockStatusStr[        (clockStatus >> 1) & 0x07 ] );    // [0111]
    this->ui->clockStatusLabel->setToolTip( clockStatusToolTips[(clockStatus >> 1) & 0x07 ] );    // [0111]

    setState( this->ui->uTCSyncState, (clockStatus >> 5) & 0x01 );
    this->ui->uTCStatusLabel->setText( uTCStatusStr[        (clockStatus >> 6) & 0x07 ] );    // [0111]
    this->ui->uTCStatusLabel->setToolTip( uTCStatusToolTips[(clockStatus >> 6) & 0x07 ] );    // [0111]
}

void SbgEcomLogStatus::update_AIDING_STATUS( unsigned int aidingStatus )
{
    setState( this->ui->aidingGPS1PositionState,    (aidingStatus >> 0) & 0x01 );
    setState( this->ui->aidingGPS1VelocityState,    (aidingStatus >> 1) & 0x01 );
    setState( this->ui->aidingGPS1HeadingState,     (aidingStatus >> 2) & 0x01 );
    setState( this->ui->aidingGPS1UTCState,         (aidingStatus >> 3) & 0x01 );
}

void SbgEcomLogStatus::update_SOLUTION_STATUS( unsigned int solutionStatus )
{
    this->ui->solutionModeLabel->setText(       solutionModeStr[        (solutionStatus >> 0) & 0x0f ] );    // [1111]
    this->ui->solutionModeLabel->setToolTip(    solutionModeToolTips[   (solutionStatus >> 0) & 0x0f ] );

    setState( this->ui->attitudeValidState,     (solutionStatus >> 4)  & 0x01 );
    this->ui->attitudeValidState->setToolTip(   solutionStatusToolTips[ 4 ] );
    setState( this->ui->headingValidState,      (solutionStatus >> 5)  & 0x01 );
    this->ui->headingValidState->setToolTip(    solutionStatusToolTips[ 5 ] );
    setState( this->ui->velocityValidState,     (solutionStatus >> 6)  & 0x01 );
    this->ui->velocityValidState->setToolTip(   solutionStatusToolTips[ 6 ] );
    setState( this->ui->positionValidState,     (solutionStatus >> 7)  & 0x01 );
    this->ui->positionValidState->setToolTip(   solutionStatusToolTips[ 7 ] );

    setState( this->ui->gPS1VelocityState,      (solutionStatus >> 10) & 0x01 );
    this->ui->gPS1VelocityState->setToolTip(    solutionStatusToolTips[ 10 ] );
    setState( this->ui->gPS1PositionState,      (solutionStatus >> 11) & 0x01 );
    this->ui->gPS1PositionState->setToolTip(    solutionStatusToolTips[ 11 ] );
    setState( this->ui->gPS1CourseState,        (solutionStatus >> 12) & 0x01 );
    this->ui->gPS1CourseState->setToolTip(      solutionStatusToolTips[ 12 ] );
    setState( this->ui->gPS1HDTState,           (solutionStatus >> 13) & 0x01 );
    this->ui->gPS1HDTState->setToolTip(         solutionStatusToolTips[ 13 ] );
}

void SbgEcomLogStatus::updateSbgEcomLogGps1Vel(QByteArray data)
{
//    typedef struct _SbgLogGpsVel
//    {
//        uint32			timeStamp;				/*!< Time in us since the sensor power up. */
//        uint32			status;					/*!< GPS velocity status, type and bitmask. */
//        uint32			timeOfWeek;				/*!< GPS time of week in ms. */
//        float			velocity[3];			/*!< GPS North, East, Down velocity in m.s^-1. */
//        float			velocityAcc[3];			/*!< GPS North, East, Down velocity 1 sigma accuracy in m.s^-1. */
//        float			course;					/*!< Track ground course in degrees. */
//        float			courseAcc;				/*!< Course accuracy in degrees. */
//    } SbgLogGpsVel;

    SbgLogGpsVel *sbgLog;

    sbgLog = (SbgLogGpsVel*) data.data();

    this->ui->gPS1VelocityStatusLabel->setText( gPSVelocityStatusStr[   (sbgLog->status >> 0) & 0x3f ] ); // [0011 1111]
    this->ui->gPS1VelocityTypeLabel->setText(   gPSVelocityTypeStr[     (sbgLog->status >> 6) & 0x3f ] ); // [0011 1111]
    this->ui->gPS1VelocityTypeLabel->setToolTip(gPSVelocityTypeToolTips[(sbgLog->status >> 6) & 0x3f ] ); // [0011 1111]
}

void SbgEcomLogStatus::updateSbgEcomLogGps1Pos(QByteArray data)
{
//    typedef struct _SbgLogGpsPos
//    {
//        uint32			timeStamp;				/*!< Time in us since the sensor power up. */
//        uint32			status;					/*!< GPS position status, type and bitmask. */
//        uint32			timeOfWeek;				/*!< GPS time of week in ms. */
//        double			latitude;				/*!< Latitude in degrees, positive north. */
//        double			longitude;				/*!< Longitude in degrees, positive east. */
//        double			altitude;				/*!< Altitude above Mean Sea Level in meters. */
//        float			undulation;				/*!< Altitude difference between the geoid and the Ellipsoid in meters (Height above Ellipsoid = altitude + undulation). */
//        float			latitudeAccuracy;		/*!< 1 sigma latitude accuracy in meters. */
//        float			longitudeAccuracy;		/*!< 1 sigma longitude accuracy in meters. */
//        float			altitudeAccuracy;		/*!< 1 sigma altitude accuracy in meters. */
//    } SbgLogGpsPos;

    SbgLogGpsPos *sbgLog;
    QString gps;
    QString glo;
    bool gpsL1Used;
    bool gpsL2Used;
    bool gpsL5Used;
    bool gloL1Used;
    bool gloL2Used;

    sbgLog = (SbgLogGpsPos*) data.data();

    this->ui->gPS1PositionStatusLabel->setText( gPSPositionStatusStr[   (sbgLog->status >> 0) & 0x3f ] ); // [0011 1111]
    this->ui->gPS1PositionTypeLabel->setText(   gPSPositionTypeStr[     (sbgLog->status >> 6) & 0x3f ] ); // [0011 1111]
    this->ui->gPS1PositionTypeLabel->setToolTip(gPSPositionTypeToolTips[(sbgLog->status >> 6) & 0x3f ] ); // [0011 1111]

    gpsL1Used = (sbgLog->status >> 12) & 0x01;
    gpsL2Used = (sbgLog->status >> 13) & 0x01;
    gpsL5Used = (sbgLog->status >> 14) & 0x01;

    if (gpsL1Used)
        gps = "L1 ";
    if (gpsL2Used)
        gps = gps + "L2";
    if (gpsL5Used)
        gps = gps + "L5 ";
    this->ui->gPS1GPSState->setText( gps );

    gloL1Used = (sbgLog->status >> 15) & 0x01;
    gloL2Used = (sbgLog->status >> 16) & 0x01;

    if (gloL1Used)
        glo = "L1 ";
    if (gloL2Used)
        glo = glo + "L2";
    this->ui->gPS1GLONASSState->setText( glo );
}

void SbgEcomLogStatus::updateSbgEcomLogGps1Hdt(QByteArray data)
{
//    typedef struct _SbgLogGpsHdt
//    {
//        uint32			timeStamp;				/*!< Time in us since the sensor power up. */
//        uint16			status;					/*!< GPS HDT status, type and bitmask. */
//        uint32			timeOfWeek;				/*!< GPS time of week in ms. */
//        float			heading;				/*!< GPS true heading in degrees. */
//        float			headingAccuracy;		/*!< 1 sigma GPS true heading accuracy in degrees. */
//        float			pitch;					/*!< GPS pitch angle measured from the master to the rover in degrees. */
//        float			pitchAccuracy;			/*!< 1 signa GPS pitch angle accuarcy in degrees. */
//    } SbgLogGpsHdt;

    SbgLogGpsHdt *sbgLog;

    sbgLog = (SbgLogGpsHdt*) data.data();

    this->ui->gPS1HeadingStatusLabel->setText( gPSHeadingStatusStr[ (sbgLog->status >> 0) & 0x3f ] ); // [0011 1111]
}


void SbgEcomLogStatus::setState(QLabel *label, bool state)
{
    QString ok = QChar(0x2714);
    QString nok = "x";

    if (state == true)
    {
        label->setText( ok );
        label->setStyleSheet("color: green");
    }
    else
    {
        label->setText( nok );
        label->setStyleSheet("color: red");
    }
}

void SbgEcomLogStatus::updateSbgEcomLogStatus( QByteArray data )
{
    SbgLogStatusData *sbgLog;

    static uint16 generalStatus = 0;
    static uint16 clockStatus = 0;
    static uint32 aidingStatus = 0;
    static uint32 solutionStatus = 0;

    sbgLog = (SbgLogStatusData*) data.data();

    this->ui->timeStampLabel->setText( QString::number(sbgLog->timeStamp / 1000000., 'f', 6) );

    if (generalStatus != sbgLog->generalStatus)
        update_GENERAL_STATUS( sbgLog->generalStatus );
    if (clockStatus != sbgLog->clockStatus)
        update_CLOCK_STATUS( sbgLog->clockStatus );
    if (aidingStatus != sbgLog->aidingStatus)
        update_AIDING_STATUS( sbgLog->aidingStatus );
    if (solutionStatus != sbgLog->solutionStatus)
        update_SOLUTION_STATUS( sbgLog->solutionStatus );

    generalStatus = sbgLog->generalStatus;
    clockStatus = sbgLog->clockStatus;
    aidingStatus = sbgLog->aidingStatus;
    solutionStatus = sbgLog->solutionStatus;
}
