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

INCLUDEPATH += ../../POSAR-MC/common_posar

FORMS += \
    qwsbg.ui \
    sbgconnection.ui \
    sbgecomlogstatus.ui \
    sbgecomlogdata.ui \
    sbgreadfile.ui

#######
#######
## UNIX

unix{
    INCLUDEPATH += \
        "/opt/InertialSDK_Unix_v3.5.0/Software Development/sbgECom/src" \
        "/opt/InertialSDK_Unix_v3.5.0/Software Development/sbgECom/common"
    LIBS += \
        "/opt/InertialSDK_Unix_v3.5.0/Software Development/sbgECom/bin/libsbgECom.a"
}

########
########
## WIN32

#win32{
#INCLUDEPATH += \
#    "C:/Documents and Settings/Administrateur/Mes documents/PAUL/Centrale_inertielle/Ekinox/Software Development/sbgECom/src"
#LIBS += \
#    "C:/Documents and Settings/Administrateur/Mes documents/PAUL/Centrale_inertielle/Ekinox/Software Development/sbgECom/sbgECom.lib"
#}

win32{
INCLUDEPATH += \
    "C:/SBG/Ekinox/Software Development/sbgECom/src" \
    "C:/SBG/Ekinox/Software Development/sbgECom/common"
LIBS += \
    "C:/SBG/Ekinox/Software Development/sbgECom/projects/visual/x64/Debug/sbgEComd.lib"
}

##########
##########
## INSTALL

target.path = $$[QT_INSTALL_LIBS]
isEmpty(target.path) {
    error(can\'t get QT_INSTALL_LIBS)
}

headers.path = $$[QT_INSTALL_HEADERS]/ietr/sbg
headers.files = $$PWD/*.h

INSTALLS += headers target
