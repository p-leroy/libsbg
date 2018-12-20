#-------------------------------------------------
#
# Project created by QtCreator 2016-05-27T15:24:03
#
#-------------------------------------------------

QT       += widgets

TARGET = libsbg
TEMPLATE = lib

DEFINES += LIBSBG_LIBRARY
DEFINES += _WINSOCKAPI_

SOURCES += libsbg.cpp \
    qwsbg.cpp \
    sbgconnection.cpp \
    sbgnew.cpp \
    sbgecomlogstatus.cpp \
    sbgecomlogdata.cpp \
    sbgreadfile.cpp

HEADERS += libsbg.h\
    qwsbg.h \
    libsbg_global.h \
    sbgconnection.h \
    sbgnew.h \
    sbgecomlogstatus.h \
    sbgecomlogdata.h \
    sbgreadfile.h


INCLUDEPATH += ../common_SWALIS

FORMS += \
    qwsbg.ui \
    sbgconnection.ui \
    sbgecomlogstatus.ui \
    sbgecomlogdata.ui \
    sbgreadfile.ui



########
########
## WIN32
INCLUDEPATH += \
        "../Centrale_inertielle/Ekinox/Software_Development/sbgECom/src" \
        "../Centrale_inertielle/Ekinox/Software_Development/sbgECom/common"\
        //"C:\Users\PC\Documents\SWALIS_portable\common_SWALIS"
LIBS += \
        "..\Centrale_inertielle/Ekinox/Software_Development/sbgECom/sbgECom.lib"
LIBS += -lws2_32 -lwsock32


