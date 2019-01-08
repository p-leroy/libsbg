#ifndef SBGECOMLOGDATA_H
#define SBGECOMLOGDATA_H

#include <QWidget>

#include "binaryLogs/sbgEComBinaryLogs.h"

namespace Ui {
class SbgEcomLogData;
}

class SbgEcomLogData : public QWidget
{
    Q_OBJECT

public:
    explicit SbgEcomLogData(QWidget *parent = 0);
    ~SbgEcomLogData();

private:
    Ui::SbgEcomLogData *ui;

public slots:
    void updateSbgEcomLogEkfEuler( QByteArray data );
    void updateSbgEcomLogEkfNav( QByteArray data );
    void updateSbgEcomLogEventB( QByteArray data );

};

#endif // SBGECOMLOGDATA_H
