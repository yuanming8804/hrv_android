// Drv.cpp: implementation of the CDrv class.
//
//////////////////////////////////////////////////////////////////////

#include "Drv.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrv::CDrv()
{
	m_fIsStarted = FALSE;
	m_fIsOpened = FALSE;
}

CDrv::~CDrv()
{

}

BOOL CDrv::EnableDriver(LPCSTR pPortName)
{
	return TRUE;
}

BOOL CDrv::DisableDriver()
{
	return TRUE;
}

BOOL CDrv::PauseDriver()
{
	return TRUE;
}

BOOL CDrv::ResumeDriver()
{
	return TRUE;
}

float CDrv::GetRawPPG(int nIdx)
{
	return 0.0f;
}

int CDrv::GetIBI()
{
	return 0;
}

int  CDrv::GetPPGValue(int Ind)
{
    return 0;
}

int  CDrv::GetIBIValue(int Ind)
{
    return 0;
}

float CDrv::GetRawGSR(int nIdx)
{
	return 0.0f;
}

float CDrv::GetRawTMP(int nIdx)
{
	return 0.0f;
}

int CDrv::GetDataAvailable()
{
	return 0;
}

float CDrv::GetBattery()
{
	return 0.0f;
}

int CDrv::CalibrateSensor(int initialSetting)
{
    return 0;
}
