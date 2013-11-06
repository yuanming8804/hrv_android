//////////////////////////////////////////////////////////////////////
//
// Drv.h: interface for the CDrv class.
//
// Base class for CDrvM801 (serial and USB)
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DrvBPM.h"

class CDrv
{
public:
	virtual float GetBattery();
	virtual int   GetDataAvailable();
	virtual float GetRawTMP(int nIdx);
	virtual float GetRawGSR(int nIdx);
	virtual int   GetIBI();
	virtual float GetRawPPG(int nIdx);
	virtual int   GetPPGValue(int);
	virtual int   GetIBIValue(int Ind);
    virtual int   CalibrateSensor(int initialSetting);

	virtual BOOL  ResumeDriver();
	virtual BOOL  PauseDriver();
	virtual BOOL  DisableDriver();
	virtual BOOL  EnableDriver(LPCSTR pPortName = NULL);

	BOOL IsDriverLoaded() { return m_fIsOpened; }
	BOOL IsDriverEnabled() { return m_fIsStarted; }

	ChannelInfo m_Channels[NUMBER_OF_CHANNELS];

    CDrv();
	virtual ~CDrv();

protected:
	BOOL m_fIsOpened;
	BOOL m_fIsStarted;
};
