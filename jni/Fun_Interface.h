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
	bool bReadOutPPG;	//���������Ƿ������ݶ�ȡ
	bool bReadOutHRV;	//HRV�Ƿ������ݶ�ȡ
	bool bReadOutEP;	//EP�Ƿ������ݶ�ȡ
	bool bReadOutIBI;	//IBI�Ƿ������ݶ�ȡ
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
	//���õ�ǰ�����Ѷ�
	void SetCurDiff(const int& diff);
	//��õ�ǰ�����Ѷ�
	int GetCurDiff();
	//�����Ѷ�������
	bool SetDiffRange(const int& diff, const float& up, const float& down);
	//����Ѷ�������
	bool GetDiffRange(const int& diff, float& up, float& down);
	//����ڵ�ǰ�Ѷȵ�״̬
	int GetHealtState();

	//��ü�ʱ����
	float GetRawPPG(int nIdx);
	//���������Ƿ������ݶ�ȡ
	void SetReadPPG(bool bRead);
	bool GetReadPPG();
	//HRV�Ƿ������ݿ��Զ�ȡ
	void SetReadHRV(bool bRead);
	bool GetReadHRV();
	//EP�Ƿ������ݿ��Զ�ȡ
	void SetReadEP(bool bRead);
	bool GetReadEP();
	//IBI�Ƿ������ݿ��Զ�ȡ
	void SetReadIBI(bool bRead);
	bool GetReadIBI();
	//����������־
	void SetBlockFlag(bool bBlockFlag);
	//�õ�������־
	bool GetBlockFlag();
	//�õ���������
	float * GetPackPPG(int &size);
	////��õ�ǰ��������ָ��
	//float GetCurPPG();

	//��õ�ǰHRV����ָ��
	float* GetPackHrt(int &size);
	//�õ�EP����
	float * GetPackEP(int &size);
	//��õ�ǰƵ�׺���ָ��
	float* GetCurIBI(int &size);
	//��õ�ǰ����
	int GetCurAScore();
	//��õ�ǰEP
	float GetCurEp();
	//��õ�ǰ�µ�����
	int GetSamplesRead();
	//�����豸
	BOOL StartDevice();
	//ֹͣ�豸
	BOOL StopDevice();
	BOOL ReadData();
	//��ʼ��ȡ����
	BOOL StartRead();
	int GetCurScore();
	int GetCurAScoreRising();
	int GetAScoreTop();
	int GetAScoreStd();
	//�õ��豸״̬
	int GetArtifactStatus();
	//�õ��豸����
	int GetArtifactCount();
	//�õ�IBI���ݸ���
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
