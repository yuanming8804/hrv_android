#pragma once

// DrvBPM.h : header file
//
#include "PreInclude.h"

#define RELEASE_TEST		1		// Device with old protocol
#define RELEASE_COMMERCIAL	2		// Device with new protocol
#define RELEASE_DEMO		3		// Always simulator

#define RELEASE	RELEASE_TEST
//#define RELEASE	RELEASE_COMMERCIAL
//#define RELEASE	RELEASE_DEMO


#if RELEASE == RELEASE_TEST

#define NUMBER_OF_CHANNELS 2
#define PPG_BUFFER_SIZE 512
#define IBI_BUFFER_SIZE 1
#define CACHE_SIZE 128

#else

#define NUMBER_OF_CHANNELS 4
#define PPG_BUFFER_SIZE 512
#define IBI_BUFFER_SIZE 512
#define GSR_BUFFER_SIZE 512
#define TMP_BUFFER_SIZE 512
#define CACHE_SIZE 512

#endif

#define PORT_SETTINGS_STR "baud=57600 parity=n data=8 stop=1"
#define MASK_OF_CHANNELS 0x60

struct ChannelInfo
{
	WORD   * pExternBuffer;
	int	     nSizeOfExtBuf;
	int      nPacksInExtBuf;
	//HANDLE   hSemaphore;
	sem_t 	 hSemaphore;
	WORD   * pInternBuffer;
	int      nSizeOfIntBuf;
	int	     nStartOfIntBuf;
	int      nPacksInIntBuf;
	int      nPacksOmitted;
};


/**
**  fle::following not used
**
**  class CDrvBPM
**  {
**  	friend DWORD ReadingThread(CDrvBPM *);
**  public:
**  	CDrvBPM();
**  	virtual ~CDrvBPM();
**
**  	BOOL EnableDriver(LPCSTR pPortName = NULL);
**  	BOOL DisableDriver();
**
**  	BOOL ResumeDriver();
**  	BOOL PauseDriver();
**
**  	int GetNumberOfChannels() { return NUMBER_OF_CHANNELS; }
**
**  	BOOL IsDriverLoaded() { return m_fIsOpened; }
**  	BOOL IsDriverEnabled() { return m_fIsStarted; }
**
**  	int GetIBI();
**  	int GetDataAvailable();
**  	int GetPPGValue(int);
**  	WORD *GetPPG() { return m_Channels[0].pExternBuffer; }
**  	int GetVariousParameters(int *, int);
**
**  protected:
**  	BOOL OpenBPM(LPCSTR);
**  	BOOL CloseBPM();
**  	BOOL OpenPort(LPCSTR);
**  	BOOL ClosePort();
**  	BOOL IsDevicePresent();
**  	BOOL StartDataCollection();
**  	BOOL StopDataCollection();
**  	void TurnDeviceOn();
**  	void TurnDeviceOff();
**
**  //private:
**  public:
**  	int GetIBIValue(int  Ind);
**  	HANDLE m_hPort;
**  	HANDLE m_hThread;
**  	ChannelInfo m_Channels[NUMBER_OF_CHANNELS];
**
**
**  	BOOL m_fIsOpened;
**  	BOOL m_fIsStarted;
**  	BOOL bOutMessage;
**  	WORD wBytes;
**  };
**  fle::above not used
*/

