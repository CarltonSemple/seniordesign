/**********************************************************
 *            AUTOGENERATED FILE                          *
 *             DO NOT MODIFY IT                           *
 *                                                        *
 * To add new commands :                                  *
 *  - Modify ../Xml/commands.xml file                     *
 *  - Re-run generateFeatureControllers.py script         *
 *                                                        *
 **********************************************************/

/**
* @file ARCONTROLLER_JNI_FeatureMiniDroneDebug
* @brief ARCONTROLLER_FEATURE_MiniDroneDebug_t JNI feature MiniDroneDebug c file.
*/

/*****************************************
 *
 *             include file :
 *
 *****************************************/

#include <jni.h>
#include <stdlib.h>

#include <libARSAL/ARSAL_Print.h>

#include <libARController/ARCONTROLLER_Error.h>
#include <libARController/ARCONTROLLER_Feature.h>

/*****************************************
 *
 *             define :
 *
 *****************************************/

#define ARCONTROLLER_JNIFEATUREMINIDRONEDEBUG_TAG "ARCONTROLLER_JNI_FeatureMiniDroneDebug"

/*****************************************
 *
 *             private header:
 *
 *****************************************/


/*****************************************
 *
 *             implementation :
 *
 *****************************************/

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureMiniDroneDebug_nativeSendDebugTest1 (JNIEnv *env, jobject thizz, jlong jFeature, jbyte _t1Args)
{
    // local declarations
    ARCONTROLLER_FEATURE_MiniDroneDebug_t *nativeFeature = (ARCONTROLLER_FEATURE_MiniDroneDebug_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendDebugTest1 (nativeFeature, _t1Args);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureMiniDroneDebug_nativeSendDebugTest2 (JNIEnv *env, jobject thizz, jlong jFeature, jbyte _t2Args)
{
    // local declarations
    ARCONTROLLER_FEATURE_MiniDroneDebug_t *nativeFeature = (ARCONTROLLER_FEATURE_MiniDroneDebug_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendDebugTest2 (nativeFeature, _t2Args);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureMiniDroneDebug_nativeSendDebugTest3 (JNIEnv *env, jobject thizz, jlong jFeature, jbyte _t3Args)
{
    // local declarations
    ARCONTROLLER_FEATURE_MiniDroneDebug_t *nativeFeature = (ARCONTROLLER_FEATURE_MiniDroneDebug_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendDebugTest3 (nativeFeature, _t3Args);

    return error;
}
