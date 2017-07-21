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
#include "PltMicroMediaController.h"
#include "PltLog.h"

#include <android/log.h>

#define VERSION_OF_PLATITUM_SDK "1.0.2"

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

JavaVM* vmGlobal = NULL;
jobject objGlobal;

#define CHECK_IF_VALIABLE(value)  if(value==0){\
                               NPT_LOG_INFO("find value error");\
                               return -1;\
                            }\
                            NPT_LOG_INFO("find value successfully");\



PLT_MicroMediaController* controller;

int callBack_AddDms() {

    NPT_LOG_INFO("callBack_AddDms");

    int status;
    JNIEnv *env;
    bool isAttached = false;
    status = vmGlobal->GetEnv((void**) &env, JNI_VERSION_1_4);
    if (status < 0) {
        status = vmGlobal->AttachCurrentThread(&env, NULL);
        if (status < 0){

        }else{
            isAttached = true;
        }
    }

    jstring devName = env->NewStringUTF("testmediaserver1");

    env->CallVoidMethod(objGlobal,gNativeUpnpClassInfo.onDmsAdded,devName);

    if (isAttached){
        vmGlobal->DetachCurrentThread();
    }

    return 1;
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

    // Create control point
    PLT_CtrlPointReference ctrlPoint(new PLT_CtrlPoint());

    // Create controller
    NPT_LOG_INFO("Create controller");
    controller =  new PLT_MicroMediaController(ctrlPoint);

    // add control point to upnp engine and start it
    self->AddCtrlPoint(ctrlPoint);

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

jstring platinum_UPnP_checkVersion(JNIEnv *env, jobject obj)
{
    NPT_LOG_INFO("getversion");

    register_NativeUpnp(env);

    objGlobal = env->NewGlobalRef(obj);

    return env->NewStringUTF(VERSION_OF_PLATITUM_SDK);
}

jstring platinum_UPnP_setms(JNIEnv *env, jclass)
{
    NPT_LOG_INFO("setms");
    controller->HandleCmd_setms();
    return env->NewStringUTF("no result");
}

jstring platinum_UPnP_getms(JNIEnv *env, jclass)
{
    NPT_LOG_INFO("getms");
    controller->GetDms();
    return env->NewStringUTF("no result");
}


static JNINativeMethod method_table[] = {
        {"_checkVersion",  "()Ljava/lang/String;", (void *)platinum_UPnP_checkVersion},
        {"_init",  "()J", (void *)platinum_UPnP_init},
        {"_start",  "(J)I",  (void *)platinum_UPnP_start},
        {"_stop",  "(J)I",  (void *)platinum_UPnP_stop},
        {"_setms",  "()Ljava/lang/String;",  (void *)platinum_UPnP_setms},
        {"_getms",  "()Ljava/lang/String;",  (void *)platinum_UPnP_getms},
};

int register_NativeUpnp(JNIEnv *env)
{
    NPT_LOG_INFO("register_NativeUpnp");
    jclass clazz;
    FIND_CLASS(clazz, javaClassName);
    CHECK_IF_VALIABLE(clazz);
    GET_METHOD_ID(gNativeUpnpClassInfo.onDmsAdded,
                  clazz,
                  "onDmsAdded", "(Ljava/lang/String;)V");
    CHECK_IF_VALIABLE(gNativeUpnpClassInfo.onDmsAdded);

    /*GET_METHOD_ID(gNativeUpnpClassInfo.onDmsRemoved,
            clazz,
            "onDmsRemoved", "(Ljava/lang/String;)V");
    GET_METHOD_ID(gNativeUpnpClassInfo.onDmrAdded,
            clazz,
            "onDmrAdded", "(Ljava/lang/String;)V");
    GET_METHOD_ID(gNativeUpnpClassInfo.onDmrRemoved,
            clazz,
            "onDmrRemoved", "(Ljava/lang/String;)V");*/
    return 0;
}



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
    NPT_LogManager::GetDefault().Configure("plist:.level=FINEST;.handlers=ConsoleHandler;.ConsoleHandler.outputs=2;.ConsoleHandler.colors=false;.ConsoleHandler.filter=59");
    jint result = JNI_ERR;

    vmGlobal = vm;

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



