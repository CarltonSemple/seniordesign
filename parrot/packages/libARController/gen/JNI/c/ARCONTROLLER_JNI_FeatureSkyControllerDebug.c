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
* @file ARCONTROLLER_JNI_FeatureSkyControllerDebug
* @brief ARCONTROLLER_FEATURE_SkyControllerDebug_t JNI feature SkyControllerDebug c file.
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

#define ARCONTROLLER_JNIFEATURESKYCONTROLLERDEBUG_TAG "ARCONTROLLER_JNI_FeatureSkyControllerDebug"

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
Java_com_parrot_arsdk_arcontroller_ARFeatureSkyControllerDebug_nativeSendDebugTest1 (JNIEnv *env, jobject thizz, jlong jFeature, jbyte _t1Args)
{
    // local declarations
    ARCONTROLLER_FEATURE_SkyControllerDebug_t *nativeFeature = (ARCONTROLLER_FEATURE_SkyControllerDebug_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendDebugTest1 (nativeFeature, _t1Args);

    return error;
}

