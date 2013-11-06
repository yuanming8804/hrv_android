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
	
	//设置当前测量难度
	public native void SetCurDiff_(int diff);
	//获得当前测量难度
	public native int GetCurDiff_();
	//设置难度上下限
	public native boolean SetDiffRange_(int diff, float up, float down);
	//获得难度上下限
	public native boolean GetDiffRange_(int diff, float up, float down);
	//获得在当前健康状态
	public native int GetHealtState_();
	//获得即时脉博
	public native float GetRawPPG_(int nIdx);
	//脉博数据是否有数据读取
	public native void SetReadPPG_(boolean bRead);
	public native boolean GetReadPPG_();
	//HRV是否有数据读取
	public native void SetReadHRV_(boolean bRead);
	public native boolean GetReadHRV_();
	//EP是否有数据读取
	public native void SetReadEP_(boolean bRead);
	public native boolean GetReadEP_();
	//频谱是否有数据读取
	public native void SetReadIBI_(boolean bRead);
	public native boolean GetReadIBI_();
	//设置阻塞标志
	public native void SetBlockFlag_(boolean bBlockFlag);
	//得到阻塞标志
	public native boolean GetBlockFlag_();


}
