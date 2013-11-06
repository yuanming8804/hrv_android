#pragma once
//��������ڹ����豸����Ľӿ�
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

	//���õ�ǰ�����Ѷ�
	void SetCurDiff_(const int& diff);
	//��õ�ǰ�����Ѷ�
	int GetCurDiff_();
	//�����Ѷ�������
	bool SetDiffRange_(const int& diff, const float& up, const float& down);
	//����Ѷ�������
	bool GetDiffRange_(const int& diff, float& up, float& down);
	//����ڵ�ǰ����״̬
	int GetHealtState_();

	//��ü�ʱ����
	float GetRawPPG_(int nIdx);
	//���������Ƿ������ݶ�ȡ
	void SetReadPPG_(bool bRead);
	bool GetReadPPG_();
	//HRV�Ƿ������ݶ�ȡ
	void SetReadHRV_(bool bRead);
	bool GetReadHRV_();
	//EP�Ƿ������ݶ�ȡ
	void SetReadEP_(bool bRead);
	bool GetReadEP_();
	//Ƶ���Ƿ������ݶ�ȡ
	void SetReadIBI_(bool bRead);
	bool GetReadIBI_();
	//����������־
	void SetBlockFlag_(bool bBlockFlag);
	//�õ�������־
	bool GetBlockFlag_();
	//�õ���������
	float * GetPackPPG_(int &size);
	////��õ�ǰ��������ָ��
	//float GetCurPPG_();
	//��õ�ǰHRV����ָ��
	float* GetPackHrt_(int& size);
	//��õ�ǰEP����ָ��
	float* GetPackEP_(int& size);
	//��õ�ǰƵ�׺���ָ��
	float * GetPackIBI_(int & size);
	//��õ�ǰ����
	int GetCurAScore_();

	//��õ�ǰEP
	float GetCurEp_();
	//��õ�ǰ�µ�����
	int GetSamplesRead_();
	//�����豸
	BOOL StartDevice_();
	//ֹͣ�豸
	BOOL StopDevice_();
	BOOL ReadData_();
	//��ʼ��ȡ����
	BOOL StartRead_();
	//�õ��豸״̬
	int GetArtifactStatus_();

	//�õ���ǰ������Ϣ
	int GetCurScore_();
	int GetCurAScoreRising_();
	int GetAScoreTop_();
	int GetAScoreStd_();
	//�õ��豸����
	int GetArtifactCount_();
	//�����Ƿ񱣴�
	bool IsDataSaved();
	void SetDataSaved(bool save);
	//�õ�IBI���ݸ���
	int GetIBICount(){ return Fun_interface::Instance()->GetIBICount(); };

	void SetCallbacks(pNewScore ScoreFunction, pNewPPG PPGFunction, pNewIBI IBIFunction, pError ErrFunction);

private:
	DIFFRANGE DiffARRAY[DIFFLEVEL];
	int CurDiff;
	bool m_bGameData;
};
//#endif
