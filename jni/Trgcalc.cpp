// TRGCalc.cpp : implementation file
//

//#include "stdafx.h"
#include "TRGCalc.h"
#include "math.h"
#include "DrvInterface.h"
#include "Fun_Interface.h"


/////////////////////////////////////////////////////////////////////////////
// CTRGCalc

CTRGCalc::CTRGCalc(long N)
{

	// N - number of interbeat interval samples to be processed with FFT

	//ERR(14000)

	//InitializeCriticalSection(&csCTRGCalc);
	if (0 != pthread_mutex_init(&csCTRGCalc, NULL))
	{
	}

	NOBS = N;
	NF	 = NOBS / 2;
	LDPM = NOBS;
	LDSM = NF;
	PI1 = 3.141592654f;
	NM = 1;
	lP = 1;
	TINT = 0.5f;
	IFSCAL = 0;
	ISWVER = 5;
	IPRINT = 0;
	NPAD = 0;
	PreScore = 0.0f;
	AScore = 0.0f;
	AScoreRising = 0.0f;
	ThresholdTop = 0.0f;
	ThresholdStd = 0.0f;

	CurEp = 0.0f;

	X	= new float[NOBS];
	Y	= new float[NOBS];
	PM	= new float[NOBS * 10];
	SM	= new float[NOBS / 2 * (NM * 7 + 2)];
	M	= new long[NM];
	WFFTR	  = new float[2 * NOBS + 15]; // This is an array of parameters needed by Fortran FFT routines
	Frequency = new float[NOBS];

	M[0] = 10;

	for (int i = 1; i <= NF; i++)
	{
		Frequency[i - 1] = PI1 * (float)i / (float)(NF); // Array of frequencies in the range of 0 - 1.0 Hz
	}


//	FFTRI (&NOBS, WFFTR); // Calculation of special parameters mentioned above
}

// Target calculation class destructor. It is called when deleting a class
// object. It is done once when exiting online mode. There is nothing special here,
// just deleting dynamic data buffers

CTRGCalc::~CTRGCalc()
{
//AfxMessageBox("~CTRGCalc()");
	//ERR(30100)
	if (X  != NULL) delete[] X;
	if (Y  != NULL) delete[] Y;
	if (PM != NULL) delete[] PM;
	if (SM != NULL) delete[] SM;
	if (Frequency != NULL) delete[] Frequency;
	if (WFFTR != NULL) delete[] WFFTR;
	if (M != NULL) delete[] M;
	//ERR(30101)
	//DeleteCriticalSection(&csCTRGCalc);
	if (0 != pthread_mutex_destroy(&csCTRGCalc)) {}
	//ERR(30102)
//AfxMessageBox("~CTRGCalc()+");
}


/////////////////////////////////////////////////////////////////////////////
// CTRGCalc message handlers

#define NMSMOOTH 15 // Smoothing order

// This is the main and only procedure of Target calculation. It is called every time
// we have a new sample of IBI. The input parameter *faTrend is a pointer to IBI buffer,
// which is an accumulator of IBIs. We use last N samples to do moving calculation.
float CTRGCalc::CalcTarget(CContrObArray<float, float> *faTrend, float & Score, float & PreScore)
{
	//ERR(14010)
	//EnterCriticalSection(&csCTRGCalc);//LeaveCriticalSection(&csExchangeData);
	if (0 != pthread_mutex_lock(&csCTRGCalc)) {}

	static float OldScore = 0;
	float Average = 0;
	float fMaxPeak = 0;
	float PSum = 0;
	float Below = 0;
	float Above = 0;
	float EP = 0;
	int iMaxPeak = 0;
	int NmVal = 0;
	static float Smooth[NMSMOOTH];
	static int iSmooth = 0;
	static int iReady = 0;

	if (NOBS < 64)
	{
		//LeaveCriticalSection(&csCTRGCalc);	//!!!Error
		if (0 != pthread_mutex_unlock(&csCTRGCalc)) {}
		return 10.0f;//0.0f; // No calculation if less than 64 samples are set up
	}

	CContrObArray<float, float> * pFFT = Fun_interface::Instance()->GetFFT();
	int Size = pFFT->GetSize();

	if (!Size)// < NOBS)
	{
		//LeaveCriticalSection(&csCTRGCalc);	//!!!Error
		if (0 != pthread_mutex_unlock(&csCTRGCalc)) {}
		return 20.0f;//0;	// No calculation until at least a preset number of
	}
								// samples were collected

	// The following code calculates an average coefficient of all bins of coherence function
	// in the range of 0.05 to 0.4 Hz

	for (int i = ST_SETUP_SBS; i < ST_SETUP_SBE - 1; i++)
	{
		float f = pFFT->GetAt(i);			//*(SM+NF*8+i);
		float f1 = pFFT->GetAt(i + 1);
		float f2 = pFFT->GetAt(i - 1);
		if (f > f1 && f > f2 && fMaxPeak < f)
		{
			fMaxPeak = f;
			iMaxPeak = i;
		}
	}

	PreScore = OldScore;

	if (fMaxPeak)
	{
		for (int i = iMaxPeak - ST_SETUP_P1; i <= iMaxPeak + ST_SETUP_P2; i++)
			PSum += pFFT->GetAt(i);

		if (iMaxPeak - ST_SETUP_B2 <= ST_SETUP_B1)
			Below = PSum;
		else for (int i = ST_SETUP_B1 - 1; i <= iMaxPeak - ST_SETUP_B2; i++)
			Below += pFFT->GetAt(i);

		for (int i = iMaxPeak + ST_SETUP_A1; i < ST_SETUP_A2; i++)
		{
			if (i < pFFT->GetSize())
				Above += pFFT->GetAt(i);
		}

		EP = (PSum / Below) * (PSum / Above);

		CurEp = EP;

		if (EP < ST_SETUP_FNLT1)
		{
			Score = 0;
		}
		else if (EP < ST_SETUP_FNLT2)
		{
			Score = 1;
		}
		else
		{
			Score = 2;
		}
	}
	else
	{
		Score = 0;
	}

	// Calculation Score

	float LastAScore = AScore;	//pjf
	if (Score == 2 && PreScore == 0) AScore += ST_SETUP_INC1;
	if (Score == 1 && PreScore == 0) AScore += ST_SETUP_INC2;
	if (Score == 0 && PreScore == 0) AScore += ST_SETUP_INC3;

	if (Score == 2 && PreScore == 1) AScore += ST_SETUP_INC4;
	if (Score == 1 && PreScore == 1) AScore += ST_SETUP_INC5;
	if (Score == 0 && PreScore == 1) AScore += ST_SETUP_INC6;

	if (Score == 2 && PreScore == 2) AScore += ST_SETUP_INC7;
	if (Score == 1 && PreScore == 2) AScore += ST_SETUP_INC8;
	if (Score == 0 && PreScore == 2) AScore += ST_SETUP_INC9;

	if (AScore < 0) AScore = 0;

	AScoreRising += Score;
	ThresholdTop += 2;
	ThresholdStd += 1;

	OldScore = Score;

	//LeaveCriticalSection(&csCTRGCalc);
	if (0 != pthread_mutex_unlock(&csCTRGCalc)) {}

	return AScore; // Returns final smoothed value of Target in %
}
