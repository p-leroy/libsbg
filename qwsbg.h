#ifndef QWSBG_H
#define QWSBG_H

#include <QWidget>

#include <sbgECom.h>
#include <QSettings>

#include "libsbg_global.h"

namespace Ui {
class QWSbg;
}

class LIBSBGSHARED_EXPORT QWSbg : public QWidget
{
    Q_OBJECT

public:
    explicit QWSbg(QWidget *parent = 0);
    ~QWSbg();

private:
    Ui::QWSbg *ui;

};

#endif // QWSBG_H
