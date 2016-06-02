#ifndef SBGECOMLOGSTATUS_H
#define SBGECOMLOGSTATUS_H

#include <QWidget>
#include <QLabel>

#include "libsbg_global.h"

#include "binaryLogs/binaryLogs.h"

namespace Ui {
class SbgEcomLogStatus;
}

class LIBSBGSHARED_EXPORT SbgEcomLogStatus : public QWidget
{
    Q_OBJECT

public:
    explicit SbgEcomLogStatus(QWidget *parent = 0);
    ~SbgEcomLogStatus();
    void update_GENERAL_STATUS( unsigned int generalStatus );
    void update_CLOCK_STATUS( unsigned int clockStatus );
    void update_AIDING_STATUS( unsigned int aidingStatus );
    void update_SOLUTION_STATUS( unsigned int solutionStatus );
    void setState(QLabel *label, bool state);

private:
    Ui::SbgEcomLogStatus *ui;

public slots:
    void updateSbgEcomLogStatus( QByteArray data );
    void updateSbgEcomLogGps1Vel(QByteArray data);
    void updateSbgEcomLogGps1Pos(QByteArray data);
    void updateSbgEcomLogGps1Hdt(QByteArray data);

};

#endif // SBGECOMLOGSTATUS_H
