//////////////////////////////////////////////////////////////////////
//
//                U  S  B
//
// DrvM801.cpp: implementation of the CDrvM801 class.
// Implementing USB with revised Calculation Procedures
// modified on Monday Oct 8 2001
// Last Modification Date : Oct 20 2001
//
//                U  S  B
//
//////////////////////////////////////////////////////////////////////
#include "PreInclude.h"
#include <jni.h>
#include <time.h>
//#include "ff.h"
#include "DrvM801Usb.h"

#include "uBeLeInts.h"
#include "DataCollector.h"
#include "FFDefs.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>


//#include "..\LogFile.h"

//#include "resource.h"
//#if defined( defTestPulseWave )
//#include "testPulseWave\graph.h"
//#endif

//#include "..\ff.h"

extern JNIEnv* m_pEnv;
extern jobject m_jObj;
/**
** Defines for strings to read from or write to the registry.
**
** For the AGC values, the first four are read from the registry,
** the last two (last power level and max min values) are written
** to the registry.
**
*/

#ifndef defRegAverageSamples
#define defRegAverageSamples           "AverageSamples"

#define defRegPpgMultiplier            "PpgMultiplier"
#define defRegIbiMultiplier            "IbiMultiplier"

#define defRegAgcOn                    "AgcOn"
#define defRegAgcPowerIfOff            "AgcPowerLevelIfOff"
#define defRegAgcAmpLimitHigh          "AgcAmpLimitHigh"
#define defRegAgcAmpLimitLow           "AgcAmpLimitLow"
#define defRegAgcLastPowerLevel        "AgcLastPowerLevel"
#define defRegAmpMaxMinValues          "AgcAmpMaxMinValues"

#define defRegFirThreshold             "FirThreshold"
#define defRegFirHysteresis            "FirHysteresis"
#endif


CDataCollector * pDataCollector = NULL;

static float fPpgMultiplier = defPpgMultiplier;

static float fIbiMultiplier = defIbiMultiplier;


/**
**
*/

//HIDP_CAPS   Capabilities;


/**
**
*/

CDrvM801::CDrvM801()
{
	//m_hCond = PTHREAD_COND_INITIALIZER;			// 线程退出条件
	//m_hLock = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_init(&m_hCond, NULL);
	pthread_mutex_init(&m_hLock, NULL);

	m_fIsOpened = false;
	//m_hSensor = INVALID_HANDLE_VALUE;

	for ( int i = 0; i < sizeof( InputReport ); i++ )  // fle::debug
	{                                                  // fle::debug
		InputReport[ i ] = 0;                          // fle::debug
	}                                                  // fle::debug

	m_hThread = 0;

	m_Channels[0].pInternBuffer = new WORD[PPG_BUFFER_SIZE];
	m_Channels[0].nSizeOfIntBuf = PPG_BUFFER_SIZE;
	m_Channels[0].pExternBuffer = new WORD[PPG_BUFFER_SIZE];
	m_Channels[0].nSizeOfExtBuf = PPG_BUFFER_SIZE;
	//m_Channels[0].hSemaphore = ::CreateSemaphore(NULL, 1, 1, NULL);
	sem_init(&m_Channels[0].hSemaphore, 0, 1);

	m_Channels[1].pInternBuffer = new WORD[PPG_BUFFER_SIZE];
	m_Channels[1].nSizeOfIntBuf = PPG_BUFFER_SIZE;
	m_Channels[1].pExternBuffer = new WORD[PPG_BUFFER_SIZE];
	m_Channels[1].nSizeOfExtBuf = PPG_BUFFER_SIZE;
	//m_Channels[1].hSemaphore = ::CreateSemaphore(NULL, 1, 1, NULL);
	sem_init(&m_Channels[1].hSemaphore, 0, 1);

	this->registryReadMulitpliers( );

////////Dont use registry//////////////////
	int numAverageSensorSamples = defNumAverageSensorSamples;

	int firThreshold = defFirThreshold;

	int firHysteresis = defFirHysteresis;

	pDataCollector = new CDataCollector( numAverageSensorSamples,
										firThreshold,
										firHysteresis            );

#if RELEASE != RELEASE_TEST
	m_Channels[2].pInternBuffer = new WORD[GSR_BUFFER_SIZE];
	m_Channels[2].nSizeOfIntBuf = GSR_BUFFER_SIZE;
	m_Channels[2].pExternBuffer = new WORD[GSR_BUFFER_SIZE];
	m_Channels[2].nSizeOfExtBuf = GSR_BUFFER_SIZE;
	//m_Channels[2].hSemaphore = ::CreateSemaphore(NULL, 1, 1, NULL);
	sem_init(&m_Channels[2].hSemaphore, 0, 1);

	m_Channels[3].pInternBuffer = new WORD[TMP_BUFFER_SIZE];
	m_Channels[3].nSizeOfIntBuf = TMP_BUFFER_SIZE;
	m_Channels[3].pExternBuffer = new WORD[TMP_BUFFER_SIZE];
	m_Channels[3].nSizeOfExtBuf = TMP_BUFFER_SIZE;
	//m_Channels[3].hSemaphore = ::CreateSemaphore(NULL, 1, 1, NULL);
	sem_init(&m_Channels[3].hSemaphore, 0, 1);
#endif


	this->agcRegistryRead( );

	this->mbPowerIsSet = FALSE;
	this->mAgcPowerLevelLast = -1;

	if ( this->OpenSensor( ))
	{
		if ( this->mAgcOn == defAgcOn )
		{                                // if agc is on, start @ medium
			this->agcPowerLevelSet( defAgcPowerLevelMed, FALSE );
		}
		else
		{                                // if agc is off, set the power to
									   // the value from the registry
			this->agcPowerLevelSet( this->mAgcPowerLevel, FALSE );
		}
		this->CloseSensor( );
	}
}


CDrvM801::~CDrvM801()
{
	if ( this->OpenSensor( ) )
	{
		this->agcPowerLevelSet( defAgcPowerLevelMed, FALSE );
		this->CloseSensor( );
	}

	for ( int i = 0; i < NUMBER_OF_CHANNELS; i++ )
	{
		//::CloseHandle(m_Channels[i].hSemaphore);
		sem_destroy(&m_Channels[i].hSemaphore);
		delete[] m_Channels[i].pInternBuffer;
   		delete[] m_Channels[i].pExternBuffer;
	}

   delete pDataCollector;
}

/**
**
*/
void CDrvM801::agcRegistryRead( )
{
	this->mAgcOn = defAgcOn;
	this->mAgcPowerLevel = defAgcPowerLevelMed;
	this->mAgcAmpLimitHigh = defAgcAmpLimitHigh;
	this->mAgcAmpLimitLow  = defAgcAmpLimitLow;
}


/**
**
*/
void CDrvM801::registryReadMulitpliers( )
{
	fPpgMultiplier = ( float )atof( defPpgMultiplierStr );
    fIbiMultiplier = ( float )atof( defIbiMultiplierStr );
}


/**
** 打开传感器
*/
BOOL CDrvM801::EnableDriver(LPCSTR pPortName)
{
	if (!OpenSensor())
	{
		return FALSE;
	}
	return TRUE;
}

// 关闭传感器
BOOL CDrvM801::DisableDriver()
{
	if (!CloseSensor())
		return FALSE;
	else
		return TRUE;
}


/**
** 关闭传感器
*/
BOOL CDrvM801::CloseSensor()
{
	if ( this->m_fIsOpened )
	{
//		if ( this->m_hSensor != INVALID_HANDLE_VALUE)
//		{
//			::CloseHandle(m_hSensor);
//			m_hSensor = INVALID_HANDLE_VALUE;
//			m_fIsOpened = FALSE;
//		}
	}
	return TRUE;
}


/**
** 打开传感器
*/
BOOL CDrvM801::OpenSensor()
{
	if ( m_fIsOpened )
		return FALSE;

	BOOL MyDeviceDetected = FALSE;

	jclass clazz = m_pEnv->FindClass("com/haofengkeji/hdread/MainActivity");
	jmethodID mid = m_pEnv->GetMethodID(clazz, "startDevice", "()V");
	if (mid == NULL) {
		MyDeviceDetected = FALSE;
	} else {
		m_pEnv->CallVoidMethod(m_jObj, mid);
		MyDeviceDetected = TRUE;
	}

//	jbyte instanceMethodResult;
//	instanceMethodResult = static_cast<jbyte>(m_pEnv->CallObjectMethod(m_jObj, mid));

	this->mbDeviceDetected = MyDeviceDetected;

	if (MyDeviceDetected == FALSE)
	{
		// allow FF to continue w/o error to
		// user if USB sensor is not present
		m_fIsOpened = FALSE;
		//this->m_hSensor = INVALID_HANDLE_VALUE;
	}
	else
	{
		m_fIsOpened = TRUE;
		//this->m_hSensor = hSensor;
	}

	return MyDeviceDetected;
}

// 恢复传感器收集数据
BOOL CDrvM801::ResumeDriver()
{
	if (!StartDataCollection())
		return FALSE;
	else
		return TRUE;
}

// 停止传感器收集数据
BOOL CDrvM801::PauseDriver()
{
	if (!StopDataCollection())
		return FALSE;
	else
		return TRUE;
}

// 初始化通道
void CDrvM801::initChannels( )
{
	for ( int i = 0; i < NUMBER_OF_CHANNELS; i++ )
	{
		m_Channels[i].nPacksInIntBuf = 0;
		m_Channels[i].nStartOfIntBuf = 0;
		m_Channels[i].nPacksInExtBuf = 0;
		m_Channels[i].nPacksOmitted = 0;
	}
}

void thread_exit_handler(int sig)
{
	//printf("this signal is %d \n", sig);
	pthread_exit(0);
}

/**
** 读取数据的线程
*/
void *ReadingThreadM801F(void *arg)
{
	DWORD NumberOfBytesRead;
	CDrvM801 *pDriver = (CDrvM801 *)arg;

	if ( pDriver->mbDeviceDetected == FALSE )
	{
		pDriver->mbDeviceDetected = pDriver->OpenSensor();
	}

	if ( pDriver->mbDeviceDetected == TRUE )
	{
		pDriver->registryReadMulitpliers( );

		pDriver->mbPowerIsSet = TRUE;

		pthread_mutex_lock( &pDriver->m_hLock );
		while ( pDriver->m_fIsStarted )
		{
			// 超时强制退出
			struct sigaction actions;
			memset(&actions, 0, sizeof(actions));
			sigemptyset(&actions.sa_mask);
			actions.sa_flags = 0;
			actions.sa_handler = thread_exit_handler;
			int rc = sigaction(SIGUSR1, &actions, NULL);

			if ( pDriver->ReadReport( &NumberOfBytesRead ) > 0 )
			{
				pDriver->extractDataFromReport( NumberOfBytesRead );
			}
		}
		pthread_mutex_unlock( &pDriver->m_hLock );

		pDriver->agcFinalize( );
	}

	pthread_cond_signal( &pDriver->m_hCond );

	return 0;//return -1;
}

// 开始收集数据
BOOL CDrvM801::StartDataCollection()
{
	if (m_fIsStarted)
		return FALSE;

	m_fIsStarted = TRUE;
    mbPowerIsSet = FALSE;

	//初始化通道
    this->initChannels( );

    pDataCollector->reinitialize();

	DWORD dwThreadId;

//	m_hThread = ::CreateThread( NULL, NULL,
//			(LPTHREAD_START_ROUTINE)ReadingThreadM801F, this, 0, &dwThreadId );

	int res;
	res = pthread_create( &m_hThread, NULL, ReadingThreadM801F, this );
	if (res != 0) {
		perror("Thread creation failed");
	}

//	if (!m_hThread)
//		return FALSE;

	// You may comment the follow line - it's not important for Pentium II (I guess)
	//::SetThreadPriority(m_hThread, THREAD_PRIORITY_ABOVE_NORMAL);

	return TRUE;
}

// 停止收集数据
BOOL CDrvM801::StopDataCollection()
{
	if (!m_fIsStarted)
		return TRUE;

	m_fIsStarted = FALSE;
	mbPowerIsSet = FALSE;

	// 5秒钟的时间，等待子线程退出
	timespec to;
	to.tv_sec = time(NULL) + 5;
	to.tv_nsec = 0;
	int err = pthread_cond_timedwait( &m_hCond, &m_hLock, &to );
	if (err == ETIMEDOUT)	// 到时
	{
		// 终止线程
		int status;
		if ( (status = pthread_kill( m_hThread, SIGUSR1 ) ) != 0 )
		{
			//printf("Error cancelling thread %d, error = %d (%s)", pthread_id, status, strerror status));
		}
	}

	//::WaitForSingleObject(m_hThread, 5000);
	//::CloseHandle(m_hThread);
	//pthread_cancel(m_hThread);
	//pthread_kill();

	this->initChannels( );

	return TRUE;
}

int CDrvM801::GetDataAvailable()
{
	if ( !m_fIsStarted )
		return 0;

	if ( !mbPowerIsSet )
	{
		return 0;
	}

	for ( int i = 0; i < NUMBER_OF_CHANNELS; i++ )
	{
		//::WaitForSingleObject(m_Channels[i].hSemaphore, INFINITE);
		sem_wait(&m_Channels[i].hSemaphore);
		//----------------------------------------------------------------
		WORD *pInternBuffer = m_Channels[i].pInternBuffer;
		WORD *pExternBuffer = m_Channels[i].pExternBuffer;
		int nPacksInBuf = m_Channels[i].nPacksInIntBuf;
		int nStartOfBuf = m_Channels[i].nStartOfIntBuf;
		int nSizeOfBuf = m_Channels[i].nSizeOfIntBuf;

		m_Channels[i].nPacksInExtBuf = nPacksInBuf;

		int nTempValue = nSizeOfBuf - nStartOfBuf;
		if ( nTempValue < nPacksInBuf )
		{
			memcpy(pExternBuffer, pInternBuffer + nStartOfBuf, nTempValue * sizeof(WORD));
			pExternBuffer += nTempValue;
			nPacksInBuf -= nTempValue;
			nStartOfBuf = 0;
		}
		memcpy(pExternBuffer, pInternBuffer + nStartOfBuf, nPacksInBuf * sizeof(WORD));
		nStartOfBuf += nPacksInBuf;
		if ( nStartOfBuf >= nSizeOfBuf )
			nStartOfBuf-= nSizeOfBuf;

		m_Channels[i].nStartOfIntBuf = nStartOfBuf;
		m_Channels[i].nPacksInIntBuf = 0;
		m_Channels[i].nPacksOmitted = 0;
		//----------------------------------------------------------------
		//::ReleaseSemaphore(m_Channels[i].hSemaphore, 1, NULL);
		sem_post(&m_Channels[i].hSemaphore);
	}

	return m_Channels[0].nPacksInExtBuf;
}


float CDrvM801::GetRawPPG(int nIdx)
{
#if RELEASE == RELEASE_TEST
	const int nFactorADC = 4096;
#else
	const int nFactorADC = 65536;
#endif


	if(!m_fIsStarted) return 0;

	if ( !mbPowerIsSet )             // can't report any data till power
	{                                // has been set
		return 0;
	}

	const int nMaxConst = 1000;
	float fValue;

	nIdx %= m_Channels[0].nSizeOfExtBuf;
	int nVal = m_Channels[0].pExternBuffer[nIdx];
	fValue = (float)nVal * nMaxConst / nFactorADC;  // * 1000 / 4096

	return fValue;
}


#if RELEASE == RELEASE_TEST

// only 2 channels PPG & IBI

float CDrvM801::GetRawGSR(int nIdx)
{
	return 0;
}


#else


float CDrvM801::GetRawGSR(int nIdx)
{
#if RELEASE == RELEASE_TEST
	const int nFactorADC = 4096;
#else
	const int nFactorADC = 65536;
#endif


	if(!m_fIsStarted) return 0;

   if ( !mbPowerIsSet )             // can't report any data till power
   {                                // has been set
      return 0;
   }


	const int nMaxConst = 1000;
	float fValue;

	nIdx%=m_Channels[2].nSizeOfExtBuf;
	int nVal = m_Channels[2].pExternBuffer[nIdx];
	fValue = (float)nVal * nMaxConst / nFactorADC;

	return fValue;
}

#endif   // if RELEASE == RELEASE_TEST



#if RELEASE == RELEASE_TEST

// only 2 channels PPG & IBI

float CDrvM801::GetRawTMP(int nIdx)
{
   return 0;
}


#else


float CDrvM801::GetRawTMP(int nIdx)
{
#if RELEASE == RELEASE_TEST
	const int nFactorADC = 4096;
#else
	const int nFactorADC = 65536;
#endif


	if(!m_fIsStarted) return 0;

   if ( !mbPowerIsSet )             // can't report any data till power
   {                                // has been set
      return 0;
   }


	const int nMaxConst = 100;
	float fValue;

	nIdx%=m_Channels[3].nSizeOfExtBuf;
	int nVal = m_Channels[3].pExternBuffer[nIdx];
	fValue = (float)nVal * nMaxConst / nFactorADC + 65.0f;

	return fValue;
}

#endif   // if RELEASE != RELEASE_TEST


void CDrvM801::reportCurrentAverage()
{
	int nChannelIdx = 0;

	WORD ValuePPG = ( WORD )pDataCollector->averageSensorSampleGet();

	ValuePPG = ( WORD )(( ValuePPG * fPpgMultiplier ) + 0.5f ); // grep:PPG

	// ---------------------------------------------------------------

	//WaitForSingleObject( m_Channels[nChannelIdx].hSemaphore, INFINITE );
	sem_wait(&m_Channels[nChannelIdx].hSemaphore);

	// --------------------------------------------------------------
	int nPacksInBuf = m_Channels[nChannelIdx].nPacksInIntBuf;
	int nStartOfBuf = m_Channels[nChannelIdx].nStartOfIntBuf;
	int nSizeOfBuf =  m_Channels[nChannelIdx].nSizeOfIntBuf;

	int nCurrentIdx = nStartOfBuf + nPacksInBuf;
	if (nCurrentIdx >= nSizeOfBuf)
		nCurrentIdx -= nSizeOfBuf;
	m_Channels[nChannelIdx].pInternBuffer[nCurrentIdx] = ValuePPG;

	if (nPacksInBuf < nSizeOfBuf)
	{
		nPacksInBuf++;
	}
	else
	{
		if (++nStartOfBuf == nSizeOfBuf)
			nStartOfBuf = 0;
		m_Channels[nChannelIdx].nPacksOmitted++;
	}
	m_Channels[nChannelIdx].nPacksInIntBuf = nPacksInBuf;
	m_Channels[nChannelIdx].nStartOfIntBuf = nStartOfBuf;

	// --------------------------------------------------------------
	//ReleaseSemaphore( m_Channels[nChannelIdx].hSemaphore, 1, NULL );
	sem_post(&m_Channels[nChannelIdx].hSemaphore);
}


/**
**
*/

void CDrvM801::reportCurrentIbi()
{
	int nChannelIdx = 1;

	//WaitForSingleObject( m_Channels[nChannelIdx].hSemaphore, INFINITE );
	sem_wait(&m_Channels[nChannelIdx].hSemaphore);

	// --------------------------------------------------------------
	int nPacksInBuf = m_Channels[nChannelIdx].nPacksInIntBuf;
	int nStartOfBuf = m_Channels[nChannelIdx].nStartOfIntBuf;
	int nSizeOfBuf =  m_Channels[nChannelIdx].nSizeOfIntBuf;

	int nCurrentIdx = nStartOfBuf + nPacksInBuf;

	if (nCurrentIdx >= nSizeOfBuf)
		nCurrentIdx -= nSizeOfBuf;


   // -------------------------------------------------------
   // grep:Report IBI
   // Magnitude of IBI influences the reported Heart Rate (HR).
   // Too small and the HR is too fast; to large and the HR is
   // too slow. For the same subject, the serial sensor will
   // report IBI from 1600 to 1800; the USB sensor up to 230.
   //
   // ibi *=  1;      5 too small, HR around 600; should be ~88
   // ibi *=  5;      5 too small, HR around 114; should be ~78
   // ibi *=  6;      6 too small; HR         91
   // ibi *=  7;      7 seems OK ; HR         79
   // ibi *=  8;      8 too large; HR         72
   // ibi *=  9;      9 too large; HR         61;
   // ibi *= 10;     10 too large; HR         50;

   WORD ibi = ( WORD ) pDataCollector->latestIbiGet();

   ibi = ( WORD )(( ibi * 4 ) + 0.5f );   // grep:IBI

   // -------------------------------------------------------

	m_Channels[nChannelIdx].pInternBuffer[nCurrentIdx] = ibi;

	if (nPacksInBuf < nSizeOfBuf)
	{
		nPacksInBuf++;
	}
	else
	{
		if(++nStartOfBuf == nSizeOfBuf)
			nStartOfBuf = 0;
		m_Channels[nChannelIdx].nPacksOmitted++;
	}
	m_Channels[nChannelIdx].nPacksInIntBuf = nPacksInBuf;
	m_Channels[nChannelIdx].nStartOfIntBuf = nStartOfBuf;
	//ReleaseSemaphore(m_Channels[nChannelIdx].hSemaphore, 1, NULL);
	sem_post(&m_Channels[nChannelIdx].hSemaphore);
}


/**
**
*/
void CDrvM801::extractDataFromReport( DWORD NumberOfBytesRead )
{
	static int sampleCount = 0;
	int idx;
	int maxIdx;
	uBeLeInts val;
	TCHAR * pInputReport;

	pInputReport = &InputReport[ 1 ];   // a full report is 9 bytes:
									    //    0th byte is report number
									    //    bytes 1 - 8 is data
	maxIdx = NumberOfBytesRead >> 1;    // 2 bytes per value, rounded down:
										//    ie, 9 >> 1 ==>> 4

	for ( idx = 0; idx < maxIdx; idx++ )
	{
									   // samples from sensor are in
									   // big endian format
		val.le.leLo = *pInputReport++;
		val.le.leHi = *pInputReport++;

		val.mInt = 1024 - val.mInt;      // grep:if need to invert signal
									     // grep:see below, too

		pDataCollector->collectNewSamplePpg( val.mInt );

		pDataCollector->collectNewSampleIbi( val.mInt );

		if ( pDataCollector->reportAverageSensorSampleGet( ))
		{
			this->reportCurrentAverage();
		}

		if ( pDataCollector->reportIbiGet( ))
		{
			this->reportCurrentIbi();
		}

	}
}

void CDrvM801::SetInputReport(jbyteArray byteArray)
{
	jbyte* a = m_pEnv->GetByteArrayElements(byteArray, NULL);
	//int textLength = strlen((const char*)a);
	//this->InputReport = malloc(textLength + 1);
	memcpy(this->InputReport, a, 16);
}

/**
**	读取数据
*/
int CDrvM801::ReadReport( DWORD * pNumberOfBytesRead )
{
	DWORD Result;

	//检查传感器是否打开
//	if ( this->m_hSensor == INVALID_HANDLE_VALUE )
//	{
//		this->OpenSensor( );
//		if ( this->m_hSensor == INVALID_HANDLE_VALUE )
//		{
//			return 0;
//		}
//	}
//
//	//Read a report from the device.
//	Result = ReadFile(  this->m_hSensor,			//传感器
//						this->InputReport,			//接收数据缓冲区
//						16,							//要读取的字节数
////		                Capabilities.InputReportByteLength,
//						pNumberOfBytesRead,			//接收读取的字节数
//						NULL );
//
//   if ( Result == 0 )
//   {
//		CloseHandle( this->m_hSensor );
//        this->m_fIsOpened = false;
//        this->m_hSensor = INVALID_HANDLE_VALUE;
//		this->mbDeviceDetected = FALSE;
//   }

//    char a[16] = "aaaaaaa";
//
//    OutputDebugString(this->InputReport);
//    OutputDebugString("\n");//LPCSTR

	return Result;
}


/**
** CDrvM801::WriteReport()
**
** write report to the USB sensor; currently we have only one report,
** and we use it to set the power level.
*/
int CDrvM801::WriteReport( int powerLevel )
{
	ULONG Result;
	DWORD BytesWritten;

//	if ( this->m_hSensor == INVALID_HANDLE_VALUE )
//	{
//		this->OpenSensor( );
//		if ( this->m_hSensor == INVALID_HANDLE_VALUE )
//		{
//			return 0;
//		}
//	}

// 	if ( Capabilities.OutputReportByteLength <= 0 )
// 	{
// 		return 0;
// 	}

	//Send a report to the device.
	this->OutputReport[ 0 ] = 0;			//The first byte is the report number.
	this->OutputReport[ 1 ] = powerLevel;
	this->OutputReport[ 2 ] = 0;

//  	Result = WriteFile( m_hSensor,
//   						this->OutputReport,
//						3,
////	                    Capabilities.OutputReportByteLength,
//	                    &BytesWritten,
//	                    NULL );
//
//	if (Result == 0)
//	{
//		CloseHandle( this->m_hSensor );
//		this->m_fIsOpened = false;
//		this->m_hSensor = INVALID_HANDLE_VALUE;
//		this->mbDeviceDetected = FALSE;
//	}

    return Result;
}




/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////


/**
** CDrvM801::reportMinMax
**
** Determine the min and max values in this report.
*/
void CDrvM801::reportMinMax( DWORD   NumberOfBytesRead,		//读取到的数据字节数
                             int   * pMin,
                             int   * pMax                )
{
	int idx;
	int maxIdx;
	uBeLeInts val;
	TCHAR * pInputReport;

	// a full report is 9 bytes: 0th byte is report number bytes 1 - 8 is data
	pInputReport = &InputReport[ 1 ];

	// 2 bytes per value, rounded down: ie, 9 >> 1 ==>> 4
	maxIdx = NumberOfBytesRead >> 1;

	// maxIdx = 4 now
	for ( idx = 0; idx < maxIdx; idx++ )
	{
		// samples from sensor are in big endian format
		val.le.leHi = *pInputReport++;
		val.le.leLo = *pInputReport++;

		pDataCollector->collectNewSamplePpg( val.mInt );

		pDataCollector->collectNewSampleIbi( val.mInt );

		if ( val.mInt > *pMax )
		{
			*pMax = val.mInt;
		}

		if ( val.mInt < *pMin )
		{
			*pMin = val.mInt;
		}
	}
}


/**
**
*/

#define defAgcPwrReportsPerSec      (  80 )

#define defAgcPwrIntervals          (   5 )
#define defAgcPwrIntervalSecsEach   (   2 )

#define defAgcPwrVoteCount          (( defAgcPwrIntervals / 2 ) + 1 )



/**
** CDrvM801::agcAmpMinMax
**
** For this time interval, determine the min and max report values
*/
void CDrvM801::agcAmpMinMax( int seconds,
                             int * pMin,
                             int * pMax     )
{
	DWORD NumberOfBytesRead;
	int reportCount;

	reportCount = seconds * defAgcPwrReportsPerSec;

	while ( this->m_fIsStarted )
	{
		if ( this->ReadReport( &NumberOfBytesRead ) > 0 )	//读取成功
		{
			this->reportMinMax( NumberOfBytesRead, pMin, pMax );

			reportCount--;
			if ( reportCount <= 0 )
			{
				break;
			}
		}
	}

//   progressBarStep( );
}


/**
** CDrvM801::agcUpdatePowerLevel( )
**
** Set a new power level.  We start at medium power (1), and we
** will either decrease by 1 to low power (0), or increase by one
** to high power (1), depending on the amplitude of the signal.
**
** First, for some number of samples or intervals, determine the
** max and min values reported from the USB sensor.
**
** From these values, determine the amplitude for each interval.
** Keep a running total of the number of samples that indicate a
** change in power is needed.  We require that the amplitude of
** more than half of the samples be either too big or too small
** to change the power level.
*/

#define defAgcMinMaxDeltaLen     ( 24 )
#define defAgcVotePowerLen       ( 20 )

#define defAmpValuesLen                   \
               ( defAgcMinMaxDeltaLen * ( defAgcPwrIntervals + 2 ) + \
                 defAgcVotePowerLen )



void CDrvM801::agcUpdatePowerLevel( )  // leave not UNICODE for now
{
   int min[ defAgcPwrIntervals ];
   int max[ defAgcPwrIntervals ];

   int idx = 0;


	while ( this->m_fIsStarted && idx < defAgcPwrIntervals )
	{
      min[ idx ] = 1024;
      max[ idx ] =    0;
                                    // determine the min and max for
                                    // this interval
      this->agcAmpMinMax( defAgcPwrIntervalSecsEach,
                          &min[ idx ],
                          &max[ idx ]  );
      idx++;
   }


   char tmpStr  [  40 ];
   char valueStr[ defAmpValuesLen ];


   valueStr[ 0 ] = '\0';

   int vote = 0;                    // determine the number of intervals
                                    // that indicate a change in power
                                    // is needed
   for ( idx = 0; idx < defAgcPwrIntervals; idx++ )
   {
      int delta = max[ idx ] - min[ idx ];

                                    // let's say each of these is
                                    // defAgcMinMaxDeltaLen bytes
//      sprintf( tmpStr, _T("[%d, %d]%d "), max[ idx ], min[ idx ], delta );
	  sprintf( tmpStr, "[%d, %d]%d ", max[ idx ], min[ idx ], delta );

      strcat( valueStr, tmpStr );

      if ( delta >= this->mAgcAmpLimitHigh )
      {
         vote--;
      }
      else if ( delta <= this->mAgcAmpLimitLow )
      {
         vote++;
      }
   }


//   TCHAR * pStr = _T("Med");
   TCHAR * pStr = "Med";
   int powerLevel = defAgcPowerLevelMed;

                                    // set the new power level if req'd
   if ( vote >= defAgcPwrVoteCount )
   {
      pStr = "High";
      powerLevel = defAgcPowerLevelHigh;
   }
   else if ( vote <= ( -1 * defAgcPwrVoteCount ))
   {
//      pStr = _T("Low");
      pStr = "Low";
      powerLevel = defAgcPowerLevelLow;
   }

   this->agcPowerLevelSet( powerLevel, TRUE );


                                    // Let's say this is
                                    // defAgcVotePowerLen bytes
//   sprintf( tmpStr, _T(" = %d, %s "), vote, pStr );

//   strcat( valueStr, tmpStr );

//   AfxGetApp()->WriteProfileString( REG_SECT_CONSTANTS,
//                                    defRegAmpMaxMinValues,
//                                    valueStr                );
}


/**
** CDrvM801::agcPowerLevelSet
**
** Set the sensor's power level.
*/
void CDrvM801::agcPowerLevelSet( int  powerLevel,
                                 bool delayAfterIfChanged )
{
	int dummyMin;
	int dummyMax;

	if ( powerLevel < defAgcPowerLevelLow )
	{
		powerLevel = defAgcPowerLevelLow;
	}
	else if ( powerLevel > defAgcPowerLevelHigh )
	{
		powerLevel = defAgcPowerLevelHigh;
	}

	this->mAgcPowerLevel = powerLevel;

	if ( powerLevel != this->mAgcPowerLevelLast )
	{
		//设置难度
		this->WriteReport( powerLevel );

//      AfxGetApp()->WriteProfileInt( REG_SECT_CONSTANTS,
//                                    defRegAgcLastPowerLevel,
//                                    powerLevel     );

		this->mAgcPowerLevelLast = powerLevel;

		if ( delayAfterIfChanged == TRUE )
		{
			for ( int i = 0; i < defAgcPwrIntervals; i++ )
			{
				this->agcAmpMinMax( defAgcPwrIntervalSecsEach,
									&dummyMin,
									&dummyMax  );
			}
		}
	}

	this->mbPowerIsSet = TRUE;
}


/**
**
*/

void CDrvM801::agcInitialize( )
{
   this->agcRegistryRead( );

   if ( this->mAgcOn == defAgcOn )
   {
//      progressBarCreate( );
                                    // if agc is on, start and end at medium
      this->agcPowerLevelSet( defAgcPowerLevelMed, TRUE );
      this->agcUpdatePowerLevel( );

//      progressBarDestroy( );
   }
   else
   {                                // if agc is off, set the power to
                                    // the value from the registry
      this->agcPowerLevelSet( this->mAgcPowerLevel, TRUE );
   }
}


/**
**
*/

void CDrvM801::agcFinalize( )
{
   if ( this->mAgcOn == defAgcOn )
   {                                // if agc is on, start and end at medium
      this->agcPowerLevelSet( defAgcPowerLevelMed, FALSE );
   }
}



int CDrvM801::GetIBI()
{
	const int nMaxConst = 2048;
#if RELEASE == RELEASE_TEST
	const int nFactorADC = 4096;
#else
	const int nFactorADC = 65536;
#endif

	if (!m_fIsStarted)
		return 0;

	int nLastIBI = (int)m_Channels[1].pExternBuffer[0];

                                       // * 2048 / 4096
	int nIBI = mul_div(nLastIBI, nMaxConst, nFactorADC);

	return nIBI;
}



int CDrvM801::GetPPGValue(int nIdx)
{
	const int nMaxConst = 1000;
#if RELEASE == RELEASE_TEST
	const int nFactorADC = 4096;
#else
	const int nFactorADC = 65536;
#endif
	int nVal;

	if (!m_fIsStarted)
		return 0;

	if ( !mbPowerIsSet )             // can't report any data till power
	{                                // has been set
		return 0;
	}

	nIdx %= m_Channels[0].nSizeOfExtBuf;
	nVal = m_Channels[0].pExternBuffer[nIdx];

    int ppg = mul_div(nVal, nMaxConst, nFactorADC);

	return ppg;
}


int CDrvM801::GetIBIValue(int Ind)
{
	const int nMaxConst = 2048;
#if RELEASE == RELEASE_TEST
	const int nFactorADC = 4096;
#else
	const int nFactorADC = 65536;
#endif

	if(!m_fIsStarted) return 0;

	if ( !mbPowerIsSet )             // can't report any data till power
	{                                // has been set
		return 0;
	}


	Ind %= m_Channels[1].nSizeOfExtBuf;

	int nLastIBI = (int)m_Channels[1].pExternBuffer[Ind];
	//int nIBI = MulDiv(nLastIBI, nMaxConst, nFactorADC);
	int nIBI = mul_div(nLastIBI, nMaxConst, nFactorADC);


	return nIBI;
}


