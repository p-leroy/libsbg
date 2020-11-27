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
    SBGPATH = "/opt/InertialSDK_Unix_v5.0/Software Development"
    INCLUDEPATH += \
        $$[QT_INSTALL_HEADERS]/common_ple/message_ple \
        $$SBGPATH/sbgECom/src \
        $$SBGPATH/sbgECom/common
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

    INCLUDEPATH += ../../lib/include/common_ple/message_ple

    SBGPATH = "C:/Program Files/SBG Systems/Inertial SDK/Software Development"
    SOURCES += \
        $$files($$SBGPATH/sbgECom/src/*.c) \
        $$files($$SBGPATH/sbgECom/src/binaryLogs/*.c) \
        $$files($$SBGPATH/sbgECom/src/commands/*.c) \
        $$files($$SBGPATH/sbgECom/src/commands/transfer/*.c) \
        $$files($$SBGPATH/sbgECom/src/protocol/*.c) \
        $$files($$SBGPATH/sbgECom/common/crc/*.c) \
        $$files($$SBGPATH/sbgECom/common/interfaces/sbgInterface.c) \
        $$files($$SBGPATH/sbgECom/common/interfaces/sbgInterfaceFile.c) \
        $$files($$SBGPATH/sbgECom/common/interfaces/sbgInterfaceSerialWin.c) \
        $$files($$SBGPATH/sbgECom/common/interfaces/sbgInterfaceUdp.c) \
        $$files($$SBGPATH/sbgECom/common/network/*.c) \
        $$files($$SBGPATH/sbgECom/common/platform/*.c) \
        $$files($$SBGPATH/sbgECom/common/splitbuffer/*.c) \
        $$files($$SBGPATH/sbgECom/common/streamBuffer/*.c) \
        $$files($$SBGPATH/sbgECom/common/swap/*.c) \
        $$files($$SBGPATH/sbgECom/common/version/*.c)
    HEADERS += \
        $$files($$SBGPATH/sbgECom/src/*.h) \
        $$files($$SBGPATH/sbgECom/src/binaryLogs/*.h) \
        $$files($$SBGPATH/sbgECom/src/commands/*.)h \
        $$files($$SBGPATH/sbgECom/src/commands/transfer/*.h) \
        $$files($$SBGPATH/sbgECom/src/protocol/*.h) \
        $$files($$SBGPATH/sbgECom/common/*.h) \
        $$files($$SBGPATH/sbgECom/common/crc/*.h) \
        $$files($$SBGPATH/sbgECom/common/interfaces/*.h) \
        $$files($$SBGPATH/sbgECom/common/network/*.h) \
        $$files($$SBGPATH/sbgECom/common/splitbuffer/*.h) \
        $$files($$SBGPATH/sbgECom/common/streamBuffer/*.h) \
        $$files($$SBGPATH/sbgECom/common/swap/*.h) \
        $$files($$SBGPATH/sbgECom/common/version/*.h)
    INCLUDEPATH += \
        $$SBGPATH/sbgECom/src \
        $$SBGPATH/sbgECom/common

## INSTALL
    target.path = ../../lib/bin
    headers.path = ../../lib/include/common_ple/sbg
    headers.files = sbgconnection.h \
        libsbg_global.h \
        sbgnew.h \
        sbgreadfile.h

    INSTALLS += headers target
}
