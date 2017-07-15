/*****************************************************************
|
|      Android JNI Interface
|
|      (c) 2002-2012 Plutinosoft LLC
|      Author: Sylvain Rebaud (sylvain@plutinosoft.com)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include <assert.h>
#include <jni.h>
#include <string.h>
#include <sys/types.h>

#include "platinum-jni.h"
#include "Platinum.h"

#include <android/log.h>

/*----------------------------------------------------------------------
|   logging
+---------------------------------------------------------------------*/
NPT_SET_LOCAL_LOGGER("platinum.android.jni")

/*----------------------------------------------------------------------
|   functions
+---------------------------------------------------------------------*/
__attribute__((constructor)) static void onDlOpen(void)
{
}

/*
 * Class:     com_plutinosoft_platinum_UPnP
 * Method:    _init
 * Signature: ()J
 */
jlong platinum_UPnP_init(JNIEnv *env, jclass)
{
    NPT_LOG_INFO("init");
    PLT_UPnP* self = new PLT_UPnP();
    return (jlong)self;
}

/*
 * Class:     com_plutinosoft_platinum_UPnP
 * Method:    _start
 * Signature: (J)I
 */
jint platinum_UPnP_start(JNIEnv *, jclass, jlong _self)
{
    NPT_LOG_INFO("start");
    PLT_UPnP* self = (PLT_UPnP*)_self;
    
    return self->Start();
}

/*
 * Class:     com_plutinosoft_platinum_UPnP
 * Method:    _stop
 * Signature: (J)I
 */
jint platinum_UPnP_stop(JNIEnv *, jclass, jlong _self)
{
    NPT_LOG_INFO("stop");
    PLT_UPnP* self = (PLT_UPnP*)_self;
    
    return self->Stop();
}

jstring platinum_UPnP_getversion(JNIEnv *env, jclass)
{
    NPT_LOG_INFO("getversion");
    return env->NewStringUTF("1.0.0");
}



static JNINativeMethod method_table[] = {
	{"_getversion",  "()Ljava/lang/String;", (void *)platinum_UPnP_getversion},
	{"_init",  "()J", (void *)platinum_UPnP_init},
	{"_start",  "(J)I",  (void *)platinum_UPnP_start},
	{"_stop",  "(J)I",  (void *)platinum_UPnP_stop},
};


static int register_method(JNIEnv *env){
	jclass clazz = env->FindClass(javaClassName);

    if (clazz == NULL) {
		NPT_LOG_INFO("register methods, unable to find class\n");
		return JNI_FALSE;
	}

	if (env->RegisterNatives(clazz, method_table, sizeof(method_table) / sizeof(method_table[0])) < 0)
    {
		NPT_LOG_INFO("register methods, failed for\n");
		return JNI_FALSE;
    }

	return JNI_TRUE;
}


/*----------------------------------------------------------------------
|    JNI_OnLoad
+---------------------------------------------------------------------*/
jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	NPT_LogManager::GetDefault().Configure("plist:.level=FINE;.handlers=ConsoleHandler;.ConsoleHandler.outputs=2;.ConsoleHandler.colors=false;.ConsoleHandler.filter=59");
    jint result = JNI_ERR;
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    if (!register_method(env)) {
        return -1;
    }
    result = JNI_VERSION_1_4;
    return result;
}


