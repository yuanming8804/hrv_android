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

//���õ�ǰ�����Ѷ�
void CHDRead::SetCurDiff_(const int &diff)
{
	CurDiff = diff;
	return Fun_interface::Instance()->SetCurDiff(diff);
}

//��õ�ǰ�����Ѷ�
int CHDRead::GetCurDiff_()
{
	return CurDiff;
	//return Fun_interface::Instance()->GetCurDiff();
}

//�����Ѷ�������
bool CHDRead::SetDiffRange_(const int& diff, const float& up, const float& down)
{
	DiffARRAY[diff].UP = up;
	DiffARRAY[diff].DOWN = down;
	return Fun_interface::Instance()->SetDiffRange(diff, up, down);
}

//����Ѷ�������
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

//����ڵ�ǰ����״̬
int CHDRead::GetHealtState_()
{
	return Fun_interface::Instance()->GetHealtState();
}

//��ü�ʱ����
float CHDRead::GetRawPPG_(int nIdx)
{
	return Fun_interface::Instance()->GetRawPPG(nIdx);
}

//���������Ƿ������ݶ�ȡ
void CHDRead::SetReadPPG_(bool bRead)
{
	Fun_interface::Instance()->SetReadPPG(bRead);
}

bool CHDRead::GetReadPPG_()
{
	return Fun_interface::Instance()->GetReadPPG();
}

//HRV�Ƿ������ݶ�ȡ
void CHDRead::SetReadHRV_(bool bRead)
{
	Fun_interface::Instance()->SetReadHRV(bRead);
}

bool CHDRead::GetReadHRV_()
{
	return Fun_interface::Instance()->GetReadHRV();
}

//EP�Ƿ������ݶ�ȡ
void CHDRead::SetReadEP_(bool bRead)
{
	Fun_interface::Instance()->SetReadEP(bRead);
}

bool CHDRead::GetReadEP_()
{
	return Fun_interface::Instance()->GetReadEP();
}

//Ƶ���Ƿ������ݶ�ȡ
void CHDRead::SetReadIBI_(bool bRead)
{
	Fun_interface::Instance()->SetReadIBI(bRead);
}

bool CHDRead::GetReadIBI_()
{
	return Fun_interface::Instance()->GetReadIBI();
}

//����������־
void CHDRead::SetBlockFlag_(bool bBlockFlag)
{
	Fun_interface::Instance()->SetBlockFlag(bBlockFlag);
}

//�õ�������־
bool CHDRead::GetBlockFlag_()
{
	return Fun_interface::Instance()->GetBlockFlag();
}

float * CHDRead::GetPackPPG_(int &size)
{
	return Fun_interface::Instance()->GetPackPPG(size);
}

////��õ�ǰ��������ָ��
//float CHDRead::GetCurPPG_(){}
//��õ�ǰHRV����ָ��
float* CHDRead::GetPackHrt_(int& size)
{
	return Fun_interface::Instance()->GetPackHrt(size);
}

//��õ�ǰEP����ָ��
float* CHDRead::GetPackEP_(int& size)
{
	return Fun_interface::Instance()->GetPackEP(size);
}

//��õ�ǰƵ�׺���ָ��
float * CHDRead::GetPackIBI_(int & size)
{
	return Fun_interface::Instance()->GetCurIBI(size);
}

//��õ�ǰ����
int CHDRead::GetCurAScore_()
{
	return Fun_interface::Instance()->GetCurAScore();
}

//��õ�ǰEP
float CHDRead::GetCurEp_()
{
	return Fun_interface::Instance()->GetCurEp();
}

//��õ�ǰ�µ�����
int CHDRead::GetSamplesRead_()
{
	return Fun_interface::Instance()->GetSamplesRead();
}

//�����豸
BOOL CHDRead::StartDevice_()
{
	return Fun_interface::Instance()->StartDevice();
}

//ֹͣ�豸
BOOL CHDRead::StopDevice_()
{
	return Fun_interface::Instance()->StopDevice();
}

BOOL CHDRead::ReadData_()
{
	return Fun_interface::Instance()->ReadData();
}

//��ʼ��ȡ����
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

//�õ��豸״̬
int CHDRead::GetArtifactStatus_()
{
	return Fun_interface::Instance()->GetArtifactStatus();
}

//�õ��豸����
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
