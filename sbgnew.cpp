/*!
 *	\file		ekinoxMinimal.c
 *      \author		SBG Systems (Alexis GUINAMARD)
 *	\date		28/03/2013
 *
 *	\brief		C example that simply opens an Ekinox interface and reads Euler Angles from it.
 *
 *	This small example demonstrates how to initialize the sbgECom library
 *	to read data from an Ekinox using callbacks.
 *
 *	\section CodeCopyright Copyright Notice
 *	Copyright (C) 2007-2013, SBG Systems SAS. All rights reserved.
 *
 *	This source code is intended for use only by SBG Systems SAS and
 *	those that have explicit written permission to use it from
 *	SBG Systems SAS.
 *
 *	THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 *	PARTICULAR PURPOSE.
 */

#include "sbgnew.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <QCoreApplication>

SbgNew* SbgNew::sbgNew = nullptr;
unsigned char SbgNew::sbgIp3 = 0;
unsigned char SbgNew::sbgIp2 = 0;
unsigned char SbgNew::sbgIp1 = 0;
unsigned char SbgNew::sbgIp0 = 0;
bool SbgNew::continueExecution = true;
uint32 SbgNew::remotePort;
uint32 SbgNew::localPort;

SbgNew::SbgNew(QObject *parent) : QObject(parent)
{
    SbgNew::sbgNew = this;
}

//----------------------------------------------------------------------//
//  Call backs                                                          //
//----------------------------------------------------------------------//

/*!
 *	Callback definition called each time a new log is received.
 *	\param[in]	pHandle									Valid handle on the sbgECom instance that has called this callback.
 *	\param[in]	logCmd									Contains the binary received log command id.
 *	\param[in]	pLogData								Contains the received log data as an union.
 *	\param[in]	pUserArg								Optional user supplied argument.
 *	\return												SBG_NO_ERROR if the received log has been used successfully.
 */

SbgErrorCode SbgNew::receiveLogFunc(SbgEComHandle *pHandle, SbgEComClass msgClass, SbgEComMsgId msg, const SbgBinaryLogData *pLogData, void *pUserArg)
{
    QByteArray array;

    if (msgClass != SBG_ECOM_CLASS_LOG_ECOM_0)
    {
        emit sbgNew->newMessage( "msgClass is not SBG_ECOM_CLASS_LOG_ECOM_0, unexpected message", LEVEL_ERR );
        return SBG_ERROR;
    }

    //
    // Handle separately each received data according to the log ID
    //

    switch (msg)
    {

    case SBG_ECOM_LOG_STATUS:
        array = QByteArray( reinterpret_cast<const char*>(pLogData), sizeof(SbgLogStatusData) );
        emit sbgNew->newSbgEcomLogStatus( array );
        break;

    case SBG_ECOM_LOG_EKF_EULER:
        array = QByteArray( reinterpret_cast<const char*>(pLogData), sizeof(SbgLogEkfEulerData) );
        emit sbgNew->newSbgEcomLogEkfEuler( array );
        break;

    case SBG_ECOM_LOG_EKF_QUAT:
        break;

    case SBG_ECOM_LOG_EKF_NAV:
        array = QByteArray( reinterpret_cast<const char*>(pLogData), sizeof(SbgLogEkfNavData) );
        emit sbgNew->newSbgEcomLogEkfNav( array );
        break;

    case SBG_ECOM_LOG_EVENT_B:
        array = QByteArray( reinterpret_cast<const char*>(pLogData), sizeof(SbgLogEvent) );
        emit sbgNew->newSbgEcomLogEventB( array );
        break;

    case SBG_ECOM_LOG_GPS1_VEL:
        array = QByteArray( reinterpret_cast<const char*>(pLogData), sizeof(SbgLogGpsVel) );
        emit sbgNew->newSbgEcomLogGPS1Vel( array );
        break;

    case SBG_ECOM_LOG_GPS1_POS:
        array = QByteArray( reinterpret_cast<const char*>(pLogData), sizeof(SbgLogGpsPos) );
        emit sbgNew->newSbgEcomLogGPS1Pos( array );
        break;

    case SBG_ECOM_LOG_GPS1_HDT:
        array = QByteArray( reinterpret_cast<const char*>(pLogData), sizeof(SbgLogGpsHdt) );
        emit sbgNew->newSbgEcomLogGPS1Hdt( array );
        break;

    default:
        break;
    }

    return SBG_NO_ERROR;
}

void SbgNew::setSbgNew(void *ptr)
{
    sbgNew = (SbgNew*) ptr;
}

/*!
 *	Main entry point.
 *	\param[in]	argc		Number of input arguments.
 *	\param[in]	argv		Input arguments as an array of strings.
 *	\return					0 if no error and -1 in case of error.
 */
int SbgNew::sbgPollingLoop()
{
    SbgEComHandle comHandle;
    SbgErrorCode errorCode;
    SbgInterface sbgInterface;
    int32 retValue = 0;

    emit sbgNew->newMessage("sbgPollingLoop *** trying to connect to: "
                              + QString::number(sbgIp3) + "."
                              + QString::number(sbgIp2) + "."
                              + QString::number(sbgIp1) + "."
                              + QString::number(sbgIp0));

    //
    // Create an iata.eulenterface:
    // We can choose either UDP or serial for real time operation, or file for previously logged data parsing
    // Note interface closing is also differentiated !
    //
    errorCode = sbgInterfaceUdpCreate(&sbgInterface, sbgIpAddr(sbgIp3, sbgIp2, sbgIp1, sbgIp0),
                                      remotePort, localPort);		// Example to read the data from an UDP interface
    //errorCode = sbgInterfaceFileOpen(&sbgInterface, "log_16h00.bin");					// Example to read the data from a text file
    //errorCode = sbgInterfaceSerialCreate(&sbgInterface, "/dev/ttyUSB0", 115200);				// Example for Unix using a FTDI Usb2Uart converter
    //errorCode = sbgInterfaceSerialCreate(&sbgInterface, "COM4", 115200);					// Example for Windows serial communication

    //
    // Test that the interface has been created
    //
    if (errorCode == SBG_NO_ERROR)
    {
        emit sbgNew->newMessage("OK *** sbgInterfaceUdpCreate");
        //
        // Create the sbgECom library and associate it with the created interfaces
        //
        errorCode = sbgEComInit(&comHandle, &sbgInterface);

        //
        // Test that the sbgECom has been initialized
        //
        if (errorCode == SBG_NO_ERROR)
        {
            //
            // Define callbacks for received data
            //
            sbgEComSetReceiveLogCallback(&comHandle, receiveLogFunc, nullptr);

            //
            // Loop until the user exits
            //
            emit sbgNew->isReady( true );
            while (continueExecution)
            {
                //QCoreApplication::processEvents();
                //
                // Try to read a frame
                //
                errorCode = sbgEComHandle(&comHandle);

                //
                // Test if we have to release some CPU (no frame received)
                //
                if (errorCode == SBG_NOT_READY)
                {
                    //
                    // Release CPU
                    //
                    sbgSleep(100);
                }
                else
                {
                    fprintf(stderr, "Error\n");
                }
            }
            emit sbgNew->newMessage("sbgPollingLoop *** end of infinite loop");

            //
            // Close the sbgEcom library
            //
            sbgEComClose(&comHandle);
            emit sbgNew->newMessage("sbgPollingLoop *** connection closed");
        }
        else
        {
            //
            // Unable to initialize the sbgECom
            //
            fprintf(stderr, "ekinoxMinimal: Unable to initialize the sbgECom library.\n");
            retValue = -1;
        }

        //
        // Close the interface
        //
        sbgInterfaceUdpDestroy(&sbgInterface);
        emit sbgNew->newMessage("sbgPollingLoop *** UDP interface destroyed");
        //sbgInterfaceSerialDestroy(&sbgInterface);
        //sbgInterfaceFileClose(&sbgInterface);

    }
    else
    {
        //
        // Unable to create the interface
        //
        emit sbgNew->newMessage("ERR *** sbgInterfaceUdpCreate *** unable to create the interface");
        fprintf(stderr, "ekinoxMinimal: Unable to create the interface.\n");
        retValue = -1;
    }

    //
    // Returns -1 if we have an error
    //

    continueExecution = true;

    emit sbgNew->isReady( false );
    emit sbgNew->newMessage("sbgPollingLoop *** end of sbgPollingLoop");
    QApplication::processEvents();
    emit sbgNew->finished();

    return retValue;
}

void SbgNew::updateSbgIp(unsigned char ip3, unsigned char ip2, unsigned char ip1, unsigned char ip0)
{
    sbgIp3 = ip3;
    sbgIp2 = ip2;
    sbgIp1 = ip1;
    sbgIp0 = ip0;
}

void SbgNew::setRemotePort(uint32 port)
{
    remotePort = port;
}

void SbgNew::setLocalPort(uint32 port)
{
    localPort = port;
}

void SbgNew::initSbg()
{
    SbgNew::sbgPollingLoop();
}

void SbgNew::closeSbgConnection()
{
    SbgNew::continueExecution = false;
}
