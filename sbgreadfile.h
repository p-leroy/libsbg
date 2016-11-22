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
    explicit SbgReadFile(QWidget *parent = 0);
    ~SbgReadFile();
    void readSettings();
    void writeSettings();

    static void storeSbgEComLogEkfEuler( SbgLogEkfEulerData *log );
    static void storeSbgEComLogEkfNav( SbgLogEkfNavData *log );
    static void storeSbgEComLogEventB( SbgLogEvent *log );
    static SbgErrorCode onLogReceived(SbgEComHandle *pHandle, SbgEComCmdId logCmd, const SbgBinaryLogData *pLogData, void *pUserArg);
    static int sbgPollingLoop();

public slots:
    int chooseFile(void);
    void readFile(void);
    void displayMessage( QString str, unsigned char level );
    void initStat();
    void updateStat();

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

    static SbgReadFile *sbgReadFile;
    static bool continueExecution;
    static SbgInterface sbgInterface;

    static unsigned int sbgEComLogStatus;
    static unsigned int sbgEComLogEkfEuler;
    static unsigned int sbgEComLogEkfQuat;
    static unsigned int sbgEComLogEkfNav;
    static unsigned int sbgEComLogEventB;
    static unsigned int sbgEComLogGPS1Vel;
    static unsigned int sbgEComLogGPS1Pos;
    static unsigned int sbgEComLogGPS1Hdt;
    static uint32 cursorPosition;
    static int delta;
    static int fileSize;
};

#endif // SBGREADFILE_H
