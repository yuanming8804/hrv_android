#include "com_haofengkeji_hdread_MainActivity.h"
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include "HDReadInterface.h"

JNIEnv* m_pEnv = NULL;		// NDK对象
jobject m_jObj;				// 应用程序实例

JNIEXPORT jboolean JNICALL Java_com_haofengkeji_hdread_MainActivity_Start
  (JNIEnv * env, jobject obj)
{
	jboolean isStartDevice = JNI_FALSE;

	m_pEnv = env;
	m_jObj = obj;

//	jclass clazz = env->FindClass("com/haofengkeji/hdread/MainActivity");
//	jmethodID mid = env->GetMethodID(clazz, "startDevice", "()V");
//	if (mid == NULL)
//		return isStartDevice;
//	env->CallVoidMethod(obj, mid);

	if ( !CHDRead::Instance()->StartDevice_() )
	{
		return isStartDevice;
	}
	if ( !CHDRead::Instance()->StartRead_() )
	{
		return isStartDevice;
	}

	return isStartDevice;
}

JNIEXPORT void JNICALL Java_com_haofengkeji_hdread_MainActivity_Stop
  (JNIEnv * env, jobject obj)
{
	//关闭硬件设备
	CHDRead::Instance()->StopDevice_();
}

JNIEXPORT void JNICALL Java_com_haofengkeji_hdread_MainActivity_SetInputReport
  (JNIEnv * env, jobject obj, jbyteArray byteArray)
{
	CHDRead::Instance()->SetInputReport(byteArray);
}
