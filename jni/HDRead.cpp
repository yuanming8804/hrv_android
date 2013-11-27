#include "com_haofengkeji_hdread_MainActivity.h"
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include "HDReadInterface.h"

JNIEnv* m_pEnv = NULL;		// NDK����
JavaVM* gs_jvm;  			// ȫ��Java�����
jobject gs_obj;				// java����
//jclass gs_clazz;			// ��ȡ��java��

jint JNI_OnLoad (JavaVM* vm, void* reserved)
{
    // Cache the JavaVM interface pointer
	gs_jvm = vm;
    return JNI_VERSION_1_6;
}

JNIEXPORT jboolean JNICALL Java_com_haofengkeji_hdread_MainActivity_Start
  (JNIEnv * env, jobject obj)
{
	jboolean isStartDevice = JNI_FALSE;

	m_pEnv = env;

	//gs_jvm = env->GetJavaVM(&gs_jvm);	// ��ȡJavaVMָ��
	gs_obj = env->NewGlobalRef(obj);
	//jclass localClazz = env->FindClass("com/haofengkeji/hdread/MainActivity");
	//jclass localClazz = env->GetObjectClass(obj);
	//gs_clazz = env->NewGlobalRef(localClazz);

//	jmethodID mid = m_pEnv->GetMethodID(m_jClazz, "readData", "(I)[B");
//	__android_log_write(ANDROID_LOG_INFO, "HRV_READ", "haha");

//	jclass clazz = env->FindClass("com/haofengkeji/hdread/MainActivity");
//	jmethodID mid = env->GetMethodID(clazz, "startDevice", "()V");
//	if (mid == NULL)
//		return isStartDevice;
//	env->CallVoidMethod(obj, mid);

	//__android_log_write(ANDROID_LOG_INFO, "HRV_READ", "StartDevice_");

	if ( !CHDRead::Instance()->StartDevice_() )
	{
		//__android_log_write(ANDROID_LOG_INFO, "HRV_READ", "fail to StartDevice_()");
		return isStartDevice;
	}

	//__android_log_write(ANDROID_LOG_INFO, "HRV_READ", "succeed in StartDevice_()");
	if ( !CHDRead::Instance()->StartRead_() )
	{
		return isStartDevice;
	}

	return isStartDevice;
}

JNIEXPORT void JNICALL Java_com_haofengkeji_hdread_MainActivity_Stop
  (JNIEnv * env, jobject obj)
{
	//�ر�Ӳ���豸
	CHDRead::Instance()->StopDevice_();
}

JNIEXPORT void JNICALL Java_com_haofengkeji_hdread_MainActivity_SetInputReport
  (JNIEnv * env, jobject obj, jbyteArray byteArray)
{
	//CHDRead::Instance()->SetInputReport(byteArray);
}
