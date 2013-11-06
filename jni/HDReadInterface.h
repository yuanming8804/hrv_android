#pragma once
//这个类用于管理设备对外的接口
#include "DrvInterface.h"
#include "Fun_Interface.h"
//#ifndef DEFAULT_CHREAD
//#define DEFAULT_CHREAD

class CHDRead
{
public:
	CHDRead(void);
	~CHDRead(void);
	static CHDRead* Instance();

	//设置当前测量难度
	void SetCurDiff_(const int& diff);
	//获得当前测量难度
	int GetCurDiff_();
	//设置难度上下限
	bool SetDiffRange_(const int& diff, const float& up, const float& down);
	//获得难度上下限
	bool GetDiffRange_(const int& diff, float& up, float& down);
	//获得在当前健康状态
	int GetHealtState_();

	//获得即时脉博
	float GetRawPPG_(int nIdx);
	//脉博数据是否有数据读取
	void SetReadPPG_(bool bRead);
	bool GetReadPPG_();
	//HRV是否有数据读取
	void SetReadHRV_(bool bRead);
	bool GetReadHRV_();
	//EP是否有数据读取
	void SetReadEP_(bool bRead);
	bool GetReadEP_();
	//频谱是否有数据读取
	void SetReadIBI_(bool bRead);
	bool GetReadIBI_();
	//设置阻塞标志
	void SetBlockFlag_(bool bBlockFlag);
	//得到阻塞标志
	bool GetBlockFlag_();
	//得到脉博数据
	float * GetPackPPG_(int &size);
	////获得当前脉博函数指针
	//float GetCurPPG_();
	//获得当前HRV函数指针
	float* GetPackHrt_(int& size);
	//获得当前EP函数指针
	float* GetPackEP_(int& size);
	//获得当前频谱函数指针
	float * GetPackIBI_(int & size);
	//获得当前分数
	int GetCurAScore_();

	//获得当前EP
	float GetCurEp_();
	//获得当前新的数据
	int GetSamplesRead_();
	//开启设备
	BOOL StartDevice_();
	//停止设备
	BOOL StopDevice_();
	BOOL ReadData_();
	//开始读取数据
	BOOL StartRead_();
	//得到设备状态
	int GetArtifactStatus_();

	//得到当前分数信息
	int GetCurScore_();
	int GetCurAScoreRising_();
	int GetAScoreTop_();
	int GetAScoreStd_();
	//得到设备个数
	int GetArtifactCount_();
	//数据是否保存
	bool IsDataSaved();
	void SetDataSaved(bool save);
	//得到IBI数据个数
	int GetIBICount(){ return Fun_interface::Instance()->GetIBICount(); };

	void SetCallbacks(pNewScore ScoreFunction, pNewPPG PPGFunction, pNewIBI IBIFunction, pError ErrFunction);

private:
	DIFFRANGE DiffARRAY[DIFFLEVEL];
	int CurDiff;
	bool m_bGameData;
};
//#endif
