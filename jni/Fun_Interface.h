#include "Drv.h"
#include "array.h"
#include "DrvM801Usb.h"
#include "Summ.h"
#include "Trgcalc.h"

//#include <sys/time.h>
//#include <linux/rtc.h>
//#include <sys/ioctl.h>
//#include <pthread.h>

///////////////////////////////////
template class CContrObArray<int, int>;
template class CContrObArray<float, float>;
template class CContrObArray<BYTE, BYTE>;

///////CALLBACKS////////////////////////
typedef int (*pNewScore)(int iScore, int iAScore, float fEP, int iSessionTime);
typedef int (*pNewPPG)(int iPPG);
typedef int (*pNewIBI)(int iIBI);
typedef int (*pError)(int iError);

//#ifndef DEFAULT_FUN_INTERFACE
//#define DEFAULT_FUN_INTERFACE
//#define INNER_FUN Fun_interface::Instance()
#define DIFFLEVEL 4
struct DIFFRANGE
{
	float UP;
	float DOWN;
};


class Fun_interface
{
public:
	Fun_interface(void);
	~Fun_interface(void);
	static Fun_interface* Instance();
private:
	CDrvM801 * pDrv;

//	HANDLE hTimerPPG;
//	HANDLE hTimerSYS;
	//HANDLE hTimerQueue;
	//pthread_t m_hTimerThread;

	CTimer hTimerPPG;
	CTimer hTimerSYS;

	int nSamplesRead;
	/////////////////////
	int nTotalSamples;
	bool bBlock;
	/////////////////////
	int iLastIBI;
	int nCurrentIBI;
	float nCurrentHRT;
	int nGlobalCountIBI;
	int cTRGCount;
	float fTargetFFT;

	float fTargetFFTRising;
	float fTargetFFTTop;
	float fTargetFFTStd;

	float Score, PreScore;
	int arg_ppg;
	int arg_sys;

	float TailRawPPG[5];
	CContrObArray<int, int> AvgAlen;
	float * X;
	bool bReadOutPPG;	//脉博数据是否有数据读取
	bool bReadOutHRV;	//HRV是否有数据读取
	bool bReadOutEP;	//EP是否有数据读取
	bool bReadOutIBI;	//IBI是否有数据读取
	int iCurrentBuffer;

	///////Artifact/////////
	int iArt;
	int iArtTotal;
	////////////////////////

	CSumm *IBISumm;
	CContrObArray<float, float> faPPG;
	CContrObArray<float, float> faIBI;
	CContrObArray<float, float> faHRT;
	CContrObArray<float, float> faTRG;
	CContrObArray<float, float> faSpectrumIBI;
	CContrObArray<BYTE, BYTE>	faHRTArtifact;
	CContrObArray<int, int>		faIBIArr;
	CContrObArray<float, float> Tmp;

	CTRGCalc * pTRGCalc;

	CContrObArray<float, float> ReturnHRV;
	CContrObArray<float, float> ReturnEP;
	float *pfTemp;
	int	  *piTemp;

	DIFFRANGE DiffARRAY[DIFFLEVEL];
	int CurDiff;
	bool inited;

public:
	CContrObArray<float, float> * GetFFT();
	////////////////////////////////////////////////////////////////////////
	//设置当前测量难度
	void SetCurDiff(const int& diff);
	//获得当前测量难度
	int GetCurDiff();
	//设置难度上下限
	bool SetDiffRange(const int& diff, const float& up, const float& down);
	//获得难度上下限
	bool GetDiffRange(const int& diff, float& up, float& down);
	//获得在当前难度的状态
	int GetHealtState();

	//获得即时脉博
	float GetRawPPG(int nIdx);
	//脉博数据是否有数据读取
	void SetReadPPG(bool bRead);
	bool GetReadPPG();
	//HRV是否有数据可以读取
	void SetReadHRV(bool bRead);
	bool GetReadHRV();
	//EP是否有数据可以读取
	void SetReadEP(bool bRead);
	bool GetReadEP();
	//IBI是否有数据可以读取
	void SetReadIBI(bool bRead);
	bool GetReadIBI();
	//设置阻塞标志
	void SetBlockFlag(bool bBlockFlag);
	//得到阻塞标志
	bool GetBlockFlag();
	//得到脉博数据
	float * GetPackPPG(int &size);
	////获得当前脉博函数指针
	//float GetCurPPG();

	//获得当前HRV函数指针
	float* GetPackHrt(int &size);
	//得到EP数据
	float * GetPackEP(int &size);
	//获得当前频谱函数指针
	float* GetCurIBI(int &size);
	//获得当前分数
	int GetCurAScore();
	//获得当前EP
	float GetCurEp();
	//获得当前新的数据
	int GetSamplesRead();
	//开启设备
	BOOL StartDevice();
	//停止设备
	BOOL StopDevice();
	BOOL ReadData();
	//开始读取数据
	BOOL StartRead();
	int GetCurScore();
	int GetCurAScoreRising();
	int GetAScoreTop();
	int GetAScoreStd();
	//得到设备状态
	int GetArtifactStatus();
	//得到设备个数
	int GetArtifactCount();
	//得到IBI数据个数
	int GetIBICount(){ return nGlobalCountIBI; };
	////////////////////////////////////////////////////////////////////////
public:
	BOOL Initialize();
	void ExInitialize();
	void Filter(CContrObArray<float, float> * fpS, float * Tail);
	BOOL TakeIBI(int & fIBI, int Ind);
	float CalcHRT();
	BOOL CheckArtifact(int & IBI);
	void DemeanDetrend(float *Y, int nSize);
	int GetPowOrder(int nVal);
	void CalcPSD(float *Y, BOOL bDemean );
	void CalcDFT();
	float FilterPoint(float x);
	BOOL StoreIBI(int iIBI);
	void CloseIBI();
	void ReadDataFromDriver();
	void Calculation();
	float ReGetPPGValue(int nIndex);
	BOOL ResumeDriver();
	BOOL PauseDriver();
	BOOL DisableDriver();
	BOOL EnableDriver(LPCSTR pPortName);
	int PacksInExtBuf(int channelIndex);
	float Median(int Num, float a[]);
	float GetRawTMP(int nIdx);
	float GetRawGSR(int nIdx);
	int GetPPGValue(int nIdx);
	int GetIBIValue(int Ind);
	int GetIBI();
	int GetDataAvailable();

	//friend VOID TimerAction(PVOID lpParam, BOOLEAN TimerOrWaitFired);
	friend void *TimerAction(void *arg);
};
//VOID TimerAction(PVOID lpParam, BOOLEAN TimerOrWaitFired);
void *TimerAction(void *arg);
//#endif
