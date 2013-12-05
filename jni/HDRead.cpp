#include "com_haofengkeji_hdread_MainActivity.h"
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include "HDReadInterface.h"
//#include <linux/usbdevice_fs.h>
//#include <sys/ioctl.h>
//#include <linux/usb.h>

JNIEnv* m_pEnv = NULL;		// NDK对象
JavaVM* gs_jvm;  			// 全局Java虚拟机
jobject gs_obj;				// java对象
//jclass gs_clazz;			// 获取的java类

jint JNI_OnLoad (JavaVM* vm, void* reserved)
{

//	char data[4] = {0x09, 0x90, 0x50, 0x50};
//
//	struct usbdevfs_bulktransfer bt;
//	bt.ep = usb_endpoint;  /* endpoint (received from Java) */
//	bt.len = 4;            /* length of data */
//	bt.timeout = 100;      /* timeout in ms */
//	bt.data = data;        /* the data */
//
//	int rtn = ioctl(fd, USBDEVFS_BULK, &bt);

    // Cache the JavaVM interface pointer
	gs_jvm = vm;
    return JNI_VERSION_1_6;
}

JNIEXPORT jboolean JNICALL Java_com_haofengkeji_hdread_MainActivity_StartDevice
  (JNIEnv * env, jobject obj)
{
	//env->GetJavaVM(&gs_jvm);
	jboolean isStartDevice = JNI_FALSE;

	m_pEnv = env;

	//gs_jvm = env->GetJavaVM(&gs_jvm);	// 获取JavaVM指针
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

JNIEXPORT void JNICALL Java_com_haofengkeji_hdread_MainActivity_StopDevice
  (JNIEnv * env, jobject obj)
{
	//关闭硬件设备
	CHDRead::Instance()->StopDevice_();
}

JNIEXPORT void JNICALL Java_com_haofengkeji_hdread_MainActivity_SetDifficultLevel
  (JNIEnv * env, jobject obj, jint diff)
{
	Fun_interface::Instance()->SetCurDiff(diff);
}

JNIEXPORT jboolean JNICALL Java_com_haofengkeji_hdread_MainActivity_GetBlockFlag
  (JNIEnv * env, jobject obj)
{
	return (jboolean)Fun_interface::Instance()->GetBlockFlag();
}

JNIEXPORT void JNICALL Java_com_haofengkeji_hdread_MainActivity_SetBlockFlag
  (JNIEnv * env, jobject obj, jboolean bBlockFlag)
{
	Fun_interface::Instance()->SetBlockFlag(bBlockFlag);
}

JNIEXPORT void JNICALL Java_com_haofengkeji_hdread_MainActivity_SetReadHRV
  (JNIEnv * env, jobject obj, jboolean bRead)
{
	Fun_interface::Instance()->SetReadHRV(bRead);
}

JNIEXPORT jboolean JNICALL Java_com_haofengkeji_hdread_MainActivity_GetReadHRV
  (JNIEnv * env, jobject obj)
{
	return (jboolean)Fun_interface::Instance()->GetReadHRV();
}

JNIEXPORT jint JNICALL Java_com_haofengkeji_hdread_MainActivity_GetIBICount
  (JNIEnv * env, jobject obj)
{
	//__android_log_print(ANDROID_LOG_INFO, "HRV_READ", "IBI count %d", Fun_interface::Instance()->GetIBICount());
	return Fun_interface::Instance()->GetIBICount();
}

JNIEXPORT jint JNICALL Java_com_haofengkeji_hdread_MainActivity_GetArtifactStatus
  (JNIEnv * env, jobject obj)
{
	return Fun_interface::Instance()->GetArtifactStatus();
}

JNIEXPORT jfloatArray JNICALL Java_com_haofengkeji_hdread_MainActivity_GetPackHrt
  (JNIEnv * env, jobject obj)
{
	float *HRV;
	int size;
	HRV = CHDRead::Instance()->GetPackHrt_(size);
	for (int i = 0; i < size; i++)
	{
//		char chFloat[20];
//		memset(chFloat,'\0', sizeof(chFloat));
//		sprintf(chFloat, "%f, ", HRV[i]);
		__android_log_print(ANDROID_LOG_INFO, "HRV_READ", "hrv = %f", HRV[i]);
	}
	jfloatArray jarr = env->NewFloatArray(size);
	env->SetFloatArrayRegion(jarr, 0, size, HRV);
	return jarr;
}
