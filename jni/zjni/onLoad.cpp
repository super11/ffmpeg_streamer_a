#define TAG "ffmpeg_onLoad"

#include <stdlib.h>
#include <android/log.h>
#include "jniUtils.h"

extern "C" {

extern int register_android_media_FFMpegAVRational(JNIEnv *env);

#ifdef BUILD_WITH_CONVERTOR
extern int register_android_media_FFMpeg(JNIEnv *env);
#endif

extern int register_android_media_FFMpegAVFormatContext(JNIEnv *env);
extern int register_android_media_FFMpegAVInputFormat(JNIEnv *env);

}

extern int register_android_media_FFMpegAVCodecContext(JNIEnv *env);
extern int register_android_media_FFMpegUtils(JNIEnv *env);
extern int register_android_media_FFMpegAVFrame(JNIEnv *env);

#ifdef BUILD_WITH_PLAYER
extern int register_android_media_FFMpegPlayerAndroid(JNIEnv *env);
#endif

static JavaVM *sVm;

/*
 * Throw an exception with the specified class and an optional message.
 */
int jniThrowException(JNIEnv* env, const char* className, const char* msg) {
    jclass exceptionClass = env->FindClass(className);
    if (exceptionClass == NULL) {
        __android_log_print(ANDROID_LOG_ERROR,
			    TAG,
			    "Unable to find exception class %s",
	                    className);
        return -1;
    }

    if (env->ThrowNew(exceptionClass, msg) != JNI_OK) {
        __android_log_print(ANDROID_LOG_ERROR,
			    TAG,
			    "Failed throwing '%s' '%s'",
			    className, msg);
    }
    return 0;
}


int attachVmToThread ()
{
    JNIEnv* env = NULL;
    //Try attach to current thread; 
    return  sVm->AttachCurrentThread( &env, NULL) ;
}

void dettachVmFromThread()
{
    sVm->DetachCurrentThread();
}

JNIEnv* getJNIEnv() {
    JNIEnv* env = NULL;


    if (sVm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) {
    	__android_log_print(ANDROID_LOG_ERROR,
							TAG,
							"Failed to obtain JNIEnv 01 ");

   
    }
    return env;
}

/*
 * Register native JNI-callable methods.
 *
 * "className" looks like "java/lang/String".
 */
int jniRegisterNativeMethods(JNIEnv* env,
                             const char* className,
                             const JNINativeMethod* gMethods,
                             int numMethods)
{
    jclass clazz;

    __android_log_print(ANDROID_LOG_INFO, TAG, "Registering %s natives\n", className);
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Native registration unable to find class '%s'\n", className);
        return -1;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "RegisterNatives failed for '%s'\n", className);
        return -1;
    }
    return 0;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env = NULL;
    jint result = JNI_ERR;
	sVm = vm;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "GetEnv failed!");
        return result;
    }

    __android_log_print(ANDROID_LOG_INFO, TAG, "loading . . .");

#ifdef BUILD_WITH_CONVERTOR
    if(register_android_media_FFMpeg(env) != JNI_OK) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "can't load android_media_FFMpeg");
        goto end;
    }
#endif

#if 0
    if(register_android_media_FFMpegAVFormatContext(env) != JNI_OK) {
    	__android_log_print(ANDROID_LOG_ERROR, TAG, "can't load android_media_FFMpegAVFormatContext");
        goto end;
	}

    if(register_android_media_FFMpegAVCodecContext(env) != JNI_OK) {
    	__android_log_print(ANDROID_LOG_ERROR, TAG, "can't load android_media_FFMpegAVCodecContext");
        goto end;
	}

    if(register_android_media_FFMpegAVRational(env) != JNI_OK) {
    	__android_log_print(ANDROID_LOG_ERROR, TAG, "can't load android_media_FFMpegAVRational");
    	goto end;
    }
	
	if(register_android_media_FFMpegAVInputFormat(env) != JNI_OK) {
    	__android_log_print(ANDROID_LOG_ERROR, TAG, "can't load android_media_FFMpegAVInputFormat");
    	goto end;
    }
	
	if(register_android_media_FFMpegUtils(env) != JNI_OK) {
		__android_log_print(ANDROID_LOG_ERROR, TAG, "can't load android_media_FFMpegUtils");
		goto end;
	}

	if(register_android_media_FFMpegAVFrame(env) != JNI_OK) {
		__android_log_print(ANDROID_LOG_ERROR, TAG, "can't load android_media_FFMpegAVFrame");
		goto end;
	}

#endif 
#ifdef BUILD_WITH_PLAYER
    if(register_android_media_FFMpegPlayerAndroid(env) != JNI_OK) {
    	__android_log_print(ANDROID_LOG_ERROR, TAG, "can't load android_media_FFMpegPlayerAndroid");
    	goto end;
    }
#endif



    result = JNI_VERSION_1_6;

end:
    return result;
}
