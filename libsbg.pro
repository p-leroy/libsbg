#-------------------------------------------------
#
# Project created by QtCreator 2016-05-27T15:24:03
#
#-------------------------------------------------

message($$QMAKESPEC)

QT       += widgets
CONFIG += debug
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
# INCLUDEPATH += ../common_SWALIS

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
        "/opt/InertialSDK_Unix_v5.0/Software Development/sbgECom/src" \
        "/opt/InertialSDK_Unix_v5.0/Software Development/sbgECom/common"
    LIBS += \
        "/opt/InertialSDK_Unix_v5.0/Software Development/sbgECom/bin/libsbgECom.a"
## INSTALL
    target.path = $$[QT_INSTALL_LIBS]
    isEmpty(target.path) {
        error(can\'t get QT_INSTALL_LIBS)
    }
    headers.path = $$[QT_INSTALL_HEADERS]/common_ple/sbg
    headers.files = $$PWD/*.h
    INSTALLS += headers target
}

############
############
## WIN32-G++

win32-g++{
    DEFINES += _WINSOCKAPI_
    LIBS += -lws2_32 -lwsock32

    SBGPATH = "C:\Program Files\SBG Systems\Inertial SDK\Software Development"
    SOURCES += \
        $$SBGPATH/sbgECom/src/*.c \
        $$SBGPATH/sbgECom/src/binaryLogs/*.c \
        $$SBGPATH/sbgECom/src/commands/*.c \
        $$SBGPATH/sbgECom/src/commands/transfer/*.c \
        $$SBGPATH/sbgECom/src/protocol/*.c \
        $$SBGPATH/sbgECom/common/crc/*.c \
        $$SBGPATH/sbgECom/common/interfaces/sbgInterface.c \
        $$SBGPATH/sbgECom/common/interfaces/sbgInterfaceFile.c \
        $$SBGPATH/sbgECom/common/interfaces/sbgInterfaceSerialWin.c \
        $$SBGPATH/sbgECom/common/interfaces/sbgInterfaceUdp.c \
        $$SBGPATH/sbgECom/common/network/*.c \
        $$SBGPATH/sbgECom/common/platform/*.c \
        $$SBGPATH/sbgECom/common/splitbuffer/*.c \
        $$SBGPATH/sbgECom/common/streamBuffer/*.c \
        $$SBGPATH/sbgECom/common/swap/*.c \
        $$SBGPATH/sbgECom/common/version/*.c
    HEADERS += \
        $$SBGPATH/sbgECom/src/*.h \
        $$SBGPATH/sbgECom/src/binaryLogs/*.h \
        $$SBGPATH/sbgECom/src/commands/*.h \
        $$SBGPATH/sbgECom/src/commands/transfer/*.h \
        $$SBGPATH/sbgECom/src/protocol/*.h \
        $$SBGPATH/sbgECom/common/*.h \
        $$SBGPATH/sbgECom/common/crc/*.h \
        $$SBGPATH/sbgECom/common/interfaces/*.h \
        $$SBGPATH/sbgECom/common/network/*.h \
        $$SBGPATH/sbgECom/common/platform/*.h \
        $$SBGPATH/sbgECom/common/splitbuffer/*.h \
        $$SBGPATH/sbgECom/common/streamBuffer/*.h \
        $$SBGPATH/sbgECom/common/swap/*.h \
        $$SBGPATH/sbgECom/common/version/*.h
    INCLUDEPATH += \
        $$SBGPATH/sbgECom/src \
        $$SBGPATH/sbgECom/common
## INSTALL
    target.path = ../lib/bin
    INSTALLS += target
}
