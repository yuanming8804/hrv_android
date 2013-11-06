LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#LOCAL_C_INCLUDES := $(LOCAL_PATH) 
LOCAL_C_INCLUDES := D:\AndroidEclipse\android-ndk-r9\sources\cxx-stl\stlport\stlport

#APP_STL := stlport_static
#APP_STL := gnustl_static #GNU STL
#APP_STL := stlprot_shared    #模板库以动态库的方式加载

LOCAL_MODULE    := HDRead
LOCAL_SRC_FILES := HDRead.cpp Array.cpp Average.cpp DataCollector.cpp Drv.cpp DrvInterface.cpp DrvM801Usb.cpp firFilter.cpp HDReadInterface.cpp Queue.cpp Summ.cpp Timer.cpp Trgcalc.cpp
LOCAL_LDLIBS    := -llog
#LOCAL_LDLIBS := -llog  D:\AndroidEclipse\android-ndk-r9\sources\cxx-stl\stlport\libs\armeabi\libstlport_static.a

include $(BUILD_SHARED_LIBRARY)
