// DrvDLL.cpp: implementation of the CDrvInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "PreInclude.h"

#include "DrvInterface.h"

#include "Drv.h"
#include "array.h"
#include "summ.h"
#include "trgCalc.h"
#include "DrvM801USB.h"
#include "mpak8732.h"
#include "math.h"

#include "Fun_Interface.h"

#include <vector>

//#ifdef _DEBUG
//#undef THIS_FILE
//static TCHAR THIS_FILE[]=__FILE__;
//#endif


using namespace std;
Fun_interface::Fun_interface(void)
{
	inited = false;
	pDrv = NULL;

	//Initialize();
}

Fun_interface::~Fun_interface(void)
{
}

Fun_interface* Fun_interface::Instance()
{
	static Fun_interface instance;
	return &instance;
}

BOOL Fun_interface::Initialize()
{
	hTimerPPG = NULL;
	hTimerSYS = NULL;
	//hTimerQueue = NULL;
	//m_hTimerThread = NULL;

	piTemp = NULL;
	pfTemp = NULL;

	faPPG.Name = "faPPG";
	faPPG.SetSize(BUF_SIZE);
	faIBIArr.Name = "faIBIArr";
	faIBIArr.SetSize(0, 10);
	Tmp.SetSize(BUF_SIZE);
	faIBI.Name = "faIBI";
	faIBI.SetSize(0, IBI_BUF_SIZE);	// IBI_BUF_SIZE==2400
	faHRT.Name = "faHRT";
	faHRT.SetSize(0, IBI_BUF_SIZE);
	faHRTArtifact.SetSize(0, IBI_BUF_SIZE);
	faSpectrumIBI.Name = "faSpectrumIBI";
	faSpectrumIBI.SetSize(0);
	faTRG.Name = "faTRG";
	faTRG.SetSize(0, IBI_BUF_SIZE);

	iArt = -1;
	iArtTotal = 0;

	iLastIBI = 1000;
	nCurrentIBI = -1;
	nCurrentHRT = 72.0;
	nGlobalCountIBI = 0;
	cTRGCount = 0;
	X = NULL;
	AvgAlen.SetSize(0, 10);
	pTRGCalc = NULL;
	fTargetFFT = 0.0f;
	fTargetFFTRising = 0.0f;
	fTargetFFTTop = 0.0f;
	fTargetFFTStd = 0.0f;
	Score = 0;
	PreScore = 0;
	arg_ppg = ID_PPG_TIMER;
	arg_sys = ID_SYS_TIMER;
	for (int i = 0; i < DIFFLEVEL; ++i)
	{
		DiffARRAY[i].DOWN = DiffARRAY[i].UP = 0.0f;
	}
	CurDiff = 0;

	nTotalSamples = 0;
	//////////////////////////////////////////////////
	bBlock = FALSE;				//清除阻塞标志
	bReadOutPPG = false;		//脉博数据没有数据读取
	bReadOutHRV = false;		//HRV没有数据读取
	bReadOutEP = false;			//EP没有数据读取
	bReadOutIBI = false;		//IBI没有数据读取
	ReturnHRV.RemoveAll();
	ReturnEP.RemoveAll();
	if (pfTemp != NULL)
	{
		delete[] pfTemp;
		pfTemp = NULL;
	}
	if (piTemp != NULL)
	{
		delete[] piTemp;
		piTemp = NULL;
	}
	//////////////////////////////////////////////////
	iCurrentBuffer = 0;
	if (IBISumm == NULL)
	{
		IBISumm = new CSumm;
	}
	if (pTRGCalc == NULL)
	{
		pTRGCalc = new CTRGCalc(MIN_IBI_COUNT);
	}
	if (X == NULL)
	{
		X = new float[MIN_IBI_COUNT];
	}
	int nNum = MIN_IBI_COUNT / 2;

	faSpectrumIBI.RemoveAll();
	faSpectrumIBI.SetSize(nNum);

	/////////////////////////////////////

	if (pDrv == NULL)
	{
		pDrv = new CDrvM801;
	}

	inited = true;

	return pDrv ? TRUE : FALSE;
}

void Fun_interface::ExInitialize()
{
//	if ( !DeleteTimerQueue( hTimerQueue ) )
//	{
//		//printf("DeleteTimerQueue failed (%d)\n", GetLastError());
//	}

	hTimerPPG.StopTimer();
	hTimerSYS.StopTimer();
	delete hTimerPPG;
	delete hTimerSYS;
	//	CloseHandle(hTimerQueue);
	hTimerPPG = NULL;
	hTimerSYS = NULL;
	//hTimerQueue = NULL;
	//m_hTimerThread = NULL;

	faPPG.RemoveAll();
	faIBI.RemoveAll();	// IBI_BUF_SIZE==2400
	faHRT.RemoveAll();
	faTRG.RemoveAll();
	faSpectrumIBI.RemoveAll();
	faHRTArtifact.RemoveAll();
	faIBIArr.RemoveAll();
	Tmp.RemoveAll();
	ReturnHRV.RemoveAll();
	ReturnEP.RemoveAll();

	iLastIBI = 1000;
	nCurrentIBI = -1;
	nGlobalCountIBI = 0;
	cTRGCount = 0;
	AvgAlen.RemoveAll();
	fTargetFFT = 0.0f;
	fTargetFFTRising = 0.0f;
	fTargetFFTTop = 0.0f;
	fTargetFFTStd = 0.0f;

	Score = 0;
	PreScore = 0;
	/////////////////////////////////////
	if (pTRGCalc != NULL)
	{
		delete[] pTRGCalc;
		pTRGCalc = NULL;
	}
	if (X != NULL)
	{
		delete[] X;
		X = NULL;
	}
	if (pDrv != NULL)
	{
		delete[] pDrv;
		pDrv = NULL;
	}
	if (IBISumm != NULL)
	{
		delete[] IBISumm;
		IBISumm = NULL;
	}
	if (pfTemp != NULL)
	{
		delete[]  pfTemp;
		pfTemp = NULL;
	}
	if (piTemp != NULL)
	{
		delete[] piTemp;
		piTemp = NULL;
	}
	///////////////////////////////////

}

//设置当前测量难度
void Fun_interface::SetCurDiff(const int& diff)
{
	if (diff >= 0 && diff <= DIFFLEVEL)
	{
		CurDiff = diff;
	}
}

//获得当前测量难度
int Fun_interface::GetCurDiff()
{
	return CurDiff;
}

//设置难度上下限
bool Fun_interface::SetDiffRange(const int& diff, const float& up, const float& down)
{
	if (diff >= 0 && diff <= DIFFLEVEL)
	{
		DiffARRAY[diff].UP = up;
		DiffARRAY[diff].DOWN = down;
		return true;
	}
	return false;
}

//获得难度上下限
bool Fun_interface::GetDiffRange(const int& diff, float& up, float& down)
{
	if (diff >= 0 && diff <= DIFFLEVEL)
	{
		up = DiffARRAY[diff].UP;
		down = DiffARRAY[diff].DOWN;
		return true;
	}
	return false;
}

//获得在当前健康状态
int Fun_interface::GetHealtState()
{
	if (nGlobalCountIBI < 40)
		return 0;;
	if (pTRGCalc->CurEp < DiffARRAY[CurDiff].DOWN)
		return 0;
	else if (pTRGCalc->CurEp < DiffARRAY[CurDiff].UP)
		return 1;
	else
		return 2;
}

float Fun_interface::GetRawPPG(int nIdx)//获得即时脉博
{
	if (pDrv)
		return pDrv->GetRawPPG(nIdx);
	return 0.0;
}

//CContrObArray<float,float> * GetPPG()
//{
//
//	return &faPPG; // NEW VERSION
//}

//得到脉博数据
float * Fun_interface::GetPackPPG(int &size)
{
	size = faPPG.GetSize();
	if (pfTemp != NULL)
	{
		delete[] pfTemp;
		pfTemp = NULL;

	}
	//	if(size > 0)
	pfTemp = new float[size];

	for(int i = 0; i < size; ++i)
	{
		pfTemp[i] = faPPG.GetAt(i);
	}
	return pfTemp;
}

//获得当前HRV函数指针
float* Fun_interface::GetPackHrt(int &size)
{
	size = ReturnHRV.GetSize();
	if (pfTemp != NULL)
	{
		delete[] pfTemp;
		pfTemp = NULL;

	}
	//	if(size > 0)
	pfTemp = new float[size];

	for(int i = 0;i < size; ++i)
	{
		pfTemp[i] = ReturnHRV.GetAt(i);
	}
	ReturnHRV.RemoveAll();
	return pfTemp;
	//return nCurrentHRT;
}

//得到EP数据
float * Fun_interface::GetPackEP(int &size)
{
	size = ReturnEP.GetSize();
	if (pfTemp != NULL)
	{
		delete[] pfTemp;
		pfTemp = NULL;

	}
	//	if(size > 0)
	pfTemp = new float[size];

	for(int i = 0; i < size; ++i)
	{
		pfTemp[i] = ReturnEP.GetAt(i);
	}
	ReturnEP.RemoveAll();
	return pfTemp;
}
//////////////////////////////////////////

////获得当前脉博函数指针
//float GetCurPPG()
//{
//	for (int i=0;i<faPPG.GetSize()-1;++i)
//	{
//		if (faPPG.GetAt(i)!=0)
//		{
//			return faPPG.GetAt(i);
//		}
//	}
//	return faPPG.GetAt(faPPG.GetSize()-1);
//}
//脉博数据是否有数据可以读取
void Fun_interface::SetReadPPG(bool bRead)
{
	bReadOutPPG = bRead;
}

bool Fun_interface::GetReadPPG()
{
	return bReadOutPPG;
}

//HRV是否有数据可以读取
void Fun_interface::SetReadHRV(bool bRead)
{
	bReadOutHRV = bRead;
}
bool Fun_interface::GetReadHRV()
{
	return bReadOutHRV;
}

//EP是否有数据读取
void Fun_interface::SetReadEP(bool bRead)
{
	bReadOutEP = bRead;
}
bool Fun_interface::GetReadEP()
{
	return bReadOutEP;
}
//IBI是否有数据可以读取
void Fun_interface::SetReadIBI(bool bRead)
{
	bReadOutIBI = bRead;
}
bool Fun_interface::GetReadIBI()
{
	return bReadOutIBI;
}
//设置阻塞标志
void Fun_interface::SetBlockFlag(bool bBlockFlag)
{
	bBlock = bBlockFlag;
}
//得到阻塞标志
bool Fun_interface::GetBlockFlag()
{
	return bBlock;
}
//获得当前频谱函数指针
float * Fun_interface::GetCurIBI(int & size)
{
	size = faSpectrumIBI.GetSize();
	if (pfTemp != NULL)
	{
		delete[] pfTemp;
		pfTemp = NULL;

	}
	pfTemp = new float[size];

	for(int i = 0; i < size; ++i)
	{
		pfTemp[i] = faSpectrumIBI.GetAt(i);
	}
	return pfTemp;
}
//获得当前分数
int Fun_interface::GetCurAScore()
{
	if (nGlobalCountIBI >= 40)
		return (int)fTargetFFT;
	return 0;
}
//获得当前EP
float Fun_interface::GetCurEp()
{
	if (nGlobalCountIBI >= 40)
		return pTRGCalc->CurEp;
	return 0.0f;
}
//获得当前新的数据
int Fun_interface::GetSamplesRead()
{
	return nSamplesRead;
}

//开启设备
BOOL Fun_interface::StartDevice()
{
	//if(!Initialize())
	//	return FALSE;
	//else
	//	return pDrv->EnableDriver( NULL );

	Initialize();
	if (pDrv == NULL)
		return FALSE;
	else
		return pDrv->EnableDriver(NULL);

}

//停止设备
BOOL Fun_interface::StopDevice()
{
	if ( !pDrv )
		return FALSE;
	if ( pDrv->PauseDriver() )
	{
		if ( pDrv->DisableDriver() )
		{
			ExInitialize();
			return TRUE;
		}
		else
			return FALSE;
	}
	else
		return FALSE;
}

//开始读取数据
BOOL Fun_interface::StartRead()
{
	if (pDrv)
	{
		if (pDrv->PauseDriver())
		{
			if (pDrv->ResumeDriver())
			{
				return ReadData();
			}
			return FALSE;
		}
		return FALSE;
	}
	return FALSE;
}

int Fun_interface::GetCurScore()
{
	if (nGlobalCountIBI >= 40)
		return (int)Score;
	return 0;
}

int Fun_interface::GetCurAScoreRising()
{
	if (nGlobalCountIBI >= 40)
		return (int)fTargetFFTRising;
	return 0;
}

int Fun_interface::GetAScoreTop()
{
	if (nGlobalCountIBI >= 40)
		return (int)fTargetFFTTop;
	return 0;
}

int Fun_interface::GetAScoreStd()
{
	if (nGlobalCountIBI >= 40)
		return (int)fTargetFFTStd;
	return 0;
}

//得到设备状态
int Fun_interface::GetArtifactStatus()
{
	return iArt;
}

//得到设备个数
int Fun_interface::GetArtifactCount()
{
	return iArtTotal;
}

int Fun_interface::GetDataAvailable()
{
	if (pDrv)
		return pDrv->GetDataAvailable();
	return 0;
}

float Fun_interface::GetRawTMP(int nIdx)
{
	if (pDrv)
		return pDrv->GetRawTMP(nIdx);
	return 0.0;
}

float Fun_interface::GetRawGSR(int nIdx)
{
	if (pDrv)
		return pDrv->GetRawGSR(nIdx);
	return 0.0;
}

int Fun_interface::GetIBI()
{
	if (pDrv)
		return pDrv->GetIBI();
	return 0;
}

int Fun_interface::GetPPGValue(int nIdx)
{
	if (pDrv)
		return pDrv->GetPPGValue(nIdx);
	return 0;
}

int Fun_interface::GetIBIValue(int Ind)
{
	if (pDrv)
		return pDrv->GetIBIValue(Ind);
	return 0;
}

BOOL Fun_interface::ResumeDriver()
{
	if (pDrv)
		return pDrv->ResumeDriver();
	return FALSE;
}

BOOL Fun_interface::PauseDriver()
{
	if (pDrv)
		return pDrv->PauseDriver();
	return FALSE;
}

BOOL Fun_interface::DisableDriver()
{
	if (pDrv)
		return pDrv->DisableDriver();
	return FALSE;
}

BOOL Fun_interface::EnableDriver(LPCSTR pPortName)
{
	if (pDrv)
		return pDrv->EnableDriver( pPortName );
	return FALSE;
}


int Fun_interface::PacksInExtBuf(int channelIndex)
{
	if (pDrv)
		return pDrv->m_Channels[ channelIndex ].nPacksInExtBuf;
	return 0;
}

////My Interface

float Fun_interface::ReGetPPGValue(int nIndex)
{
	const int nMaxConst = 1000;
	const int nFactorADC = 4096;
	float fValue;

	int nVal = pDrv->GetPPGValue(nIndex);
	fValue = (float)nVal * nMaxConst / nFactorADC;
	/////////////////////////////

	if(iCurrentBuffer + nIndex < BUF_SIZE)
	{
		faPPG[nIndex+iCurrentBuffer] = fValue;
	}
	else
	{
		for(int i = 0; i < BUF_SIZE - 1; i ++)
			faPPG[i] = faPPG[i + 1];
		faPPG[BUF_SIZE - 1] = fValue;
	}

	return fValue;
}

CContrObArray<float,float> * Fun_interface::GetFFT()
{
	return &faSpectrumIBI;
}

void Fun_interface::ReadDataFromDriver()
{
	static DWORD dwSmpInt = 0L;
	static DWORD dwTickInt = 0L;
	static int cSampleCnt = 0;
	int PPGPoints = 0;
	int i;

	if (pDrv->IsDriverLoaded() && pDrv->IsDriverEnabled())
	{
		nSamplesRead = pDrv->GetDataAvailable();		// pjf - blocks until data is available...
		nTotalSamples += nSamplesRead;
		if (faPPG.GetSize() <= nSamplesRead) 			//!!!Error
		{
			nSamplesRead = faPPG.GetSize();
		}

		//		for (int i=0; i<nSamplesRead; i++)
		//            faPPG[i] = -1.0f;
		//脉博数据没有数据可以读取
		if (bReadOutPPG == FALSE)
		{
			for (i = 0; i < faPPG.GetSize(); i++)
				faPPG[i] = -1.0f;
		}

		for (i = 0; i < faPPG.GetSize(); i++)
		{
			if (faPPG[i] == -1)
			{
				iCurrentBuffer = i;
				break;
			}
		}

		for (i = 0; i < nSamplesRead; i++)
		{
			float fVal = ReGetPPGValue(i);
		}

		//脉博数据有数据可以读取
		if (nSamplesRead > 0)
			bReadOutPPG = TRUE;

		int npacks = PacksInExtBuf(1);
		for (i = 0; i < npacks; i++)
		{
			int iIBI = pDrv->GetIBIValue(i);

			if (iIBI > 0)
			{
				iLastIBI = iIBI;
			}

			if (IBISumm->Tick((float)iLastIBI))
			{
				faIBIArr.Add((int)(IBISumm->fSumm));
			}
		}
		/*
		for (i=0; i<nSamplesRead; i++)
		{
		float fVal = ReGetPPGValue(i);

		}
		*/
	}
	else
	{
		iArt = -2;
	}
}

float Fun_interface::Median(int Num, float a[])
{
	int Cnt = (int)((float)Num / 2.0 + 0.5);
	float b[5];
	int i;

	for (i = 0; i < Num; i++)
		b[i] = a[i];
	for (i = 0; i < Cnt; i++)
	{
		float fMin = b[i];
		int iMin = i;
		float fC;
		for (int j = i; j < Num; j++)
		{
			if (b[j] < fMin)
			{
				fMin = b[j];
				iMin = j;
			}
		}
		if (iMin != i)
		{
			fC = b[i];
			b[i] = b[iMin];
			b[iMin] = fC;
		}
	}
	return b[Cnt - 1];
}

void Fun_interface::Filter(CContrObArray<float, float> * fpS, float * Tail)
{
	static const float fQ[] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
	float Med;
	int i;

	if (iCurrentBuffer + nSamplesRead < BUF_SIZE)
	{
		for (i = 0; i < nSamplesRead; i++)
		{
			*Tail = *(Tail + 1);
			*(Tail + 1) = *(Tail + 2);
			*(Tail + 2) = fpS->GetAt(i + iCurrentBuffer);
			Med = Median(3, Tail);
			Tmp.SetAt(i, Med);
		}
		for (i = 0; i < nSamplesRead; i++)
			fpS->SetAt(i + iCurrentBuffer, Tmp[i]);
	}
	else
	{
		for (i = BUF_SIZE - nSamplesRead; i < BUF_SIZE; i++)
		{
			*Tail = *(Tail + 1);
			*(Tail + 1) = *(Tail + 2);
			*(Tail + 2) = fpS->GetAt(i);
			Med = Median(3, Tail);
			Tmp.SetAt(i - BUF_SIZE + nSamplesRead, Med);
		}
		for (i = BUF_SIZE - nSamplesRead; i < BUF_SIZE; i++)
			fpS->SetAt(i, Tmp[i - BUF_SIZE + nSamplesRead]);
	}
}

void Fun_interface::Calculation()
{
	//Filter(GetPPG(),TailRawPPG);
	Filter(&faPPG, TailRawPPG);

	nCurrentIBI = GetIBI(); // NEW VERSION
}

/////////////////////////////////////
BOOL Fun_interface::TakeIBI(int & fIBI,int Ind)
{
	if (faIBIArr.GetSize()>Ind)
	{
		fIBI = (int)faIBIArr[Ind];
		return TRUE;
	}
	return FALSE;
}

float Fun_interface::CalcHRT()
{
	float fRes;

	if (nCurrentIBI > 0)
	{
		fRes = (float)(60000 / nCurrentIBI);
	}
	else
	{
		fRes = 0.0f;
	}
	return fRes;
}

BOOL Fun_interface::CheckArtifact(int & IBI)
{
	BOOL bArtifact = FALSE;

	int nSum = 0;
	int min;
	int max;

	//ERR(3240)
	// make sure the array is full
	if (AvgAlen.GetSize() < ST_SETUP_ALEN)
	{
		min = ( int )( ST_SETUP_ABMIN * 1000 );
		max = ( int )( ST_SETUP_ABMAX * 1000 );

		if ( IBI < min || IBI > max )
		{
			bArtifact = TRUE;
		}
		else
		{
			AvgAlen.Add(IBI);
		}
	}
	else
	{
		nSum = 0;                        // calculate the average of non-
		// artifact IBIs
		for (int i = 0; i < AvgAlen.GetSize(); i++ )
		{
			nSum += AvgAlen[i];
		}
		nSum /= AvgAlen.GetSize();

		min = nSum - ST_SETUP_PERMIN * nSum / 100;
		max = nSum + ST_SETUP_PERMAX * nSum / 100;

		// if the current IBI is outside of
		// the range record it as an artifact
		// otherwise add it to the array
		if ( IBI < min || IBI > max )
		{
			bArtifact = TRUE;
		}
		else
		{
			AvgAlen.RemoveAt(0,1);
			AvgAlen.Add(IBI);
		}
	}

	return bArtifact;

}

void Fun_interface::DemeanDetrend(float *Y, int nSize)
{
	float m, b, r, sm, sb;
	int i;

	for (i = 0; i < nSize; i++)
		X[i] = (float)i;

	//LinFit(X, Y, &m, &b, &r, &sm, &sb, nSize);
	LinFit(X, Y, &m, &b, nSize);

	for (i = 0; i < nSize; i++)
	{
		Y[i] -= m * X[i] + b;
		Y[i] *= (float)(0.5 * (1 - cos(2 * PI * i / nSize)));
	}
}

int Fun_interface::GetPowOrder(int nVal)
{
	//ERR(3230)

	int nOrd = 0;
	int nPow2;

	do
	{
		nPow2 = 1 << nOrd;
		if (nVal == nPow2) break;
		nOrd++;
	}
	while(nPow2 <= DFT_BUF_SIZE);

	//ERR(3231)

	return nOrd;
}

void Fun_interface::CalcPSD(float *Y, BOOL bDemean )
{
	long NOBS = MIN_IBI_COUNT;
	long NF	= NOBS / 2;
	int i;

	if ( bDemean )
		DemeanDetrend( Y, NOBS );

	complx16 xx[MIN_IBI_COUNT];
	for (i = 0; i < NOBS; i++)
	{
		xx[i].r = Y[i];
		xx[i].i = 0;
	}
	float fB = 0.0f;
	int MM = GetPowOrder(NOBS);

	//rfftc(&xx[0], MM);
	float xbuffer[MIN_IBI_COUNT ];
	for (i = 0; i < NOBS; i++)
	{
		xbuffer[i] = xx[i].r;
	}
	rfft(xbuffer, NOBS, MM);
	for (i = 0; i < NF; i++)
	{
		if(i == 0)
		{
			xx[i].r = xbuffer[0];
			xx[i].i = 0;
		}
		else
		{
			xx[i].r = xbuffer[i];
			xx[i].i = xbuffer[NOBS - i];
		}
	}

	for (i = 0; i < NF; i++)
	{
		float fM = (float)(xx[i].r * xx[i].r + xx[i].i * xx[i].i);
		Y[i] = fM;
	}
}

void Fun_interface::CalcDFT()
{
	float Y[MIN_IBI_COUNT];
	float Y2[MIN_IBI_COUNT];

	long NOBS = MIN_IBI_COUNT;	// NOBS == 128
	long NF	= NOBS / 2;
	int i,j;

	int nSize = faIBI.GetSize();
	if (nSize < 40)					// Start the Power Spectrum after 20 seconds
		return;

	if ( nSize < NOBS )
	{
		for (i=0; i < nSize; i++)
			Y2[i] = faIBI[i];
	}
	else
	{
		for (i = nSize - NOBS, j = 0; i < nSize; i++, j++)
			Y2[j] = faIBI[i];
	}

	DemeanDetrend( Y2, min(nSize, NOBS) );

	if (nSize < NOBS)				// Employing Zero padding here
	{
		memset(Y, '\0', MIN_IBI_COUNT * sizeof(float));	//pjf - clear the entire array
		int nLeadLength = (NOBS - nSize) / 2;
		for (i = nLeadLength, j = 0; j < nSize; i++, j++)
			Y[i] = Y2[j];			//pjf - copy partial data to the center of the array
	}
	else
	{
		for (i = 0; i < NOBS; i++)
			Y[i] = Y2[i];
	}

	CalcPSD( Y, 0 );				//pjf - don't do demean/detrend again...

	for (i = 0; i < NF; i++)
		faSpectrumIBI[i] = Y[i];
	bReadOutIBI = true;

}

#define SMOOTH_5 5

float Fun_interface::FilterPoint(float x)
{
	double y;

	static float fTmp[SMOOTH_5];
	static int iCurrent = 0;
	static BOOL bEnd = false;
	fTmp[iCurrent++] = x;
	if (iCurrent >= SMOOTH_5)
	{
		iCurrent = 0;
		bEnd = true;
	}
	if (bEnd)
	{
		float fSum = 0;
		for (int j = 0; j < SMOOTH_5; j++)
		{
			fSum += fTmp[j];
		}
		y = fSum / SMOOTH_5;
	}
	else
		y = x;

	return (float)y;
}

BOOL Fun_interface::StoreIBI(int iIBI)
{
	static BOOL bInLoop = FALSE;
	BOOL  bCalcFFT = TRUE;
	static int iArtCount = 0;
	nCurrentIBI = iIBI;
	int i;

	if (nCurrentIBI == -1)
		return FALSE;

	faIBI.Add((float)nCurrentIBI);

	float fHRT = (float)CalcHRT() ;//+ 10; // Why ?...

	/*
	if (HRTSumm.Tick(fHRT))
	{
	fMeanHRT = HRTSumm.fSumm;	//pjf cnt - for bar display only
	}
	*/
	fHRT = FilterPoint(fHRT);
	////////add by db/////////////////////
	ReturnHRV.Add(fHRT);
	nCurrentHRT = fHRT;
	//////////////////////////////////////
	faHRT.Add(fHRT);
	BOOL bArt = FALSE;
	iArt = -1;

	if (nGlobalCountIBI >= 40) // for Spec no 6 ;40 samples will be obtained in 20 seconds

	{
		// Artifact Calc Starts

		bArt = CheckArtifact(nCurrentIBI);
		if(bArt)
		{
			iArt = 1;
			iArtTotal ++;
		}
		else
			iArt = 0;

		if (bArt)
		{
			faHRTArtifact.Add(1);
			iArtCount++;
			if (iArtCount > ST_SETUP_NUMEVENTS)
				bCalcFFT = false;
		}
		else
		{
			iArtCount = 0;
			for (i = faHRTArtifact.GetSize() - 1; i >= 0; i--)
			{
				if (!faHRTArtifact[i]) break;
			}
			/////用内插替换出伪信号
			if (i >= 0)
			{
				float Delta = (float)(faIBI[faIBI.GetSize() - 1] - faIBI[i]) / (faHRTArtifact.GetSize() - i);
				for (int j = i + 1; j < faHRTArtifact.GetSize(); j++)
				{
					faIBI[j] = faIBI[j - 1] + Delta;		//500;//
					//	faHRT[j] = (float)(60000 / faIBI[j]);
				}
			}
			faHRTArtifact.Add(0);
		}

		int nSum = 0;
		if (faHRTArtifact.GetSize() >= ST_SETUP_LASTTIME * SAMPLE_RATE_IBI && bCalcFFT)
		{
			for (int i = faHRTArtifact.GetSize() - ST_SETUP_LASTTIME * SAMPLE_RATE_IBI; i < faHRTArtifact.GetSize(); i++)
			{
				if (faHRTArtifact[i])
					nSum++;
			}
			if (nSum > ST_SETUP_LASTEVENTS)
				////不再计算EP,将其置0
			{
				bCalcFFT = false;
			}
			else bCalcFFT = true;
		}
		// Artifact Calc Ends

		// Start Recording the Data
		if ((++cTRGCount % TRG_UPDATE_RATE) == 0)	//pjf cmnt: TRG_UPDATE_RATE == 10
		{
			CalcDFT();

			//CalcTargetPeak();

			if (pTRGCalc != NULL)
			{
				if (bCalcFFT) // When CalcFFT is Enabled / disabled
				{
					// !!! Produces zeros when nMinSamples < 64
					// also starts sound playing when appropriate
					fTargetFFT = pTRGCalc->CalcTarget(&faIBI, Score, PreScore);
					fTargetFFTRising = pTRGCalc->AScoreRising;
					fTargetFFTTop = pTRGCalc->ThresholdTop;
					fTargetFFTStd = pTRGCalc->ThresholdStd;
					bInLoop = TRUE;
				}
				else
				{
					Score = 0;
					if (bInLoop == false)
						PreScore = 0;
					if (PreScore == 0)
					{
						pTRGCalc->AScore += ST_SETUP_INC3;
					}
					else
					{
						if (PreScore == 1)
						{
							pTRGCalc->AScore += ST_SETUP_INC6;
						}
						else
						{
							pTRGCalc->AScore += ST_SETUP_INC9;
						}
						if (pTRGCalc->AScore < 0)
							pTRGCalc->AScore = 0;
						if (pTRGCalc->AScore > 100)
							pTRGCalc->AScore = 100;
						fTargetFFT = pTRGCalc->AScore;
						/////////////////////////////////////////////////////
						pTRGCalc->ThresholdTop += 2;
						pTRGCalc->ThresholdStd += 1;
						fTargetFFTTop = pTRGCalc->ThresholdTop;
						fTargetFFTStd = pTRGCalc->ThresholdStd;

						bInLoop = false;
						////不再计算EP,将其置0
						pTRGCalc->CurEp = 0;
					}
				}
				bReadOutEP = true;
				ReturnEP.Add(pTRGCalc->CurEp);
			}
		}
	}
	else
	{
		faHRTArtifact.Add(0);
	}
	faTRG.Add(fTargetFFT);

	nGlobalCountIBI++;

	return TRUE;
}

/////////////////////////////////////
void Fun_interface::CloseIBI()
{
	faIBIArr.SetSize(0, 10);
}

void *TimerAction(void *arg)
{
	if ( Fun_interface::Instance()->bBlock )
		return (void*)0;
	Fun_interface::Instance()->bBlock = TRUE;
	if ( arg == NULL )
	{
		return (void*)0;
	}
	else
	{
		// arg points to the argument; in this case it is an int
		int event = *((int *)arg);
		switch (event)
		{
			case ID_PPG_TIMER:	//pjf cmnt: 100(55) ms period
				Fun_interface::Instance()->ReadDataFromDriver();
				Fun_interface::Instance()->Calculation();
				break;
			case ID_SYS_TIMER:	//pjf cmnt: 500 Ms period
				int fIBI;
				int i;
				for ( i = 0; Fun_interface::Instance()->TakeIBI(fIBI, i); i++ )
				{
					Fun_interface::Instance()->StoreIBI(fIBI);
				}
				Fun_interface::Instance()->CloseIBI();	// clears the faIBIArr array
				if ( i > 0 )
				{
					Fun_interface::Instance()->bReadOutHRV = true;
				}
				else
				{
					Fun_interface::Instance()->bReadOutHRV = false;
				}
				break;
		}
	}

	Fun_interface::Instance()->bBlock = FALSE;

	return (void*)1;
}

BOOL Fun_interface::ReadData()
{
	//CTimer ppgTimer(TimerAction, 1, 0, 100, &arg_ppg);
	//CTimer sysTimer(TimerAction, 1, 0, 500, &arg_sys);
	hTimerPPG = new CTimer(TimerAction, 1, 0, 100, &arg_ppg);
	hTimerSYS = new CTimer(TimerAction, 1, 0, 500, &arg_sys);
	hTimerPPG.StartTimer();
	hTimerSYS.StartTimer();

//	struct timeval tempval;
//	tempval.tv_sec = 0;
//	tempval.tv_usec = 1;
//	select(0, NULL, NULL, NULL, &tempval);
//
//	int res;
//	res = pthread_create(&m_hTimerThread, NULL, TimerAction, this);
//	if (res != 0) {
//		return FALSE;
//	}

//	hTimerQueue = CreateTimerQueue();
//	if (!hTimerQueue)
//	{
//		return FALSE;
//	}

//	if (!CreateTimerQueueTimer(
//		&hTimerPPG, hTimerQueue, TimerAction, &arg_ppg , 1, PPG_TICK, 0))
//	{
//		return FALSE;
//	}
//
//	// SYS_TICK = 500( ms )
//	if (!CreateTimerQueueTimer(
//		&hTimerSYS, hTimerQueue, TimerAction, &arg_sys , 1, SYS_TICK, 0))
//	{
//		return FALSE;
//	}

	return TRUE;
}
