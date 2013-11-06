package com.haofengkeji.hdread;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;

public class MainActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
	}
	
	static {
        System.loadLibrary("HDRead");
    }
	
	//���õ�ǰ�����Ѷ�
	public native void SetCurDiff_(int diff);
	//��õ�ǰ�����Ѷ�
	public native int GetCurDiff_();
	//�����Ѷ�������
	public native boolean SetDiffRange_(int diff, float up, float down);
	//����Ѷ�������
	public native boolean GetDiffRange_(int diff, float up, float down);
	//����ڵ�ǰ����״̬
	public native int GetHealtState_();
	//��ü�ʱ����
	public native float GetRawPPG_(int nIdx);
	//���������Ƿ������ݶ�ȡ
	public native void SetReadPPG_(boolean bRead);
	public native boolean GetReadPPG_();
	//HRV�Ƿ������ݶ�ȡ
	public native void SetReadHRV_(boolean bRead);
	public native boolean GetReadHRV_();
	//EP�Ƿ������ݶ�ȡ
	public native void SetReadEP_(boolean bRead);
	public native boolean GetReadEP_();
	//Ƶ���Ƿ������ݶ�ȡ
	public native void SetReadIBI_(boolean bRead);
	public native boolean GetReadIBI_();
	//����������־
	public native void SetBlockFlag_(boolean bBlockFlag);
	//�õ�������־
	public native boolean GetBlockFlag_();


}
