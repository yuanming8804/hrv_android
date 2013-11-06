#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include "HDReadInterface.h"

JNIEXPORT jboolean JNICALL Java_com_haofengkeji_hdread_MainActivity_Start
  (JNIEnv * env, jobject obj)
{
	if( ! CHDRead::Instance()->StartDevice_() )
	{
		return false;
	}
	if( ! CHDRead::Instance()->StartRead_() )
	{
		return false;
	}

	return true;
}

JNIEXPORT void JNICALL Java_com_haofengkeji_hdread_MainActivity_Stop
  (JNIEnv * env, jobject obj)
{
	//关闭硬件设备
	CHDRead::Instance()->StopDevice_();
}
