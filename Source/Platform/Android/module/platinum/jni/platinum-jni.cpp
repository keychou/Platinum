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
#include "PltMediaItem.h"

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

int callBack_AddDms(const char* uuid, const char* friendName, const char* deviceType) {

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

    jstring luuid = env->NewStringUTF(uuid);
	jstring lfriendName = env->NewStringUTF(friendName);
	jstring ldeviceType = env->NewStringUTF(deviceType);

    env->CallVoidMethod(objGlobal,gNativeUpnpClassInfo.onDmsAdded,luuid,lfriendName,ldeviceType);

    if (isAttached){
        vmGlobal->DetachCurrentThread();
    }

    return 1;
}

int callBack_RemovedDms(const char* uuid, const char* friendName, const char* deviceType) {

    NPT_LOG_INFO("callBack_RemovedDms");

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

    jstring luuid = env->NewStringUTF(uuid);
	jstring lfriendName = env->NewStringUTF(friendName);
	jstring ldeviceType = env->NewStringUTF(deviceType);

    env->CallVoidMethod(objGlobal, gNativeUpnpClassInfo.onDmsRemoved,luuid,lfriendName,ldeviceType);

    if (isAttached){
        vmGlobal->DetachCurrentThread();
    }

    return 1;
}



int callBack_AddDmr(const char* uuid, const char* friendName, const char* deviceType) {

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

	jstring luuid = env->NewStringUTF(uuid);
	jstring lfriendName = env->NewStringUTF(friendName);
	jstring ldeviceType = env->NewStringUTF(deviceType);

    env->CallVoidMethod(objGlobal,gNativeUpnpClassInfo.onDmrAdded,luuid,lfriendName,ldeviceType);

    if (isAttached){
        vmGlobal->DetachCurrentThread();
    }

    return 1;
}

int callBack_RemovedDmr(const char* uuid, const char* friendName, const char* deviceType) {

    NPT_LOG_INFO("callBack_RemovedDmr");

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

    jstring luuid = env->NewStringUTF(uuid);
	jstring lfriendName = env->NewStringUTF(friendName);
	jstring ldeviceType = env->NewStringUTF(deviceType);

    env->CallVoidMethod(objGlobal, gNativeUpnpClassInfo.onDmrRemoved,luuid,lfriendName,ldeviceType);

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

jint platinum_UPnP_setActiveDms(JNIEnv *env, jclass, jstring uuid)
{
    NPT_LOG_INFO("setActiveDms");
	const char* luuid = env->GetStringUTFChars(uuid, 0);
    
    //char nvid_value[MAX_NVID_BUFF];
    //strcpy(nvid_value, s);
	
    controller->setActiveDms(luuid);
    return 0;
}

jint platinum_UPnP_setActiveDmr(JNIEnv *env, jclass, jstring uuid)
{
    NPT_LOG_INFO("setActiveDmr");
	if (uuid == NULL){
		NPT_LOG_INFO("uuid is null");
        return -1;
	}
	const char* luuid = env->GetStringUTFChars(uuid, 0);
	
    controller->setActiveDmr(luuid);
    return 0;
}

jstring platinum_UPnP_getActiveDms(JNIEnv *env, jobject obj)
{
    NPT_LOG_INFO("platinum_UPnP_getActiveDms");

	char* activeDms = controller->getActiveDms();

    return env->NewStringUTF(activeDms);
}

jstring platinum_UPnP_getActiveDmr(JNIEnv *env, jobject obj)
{
    NPT_LOG_INFO("platinum_UPnP_getActiveDmr");

	char* activeDmr = controller->getActiveDmr();

    return env->NewStringUTF(activeDmr);
}

jobjectArray platinum_UPnP_lsFiles(JNIEnv *env, jobject obj)
{
    NPT_LOG_INFO("platinum_UPnP_lsFiles");

	jobjectArray mediaObjectArray = NULL;
    jclass cl = NULL;
    jobject mediaobj;  

    cl = env->FindClass("com/plutinosoft/platinum/MediaObject");
    jfieldID Objecttype = (env)->GetFieldID(cl, "m_Objecttype", "Ljava/lang/String;");  
    jfieldID ObjectID = (env)->GetFieldID(cl, "m_ObjectID", "Ljava/lang/String;");
	jfieldID ParentID = (env)->GetFieldID(cl, "m_ParentID", "Ljava/lang/String;");
	jfieldID Title = (env)->GetFieldID(cl, "m_Title", "Ljava/lang/String;");

    jmethodID consID = (env)->GetMethodID(cl, "<init>", "()V");

	NPT_Reference<PLT_MediaObjectList> mediaObjectList = controller->lsFiles();
	
		if (!mediaObjectList.IsNull()) {
			    jint len = mediaObjectList->GetItemCount();
				mediaObjectArray = env->NewObjectArray(len, cl, NULL);
				NPT_LOG_INFO_1("klein------There were %d results\n", len);
		
				NPT_List<PLT_MediaObject*>::Iterator item = mediaObjectList->GetFirstItem();
				int i = 0;
				while (item) {
					if ((*item)->IsContainer()) {
						NPT_LOG_INFO_2("Container: %s (%s)\n", (*item)->m_Title.GetChars(), (*item)->m_ObjectID.GetChars());
						mediaobj = env->NewObject(cl, consID);
								
						env->SetObjectField(mediaobj, Objecttype, env->NewStringUTF("object.container"));
						env->SetObjectField(mediaobj, ObjectID, env->NewStringUTF((*item)->m_ObjectID.GetChars())); 
						env->SetObjectField(mediaobj, ParentID, env->NewStringUTF((*item)->m_ParentID.GetChars())); 
						env->SetObjectField(mediaobj, Title, env->NewStringUTF((*item)->m_Title.GetChars()));
						env->SetObjectArrayElement(mediaObjectArray, i, mediaobj);

						
					} else {
						NPT_LOG_INFO_2("Item: %s (%s)\n", (*item)->m_Title.GetChars(), (*item)->m_ObjectID.GetChars());
						mediaobj = env->NewObject(cl, consID);
								
						env->SetObjectField(mediaobj, Objecttype, env->NewStringUTF("object.item"));
						env->SetObjectField(mediaobj, ObjectID, env->NewStringUTF((*item)->m_ObjectID.GetChars())); 
						env->SetObjectField(mediaobj, ParentID, env->NewStringUTF((*item)->m_ParentID.GetChars())); 
						env->SetObjectField(mediaobj, Title, env->NewStringUTF((*item)->m_Title.GetChars()));
						env->SetObjectArrayElement(mediaObjectArray, i, mediaobj);
					}
					++item;
					++i;
				}
		
				mediaObjectList = NULL;
			}
		
    return mediaObjectArray;
}

jint platinum_UPnP_changeDirectory(JNIEnv *env, jclass, jstring objectId)
{
    NPT_LOG_INFO("platinum_UPnP_changeDirectory");
	const char* lobjectid = env->GetStringUTFChars(objectId, 0);
	
    controller->changeDirectory(lobjectid);
    return 0;
}

jint platinum_UPnP_cdup(JNIEnv *env, jclass)
{
    NPT_LOG_INFO("platinum_UPnP_cdup");
    controller->cdup();
    return 0;
}


jint platinum_UPnP_play(JNIEnv *env, jclass, jstring objectId)
{
    NPT_LOG_INFO("platinum_UPnP_play");
	const char* lobjectid = env->GetStringUTFChars(objectId, 0);
	
    controller->playFiles(lobjectid);
    return 0;
}




static JNINativeMethod method_table[] = {
        {"_checkVersion",  "()Ljava/lang/String;", (void *)platinum_UPnP_checkVersion},
        {"_init",  "()J", (void *)platinum_UPnP_init},
        {"_start",  "(J)I",  (void *)platinum_UPnP_start},
        {"_stop",  "(J)I",  (void *)platinum_UPnP_stop},
        {"_setActiveDms",  "(Ljava/lang/String;)I",  (void *)platinum_UPnP_setActiveDms},
        {"_setActiveDmr",  "(Ljava/lang/String;)I",  (void *)platinum_UPnP_setActiveDmr},
        {"_getActiveDms",  "()Ljava/lang/String;",  (void *)platinum_UPnP_getActiveDms},
        {"_getActiveDmr",  "()Ljava/lang/String;",  (void *)platinum_UPnP_getActiveDmr},
        {"_lsFiles",  "()[Lcom/plutinosoft/platinum/MediaObject;",  (void *)platinum_UPnP_lsFiles},
        {"_changeDirectory",  "(Ljava/lang/String;)I",  (void *)platinum_UPnP_changeDirectory},
        {"_play",  "(Ljava/lang/String;)I",  (void *)platinum_UPnP_play},
        {"_cdup",  "()I",  (void *)platinum_UPnP_cdup},
};

int register_NativeUpnp(JNIEnv *env)
{
    NPT_LOG_INFO("register_NativeUpnp");
    jclass clazz;
    FIND_CLASS(clazz, javaClassName);
    CHECK_IF_VALIABLE(clazz);
    GET_METHOD_ID(gNativeUpnpClassInfo.onDmsAdded,
                  clazz,
                  "onDmsAdded", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    CHECK_IF_VALIABLE(gNativeUpnpClassInfo.onDmsAdded);

    GET_METHOD_ID(gNativeUpnpClassInfo.onDmsRemoved,
            clazz,
            "onDmsRemoved", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    CHECK_IF_VALIABLE(gNativeUpnpClassInfo.onDmsRemoved);

	
    GET_METHOD_ID(gNativeUpnpClassInfo.onDmrAdded,
            clazz,
            "onDmrAdded", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	CHECK_IF_VALIABLE(gNativeUpnpClassInfo.onDmrAdded);
	
    GET_METHOD_ID(gNativeUpnpClassInfo.onDmrRemoved,
            clazz,
            "onDmrRemoved", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	CHECK_IF_VALIABLE(gNativeUpnpClassInfo.onDmrRemoved);
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



