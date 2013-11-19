// Summ.h: interface for the CSumm class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __Summ_h__
#define __Summ_h__

typedef enum
{
	HRTTYPE,
	GSRTYPE,
	TMPTYPE
} tCalcType;

class CSumm
{
public:
	BOOL LASTPOINT;
	int iTact;
	int iTactSumm;
	float fSumm, fCurrSumm;
	tCalcType CalcType;

public:
	void Calc(float & fValue);
	BOOL Tick(float fValue);
	CSumm(int Tick = 32, tCalcType CalcType = HRTTYPE);
	virtual ~CSumm();

};

#endif

