//////////////////////////////////////////////////////////////////////
//
//             U S B
//
// DrvM801.h: interface for the CDrvM801 class.
//
// To implement USB Driver interface for FF software
// Last Date of Modification : Oct 20 2001
//////////////////////////////////////////////////////////////////////
#ifndef __DrvM801Usb_h__
#define __DrvM801Usb_h__

#include "Drv.h"
/**
** Report sizes
**
** Sizes of the buffers for the input and output reports
** for communicating with USB Human Interface Devices (HIDs).
**
*/
#define defUsbHidReportSizeInput     ( 10 +  6 )
#define defUsbHidReportSizeOutput    (  4 + 12 )


/**
** Defines for Automatic Gain Control (AGC)
*/

/**
** AGC can be on or off (controlled by the registry).
*/

#define  defAgcOn                ( 1 )
#define  defAgcOff               ( 0 )

/////Defines for timer
#ifndef PI
#define PI 3.141592654f
#endif

#define DFT_BUF_SIZE		256

#define SAMPLE_RATE_IBI		2
#define ID_PPG_TIMER		20001
#define ID_SYS_TIMER		20002
#define PPG_TICK			100
#define SYS_TICK			1000 / SAMPLE_RATE_IBI

#define BUF_SIZE			64				// SAMPLE_RATE
#define	SAMPLE_RATE_IBI		2				// 2 samples/sec
#define	DEF_TEST_LEN		1200			// default length of test 15 min
#define IBI_BUF_SIZE		DEF_TEST_LEN * SAMPLE_RATE_IBI

#define ST_SETUP_ALEN		10
#define ST_SETUP_ABMIN		0.5f
#define ST_SETUP_ABMAX		1.4f
#define ST_SETUP_PERMIN		64
#define ST_SETUP_PERMAX		64
#define ST_SETUP_NUMEVENTS	5
#define ST_SETUP_LASTTIME	30
#define ST_SETUP_LASTEVENTS 10

#define TRG_UPDATE_RATE		10
#define MIN_IBI_COUNT		128



class CDrvM801 : public CDrv
{
private:
	friend void *ReadingThreadM801F(void *arg);

public:
	int GetPPGValue( int );
	int GetIBIValue( int Ind );

	int GetIBI( );

	int GetNumberOfChannels() { return NUMBER_OF_CHANNELS; }

	virtual float GetRawTMP(int nIdx);
	virtual float GetRawGSR(int nIdx);
	virtual float GetRawPPG(int nIdx);
	virtual int   GetDataAvailable();

	virtual BOOL PauseDriver();
	virtual BOOL ResumeDriver();
	virtual BOOL DisableDriver();
	virtual BOOL EnableDriver(LPCSTR pPortName = NULL);

	CDrvM801();
	virtual ~CDrvM801();

	//-----------------------------------------------------------
	void SetInputReport(jbyteArray byteArray);
	//-----------------------------------------------------------
private:
	//HANDLE   m_hThread;
	HANDLE   m_hSensor;												// ������

	pthread_t m_hThread;
	pthread_cond_t m_hCond;// = PTHREAD_COND_INITIALIZER;			// �߳��˳�����
	pthread_mutex_t m_hLock;// = PTHREAD_MUTEX_INITIALIZER;

	int      mAgcOn;                 // AGC on (1) or off (0)
	int      mAgcPowerLevel;         // Current power: Low(0), Med(1), Hi(2)
	int      mAgcPowerLevelLast;
	int      mAgcAmpLimitHigh;       // Amplitude limits
	int      mAgcAmpLimitLow;
	BOOL     mbDeviceDetected;       // USB sensor found
	BOOL     mbPowerIsSet;           // Power level has been set

	char     InputReport  [ defUsbHidReportSizeInput  ]; // UNICODE ??	�������ݵĻ�����
	char     OutputReport [ defUsbHidReportSizeOutput ]; // UNICODE ??  ������ݵĻ�����

	void initChannels( );												// ��ʼ��ͨ��

	void agcRegistryRead( );

	void registryReadMulitpliers( );

	BOOL StopDataCollection();											// ֹͣ�ռ�����
	BOOL StartDataCollection();											// ��ʼ�ռ�����

	BOOL OpenSensor();													// �򿪴�����
	BOOL CloseSensor();													// �رմ�����

	void extractDataFromReport( DWORD   NumberOfBytesRead  );

	int  ReadReport ( DWORD * pNumberOfBytesRead );						// ������
	int  WriteReport( int powerLevel );									// д����

	void reportCurrentAverage();

	void reportCurrentIbi();

	void reportMinMax( DWORD   NumberOfBytesRead,
					  int   * pMin,
					  int   * pMax                );

	void agcAmpMinMax( int   seconds,
					  int * pMin,
					  int * pMax     );

	void agcUpdatePowerLevel( );

	void agcPowerLevelSet( int powerLevel, bool delayAfterIfChanged );	//�����Ѷ�

	void agcInitialize( );

	void agcFinalize( );
};

#endif

