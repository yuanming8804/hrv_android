#include "PreInclude.h"
#include "HDReadInterface.h"

//////////CALLBACK////////////////
pNewScore NewScore = NULL;
pNewPPG NewPPG = NULL;
pNewIBI NewIBI = NULL;
pError Error = NULL;

CHDRead::CHDRead(void)
{
}

CHDRead::~CHDRead(void)
{
}

CHDRead* CHDRead::Instance()
{
//	if ( g_bHReaderInit == false )
//	{
//		static CHDRead HRead;
//		int *add = (int*)g_HReaderAddr;
//		add = (int*)&HRead;
//		g_bHReaderInit = true;
//	}
	static CHDRead instance;
	return &instance;
	//return (CHDRead*)(g_HReaderAddr);
}

//设置当前测量难度
void CHDRead::SetCurDiff_(const int &diff)
{
	CurDiff = diff;
	return Fun_interface::Instance()->SetCurDiff(diff);
}

//获得当前测量难度
int CHDRead::GetCurDiff_()
{
	return CurDiff;
	//return Fun_interface::Instance()->GetCurDiff();
}

//设置难度上下限
bool CHDRead::SetDiffRange_(const int& diff, const float& up, const float& down)
{
	DiffARRAY[diff].UP = up;
	DiffARRAY[diff].DOWN = down;
	return Fun_interface::Instance()->SetDiffRange(diff, up, down);
}

//获得难度上下限
bool CHDRead::GetDiffRange_(const int& diff,float& up,float& down)
{
	if ( diff >= 0 && diff <= DIFFLEVEL )
	{
		up = DiffARRAY[diff].UP;
		down = DiffARRAY[diff].DOWN;
		return true;
	}
	return false;
	//return Fun_interface::Instance()->GetDiffRange(diff,up,down);
}

//获得在当前健康状态
int CHDRead::GetHealtState_()
{
	return Fun_interface::Instance()->GetHealtState();
}

//获得即时脉博
float CHDRead::GetRawPPG_(int nIdx)
{
	return Fun_interface::Instance()->GetRawPPG(nIdx);
}

//脉博数据是否有数据读取
void CHDRead::SetReadPPG_(bool bRead)
{
	Fun_interface::Instance()->SetReadPPG(bRead);
}

bool CHDRead::GetReadPPG_()
{
	return Fun_interface::Instance()->GetReadPPG();
}

//HRV是否有数据读取
void CHDRead::SetReadHRV_(bool bRead)
{
	Fun_interface::Instance()->SetReadHRV(bRead);
}

bool CHDRead::GetReadHRV_()
{
	return Fun_interface::Instance()->GetReadHRV();
}

//EP是否有数据读取
void CHDRead::SetReadEP_(bool bRead)
{
	Fun_interface::Instance()->SetReadEP(bRead);
}

bool CHDRead::GetReadEP_()
{
	return Fun_interface::Instance()->GetReadEP();
}

//频谱是否有数据读取
void CHDRead::SetReadIBI_(bool bRead)
{
	Fun_interface::Instance()->SetReadIBI(bRead);
}

bool CHDRead::GetReadIBI_()
{
	return Fun_interface::Instance()->GetReadIBI();
}

//设置阻塞标志
void CHDRead::SetBlockFlag_(bool bBlockFlag)
{
	Fun_interface::Instance()->SetBlockFlag(bBlockFlag);
}

//得到阻塞标志
bool CHDRead::GetBlockFlag_()
{
	return Fun_interface::Instance()->GetBlockFlag();
}

float * CHDRead::GetPackPPG_(int &size)
{
	return Fun_interface::Instance()->GetPackPPG(size);
}

////获得当前脉博函数指针
//float CHDRead::GetCurPPG_(){}
//获得当前HRV函数指针
float* CHDRead::GetPackHrt_(int& size)
{
	return Fun_interface::Instance()->GetPackHrt(size);
}

//获得当前EP函数指针
float* CHDRead::GetPackEP_(int& size)
{
	return Fun_interface::Instance()->GetPackEP(size);
}

//获得当前频谱函数指针
float * CHDRead::GetPackIBI_(int & size)
{
	return Fun_interface::Instance()->GetCurIBI(size);
}

//获得当前分数
int CHDRead::GetCurAScore_()
{
	return Fun_interface::Instance()->GetCurAScore();
}

//获得当前EP
float CHDRead::GetCurEp_()
{
	return Fun_interface::Instance()->GetCurEp();
}

//获得当前新的数据
int CHDRead::GetSamplesRead_()
{
	return Fun_interface::Instance()->GetSamplesRead();
}

//开启设备
BOOL CHDRead::StartDevice_()
{
	return Fun_interface::Instance()->StartDevice();
}

//停止设备
BOOL CHDRead::StopDevice_()
{
	return Fun_interface::Instance()->StopDevice();
}

BOOL CHDRead::ReadData_()
{
	return Fun_interface::Instance()->ReadData();
}

//开始读取数据
BOOL CHDRead::StartRead_()
{
	return Fun_interface::Instance()->StartRead();
}

int CHDRead::GetCurScore_()
{
	return Fun_interface::Instance()->GetCurScore();
}

int CHDRead::GetCurAScoreRising_()
{
	return Fun_interface::Instance()->GetCurAScoreRising();
}

int CHDRead::GetAScoreTop_()
{
	return Fun_interface::Instance()->GetAScoreTop();
}

int CHDRead::GetAScoreStd_()
{
	return Fun_interface::Instance()->GetAScoreStd();
}

//得到设备状态
int CHDRead::GetArtifactStatus_()
{
	return Fun_interface::Instance()->GetArtifactStatus();
}

//得到设备个数
int CHDRead::GetArtifactCount_()
{
	return Fun_interface::Instance()->GetArtifactCount();
}

bool CHDRead::IsDataSaved()
{
	return m_bGameData;
}

void CHDRead::SetDataSaved(bool save)
{
	m_bGameData = save;
}

void CHDRead::SetCallbacks(pNewScore ScoreFunction, pNewPPG PPGFunction, pNewIBI IBIFunction, pError ErrFunction)
{
	NewScore = ScoreFunction;
	NewPPG = PPGFunction;
	NewIBI = IBIFunction;
	Error = ErrFunction;
}

//void CHDRead::SetInputReport(jbyteArray byteArray)
//{
//	Fun_interface::Instance()->SetInputReport(byteArray);
//}
