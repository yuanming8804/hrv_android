#ifndef __Trgcalc_h__
#define __Trgcalc_h__

// TRGCalc.h : header file
//
#define MINHZ	0.05f
#define MAXHZ	0.4f
#define DEVHZ	0.05f
#define RRHZ	2.0f

/////////////////////////////////////////////////////////////////////////////
// CTRGCalc command target
#include "array.h"

class CTRGCalc
{
public:
	CTRGCalc(long N = 0);           // protected constructor used by dynamic creation

// Attributes
public:
	//CRITICAL_SECTION csCTRGCalc;
	pthread_mutex_t csCTRGCalc;
	float PreScore, AScore;

	float AScoreRising, ThresholdTop, ThresholdStd;

	float CurEp;
protected:
	long NOBS;
	float PI1;
	long lP;
	float TINT, XCNTR, YCNTR;
	long NF, IFSCAL, IPRINT, ISWVER, NPAD, LDPM, LDSM, NM;
	float *X;
	float *Y;
	float *SM;
	float *PM;
	float *Frequency;
	float *WFFTR;
	long  *M;
// Operations
public:
	float CalcTarget(CContrObArray<float, float> *faTrend, float & Score, float & PreScore);
	virtual ~CTRGCalc();

};

#endif
