#ifndef SBGREADFILE_H
#define SBGREADFILE_H

#include <QWidget>

#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QSettings>
#include <QTextStream>

#include "sbgEComLib.h"

#include "libsbg_global.h"
#include "common_tctmserver.h"

#define POSAR_MC_SUCCESSFUL 0
#define POSAR_MC_FAILED 1

namespace Ui {
class SbgReadFile;
}

class LIBSBGSHARED_EXPORT SbgReadFile : public QWidget
{
    Q_OBJECT

public:
    explicit SbgReadFile(QWidget *parent = nullptr);
    ~SbgReadFile();
    void readSettings();
    void writeSettings();

    static void storeSbgEComLogEkfEuler(const SbgLogEkfEulerData *log );
    static void storeSbgEComLogEkfNav( const SbgLogEkfNavData *log );
    static void storeSbgEComLogEventB( const SbgLogEvent *log );
    static void storeSbgEComLogStatus(const SbgLogStatusData *log );
    static void storeSbgLogUtcData( const SbgLogUtcData *log );
    static void storeSbgLogGpsPos( const SbgLogGpsPos *log );
    static void storeSbgLogGpsVel( const SbgLogGpsVel *log );
    static void storeSbgLogGpsHdt( const SbgLogGpsHdt *log );
    static SbgErrorCode receiveLogFunc(SbgEComHandle *pHandle, SbgEComClass msgClass, SbgEComMsgId msg, const SbgBinaryLogData *pLogData, void *pUserArg);
    static int sbgPollingLoop();

public slots:
    int chooseFile(void);
    void readFile(void);
    void displayMessage( QString str, unsigned char level );
    void initStat();
    void updateStat();
    void updateStoreGpsPos( bool status );

signals:
    void newMessage( QString, unsigned char );

private:
    Ui::SbgReadFile *ui;

    QFileInfo fileInfo;
    QString dataStorageDirectory;
    QString dataFile;
    static QFile *sbgEComLogEkfEuler_File;
    static QTextStream *sbgEComLogEkfEuler_Strm;
    static QFile *sbgEComLogEventB_File;
    static QTextStream *sbgEComLogEventB_Strm;
    static QFile *sbgEComLogEkfNav_File;
    static QTextStream *sbgEComLogEkfNav_Strm;
    static QFile *sbgEComLogStatus_File;
    static QTextStream *sbgEComLogStatus_Strm;
    static QFile *sbgLogUtcData_File;
    static QTextStream *sbgLogUtcData_Strm;
    // GPS
    static bool storeGpsPos;
    static QFile *sbgLogGpsPos_File;
    static QTextStream *sbgLogGpsPos_Strm;
    static QFile *sbgLogGpsVel_File;
    static QTextStream *sbgLogGpsVel_Strm;
    static QFile *sbgLogGpsHdt_File;
    static QTextStream *sbgLogGpsHdt_Strm;

    static SbgReadFile *sbgReadFile;
    static bool continueExecution;
    static SbgInterface sbgInterface;

    static unsigned int sbgEComLogStatus;
    static unsigned int sbgEComLogEkfEuler;
    static unsigned int sbgEComLogEkfQuat;
    static unsigned int sbgEComLogEkfNav;
    static unsigned int sbgEComLogEventB;
    static unsigned int sbgLogUtcData;
    // GPS
    static unsigned int sbgLogGpsPos;
    static unsigned int sbgLogGpsVel;
    static unsigned int sbgLogGpsHdt;
    static size_t cursorPosition;
    static double delta;
    static size_t fileSize;
};

#endif // SBGREADFILE_H
