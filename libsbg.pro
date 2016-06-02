#-------------------------------------------------
#
# Project created by QtCreator 2016-05-27T15:24:03
#
#-------------------------------------------------

QT       += widgets

TARGET = libsbg
TEMPLATE = lib

DEFINES += LIBSBG_LIBRARY

SOURCES += libsbg.cpp \
    qwsbg.cpp \
    sbgconnection.cpp \
    sbgnew.cpp \
    sbgecomlogstatus.cpp \
    sbgecomlogdata.cpp

HEADERS += libsbg.h\
    qwsbg.h \
    libsbg_global.h \
    sbgconnection.h \
    sbgnew.h \
    sbgecomlogstatus.h \
    sbgecomlogdata.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    qwsbg.ui \
    sbgconnection.ui \
    sbgecomlogstatus.ui \
    sbgecomlogdata.ui

#######
#######
## UNIX
#######
#######

unix{
    INCLUDEPATH += \
        /opt/SBG/Ekinox/Software_Development/sbgECom/src
    LIBS += \
        /opt/SBG/Ekinox/Software_Development/sbgECom/libSbgECom.a
}

########
########
## WIN32
########
########

win32{
INCLUDEPATH += \
    "C:/Documents and Settings/Administrateur/Mes documents/PAUL/Centrale_inertielle/Ekinox/Software Development/sbgECom/src"
LIBS += \
    "C:/Documents and Settings/Administrateur/Mes documents/PAUL/Centrale_inertielle/Ekinox/Software Development/sbgECom/sbgECom.lib"
}
