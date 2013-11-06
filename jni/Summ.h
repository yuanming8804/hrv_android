// Summ.h: interface for the CSumm class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

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


