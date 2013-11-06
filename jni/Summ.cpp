// Summ.cpp: implementation of the CSumm class.
//
//////////////////////////////////////////////////////////////////////

#include "PreInclude.h"
#include "Summ.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSumm::CSumm(int Tact, tCalcType _CalcType)
{
	iTact = 0;
	iTactSumm = Tact;
	fSumm = 0.0f;
	fCurrSumm = 0.0f;
	CalcType = _CalcType;
	LASTPOINT = TRUE;
}

CSumm::~CSumm()
{
}

BOOL CSumm::Tick(float fValue)
{
	BOOL Ok = FALSE;
	iTact++;
	if (fValue > 0)
	{
		if (LASTPOINT)
		{
			fCurrSumm = fValue;
		}
		else
		{
			fCurrSumm += fValue;
		}
	}
	else
	{
		fCurrSumm = fSumm;
	}
	if (iTact == iTactSumm)
	{
		if (LASTPOINT)
		{
			fSumm = fCurrSumm;
		}
		else
		{
			fSumm = fCurrSumm / iTactSumm;
		}
		Calc(fSumm);
		fCurrSumm = 0.0f;
		Ok = TRUE;
		iTact = 0;
	}
	return Ok;
}

#define TMP_A 30.0f/4096
#define TMP_B 65.0f

//int iTick = 0;
//int Ampl = 2048;
//#include "math.h"

void CSumm::Calc(float & fValue)
{
	switch(CalcType)
	{
	case HRTTYPE:
		if (fValue < 30)
			fValue = 30;
		break;
	case TMPTYPE:
		fValue = TMP_A * fValue + TMP_B;
		if (fValue < 65)
			fValue = 65;
		if (fValue > 105)
			fValue = 105;
		break;
	case GSRTYPE:
//fValue = 2048+Ampl * sin (iTick/10.0);
//Ampl-=10;
//iTick++;
//if (Ampl<0) Ampl=0;
		fValue = 1000 - fValue * 1000 / 4096;
		if (fValue < 0)
			fValue = 0;
		if (fValue > 1000)
			fValue = 1000;
		break;
	}
}
