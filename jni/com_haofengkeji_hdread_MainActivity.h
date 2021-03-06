/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_haofengkeji_hdread_MainActivity */

#ifndef _Included_com_haofengkeji_hdread_MainActivity
#define _Included_com_haofengkeji_hdread_MainActivity
#ifdef __cplusplus
extern "C" {
#endif
#undef com_haofengkeji_hdread_MainActivity_MODE_PRIVATE
#define com_haofengkeji_hdread_MainActivity_MODE_PRIVATE 0L
#undef com_haofengkeji_hdread_MainActivity_MODE_WORLD_READABLE
#define com_haofengkeji_hdread_MainActivity_MODE_WORLD_READABLE 1L
#undef com_haofengkeji_hdread_MainActivity_MODE_WORLD_WRITEABLE
#define com_haofengkeji_hdread_MainActivity_MODE_WORLD_WRITEABLE 2L
#undef com_haofengkeji_hdread_MainActivity_MODE_APPEND
#define com_haofengkeji_hdread_MainActivity_MODE_APPEND 32768L
#undef com_haofengkeji_hdread_MainActivity_MODE_MULTI_PROCESS
#define com_haofengkeji_hdread_MainActivity_MODE_MULTI_PROCESS 4L
#undef com_haofengkeji_hdread_MainActivity_BIND_AUTO_CREATE
#define com_haofengkeji_hdread_MainActivity_BIND_AUTO_CREATE 1L
#undef com_haofengkeji_hdread_MainActivity_BIND_DEBUG_UNBIND
#define com_haofengkeji_hdread_MainActivity_BIND_DEBUG_UNBIND 2L
#undef com_haofengkeji_hdread_MainActivity_BIND_NOT_FOREGROUND
#define com_haofengkeji_hdread_MainActivity_BIND_NOT_FOREGROUND 4L
#undef com_haofengkeji_hdread_MainActivity_BIND_ABOVE_CLIENT
#define com_haofengkeji_hdread_MainActivity_BIND_ABOVE_CLIENT 8L
#undef com_haofengkeji_hdread_MainActivity_BIND_ALLOW_OOM_MANAGEMENT
#define com_haofengkeji_hdread_MainActivity_BIND_ALLOW_OOM_MANAGEMENT 16L
#undef com_haofengkeji_hdread_MainActivity_BIND_WAIVE_PRIORITY
#define com_haofengkeji_hdread_MainActivity_BIND_WAIVE_PRIORITY 32L
#undef com_haofengkeji_hdread_MainActivity_BIND_IMPORTANT
#define com_haofengkeji_hdread_MainActivity_BIND_IMPORTANT 64L
#undef com_haofengkeji_hdread_MainActivity_BIND_ADJUST_WITH_ACTIVITY
#define com_haofengkeji_hdread_MainActivity_BIND_ADJUST_WITH_ACTIVITY 64L
#undef com_haofengkeji_hdread_MainActivity_CONTEXT_INCLUDE_CODE
#define com_haofengkeji_hdread_MainActivity_CONTEXT_INCLUDE_CODE 1L
#undef com_haofengkeji_hdread_MainActivity_CONTEXT_IGNORE_SECURITY
#define com_haofengkeji_hdread_MainActivity_CONTEXT_IGNORE_SECURITY 2L
#undef com_haofengkeji_hdread_MainActivity_CONTEXT_RESTRICTED
#define com_haofengkeji_hdread_MainActivity_CONTEXT_RESTRICTED 4L
#undef com_haofengkeji_hdread_MainActivity_RESULT_CANCELED
#define com_haofengkeji_hdread_MainActivity_RESULT_CANCELED 0L
#undef com_haofengkeji_hdread_MainActivity_RESULT_OK
#define com_haofengkeji_hdread_MainActivity_RESULT_OK -1L
#undef com_haofengkeji_hdread_MainActivity_RESULT_FIRST_USER
#define com_haofengkeji_hdread_MainActivity_RESULT_FIRST_USER 1L
#undef com_haofengkeji_hdread_MainActivity_DEFAULT_KEYS_DISABLE
#define com_haofengkeji_hdread_MainActivity_DEFAULT_KEYS_DISABLE 0L
#undef com_haofengkeji_hdread_MainActivity_DEFAULT_KEYS_DIALER
#define com_haofengkeji_hdread_MainActivity_DEFAULT_KEYS_DIALER 1L
#undef com_haofengkeji_hdread_MainActivity_DEFAULT_KEYS_SHORTCUT
#define com_haofengkeji_hdread_MainActivity_DEFAULT_KEYS_SHORTCUT 2L
#undef com_haofengkeji_hdread_MainActivity_DEFAULT_KEYS_SEARCH_LOCAL
#define com_haofengkeji_hdread_MainActivity_DEFAULT_KEYS_SEARCH_LOCAL 3L
#undef com_haofengkeji_hdread_MainActivity_DEFAULT_KEYS_SEARCH_GLOBAL
#define com_haofengkeji_hdread_MainActivity_DEFAULT_KEYS_SEARCH_GLOBAL 4L
/*
 * Class:     com_haofengkeji_hdread_MainActivity
 * Method:    StartDevice
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_haofengkeji_hdread_MainActivity_StartDevice
  (JNIEnv *, jobject);

/*
 * Class:     com_haofengkeji_hdread_MainActivity
 * Method:    StopDevice
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_haofengkeji_hdread_MainActivity_StopDevice
  (JNIEnv *, jobject);

/*
 * Class:     com_haofengkeji_hdread_MainActivity
 * Method:    SetDifficultLevel
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_haofengkeji_hdread_MainActivity_SetDifficultLevel
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_haofengkeji_hdread_MainActivity
 * Method:    GetBlockFlag
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_haofengkeji_hdread_MainActivity_GetBlockFlag
  (JNIEnv *, jobject);

/*
 * Class:     com_haofengkeji_hdread_MainActivity
 * Method:    SetBlockFlag
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_com_haofengkeji_hdread_MainActivity_SetBlockFlag
  (JNIEnv *, jobject, jboolean);

/*
 * Class:     com_haofengkeji_hdread_MainActivity
 * Method:    SetReadHRV
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_com_haofengkeji_hdread_MainActivity_SetReadHRV
  (JNIEnv *, jobject, jboolean);

/*
 * Class:     com_haofengkeji_hdread_MainActivity
 * Method:    GetReadHRV
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_haofengkeji_hdread_MainActivity_GetReadHRV
  (JNIEnv *, jobject);

/*
 * Class:     com_haofengkeji_hdread_MainActivity
 * Method:    GetPackHrt
 * Signature: ()[F
 */
JNIEXPORT jfloatArray JNICALL Java_com_haofengkeji_hdread_MainActivity_GetPackHrt
  (JNIEnv *, jobject);

/*
 * Class:     com_haofengkeji_hdread_MainActivity
 * Method:    GetIBICount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_haofengkeji_hdread_MainActivity_GetIBICount
  (JNIEnv *, jobject);

/*
 * Class:     com_haofengkeji_hdread_MainActivity
 * Method:    GetArtifactStatus
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_haofengkeji_hdread_MainActivity_GetArtifactStatus
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
