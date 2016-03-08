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
* @file ARCONTROLLER_JNI_FeatureJumpingSumo
* @brief ARCONTROLLER_FEATURE_JumpingSumo_t JNI feature JumpingSumo c file.
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

#define ARCONTROLLER_JNIFEATUREJUMPINGSUMO_TAG "ARCONTROLLER_JNI_FeatureJumpingSumo"

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

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoPilotingStatePostureChangedState (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_PILOTINGSTATE_POSTURECHANGED_STATE);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoPilotingStateAlertStateChangedState (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_PILOTINGSTATE_ALERTSTATECHANGED_STATE);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoPilotingStateSpeedChangedSpeed (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_PILOTINGSTATE_SPEEDCHANGED_SPEED);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoPilotingStateSpeedChangedRealSpeed (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_PILOTINGSTATE_SPEEDCHANGED_REALSPEED);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoAnimationsStateJumpLoadChangedState (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_ANIMATIONSSTATE_JUMPLOADCHANGED_STATE);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoAnimationsStateJumpTypeChangedState (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_ANIMATIONSSTATE_JUMPTYPECHANGED_STATE);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoAnimationsStateJumpMotorProblemChangedError (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_ANIMATIONSSTATE_JUMPMOTORPROBLEMCHANGED_ERROR);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoSettingsStateProductGPSVersionChangedSoftware (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_SETTINGSSTATE_PRODUCTGPSVERSIONCHANGED_SOFTWARE);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoSettingsStateProductGPSVersionChangedHardware (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_SETTINGSSTATE_PRODUCTGPSVERSIONCHANGED_HARDWARE);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoMediaRecordStatePictureStateChangedState (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_MEDIARECORDSTATE_PICTURESTATECHANGED_STATE);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoMediaRecordStatePictureStateChangedMassstorageid (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_MEDIARECORDSTATE_PICTURESTATECHANGED_MASS_STORAGE_ID);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoMediaRecordStateVideoStateChangedState (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_MEDIARECORDSTATE_VIDEOSTATECHANGED_STATE);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoMediaRecordStateVideoStateChangedMassstorageid (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_MEDIARECORDSTATE_VIDEOSTATECHANGED_MASS_STORAGE_ID);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoMediaRecordStatePictureStateChangedV2State (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_MEDIARECORDSTATE_PICTURESTATECHANGEDV2_STATE);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoMediaRecordStatePictureStateChangedV2Error (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_MEDIARECORDSTATE_PICTURESTATECHANGEDV2_ERROR);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoMediaRecordStateVideoStateChangedV2State (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_MEDIARECORDSTATE_VIDEOSTATECHANGEDV2_STATE);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoMediaRecordStateVideoStateChangedV2Error (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_MEDIARECORDSTATE_VIDEOSTATECHANGEDV2_ERROR);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoMediaRecordEventPictureEventChangedEvent (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_MEDIARECORDEVENT_PICTUREEVENTCHANGED_EVENT);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoMediaRecordEventPictureEventChangedError (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_MEDIARECORDEVENT_PICTUREEVENTCHANGED_ERROR);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoMediaRecordEventVideoEventChangedEvent (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_MEDIARECORDEVENT_VIDEOEVENTCHANGED_EVENT);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoMediaRecordEventVideoEventChangedError (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_MEDIARECORDEVENT_VIDEOEVENTCHANGED_ERROR);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoNetworkSettingsStateWifiSelectionChangedType (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_NETWORKSETTINGSSTATE_WIFISELECTIONCHANGED_TYPE);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoNetworkSettingsStateWifiSelectionChangedBand (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_NETWORKSETTINGSSTATE_WIFISELECTIONCHANGED_BAND);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoNetworkSettingsStateWifiSelectionChangedChannel (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_NETWORKSETTINGSSTATE_WIFISELECTIONCHANGED_CHANNEL);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoNetworkStateWifiScanListChangedSsid (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_NETWORKSTATE_WIFISCANLISTCHANGED_SSID);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoNetworkStateWifiScanListChangedRssi (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_NETWORKSTATE_WIFISCANLISTCHANGED_RSSI);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoNetworkStateWifiScanListChangedBand (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_NETWORKSTATE_WIFISCANLISTCHANGED_BAND);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoNetworkStateWifiScanListChangedChannel (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_NETWORKSTATE_WIFISCANLISTCHANGED_CHANNEL);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoNetworkStateWifiAuthChannelListChangedBand (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_NETWORKSTATE_WIFIAUTHCHANNELLISTCHANGED_BAND);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoNetworkStateWifiAuthChannelListChangedChannel (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_NETWORKSTATE_WIFIAUTHCHANNELLISTCHANGED_CHANNEL);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoNetworkStateWifiAuthChannelListChangedInorout (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_NETWORKSTATE_WIFIAUTHCHANNELLISTCHANGED_IN_OR_OUT);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoNetworkStateLinkQualityChangedQuality (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_NETWORKSTATE_LINKQUALITYCHANGED_QUALITY);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoAudioSettingsStateMasterVolumeChangedVolume (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_AUDIOSETTINGSSTATE_MASTERVOLUMECHANGED_VOLUME);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoAudioSettingsStateThemeChangedTheme (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_AUDIOSETTINGSSTATE_THEMECHANGED_THEME);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoRoadPlanStateScriptMetadataListChangedUuid (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_ROADPLANSTATE_SCRIPTMETADATALISTCHANGED_UUID);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoRoadPlanStateScriptMetadataListChangedVersion (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_ROADPLANSTATE_SCRIPTMETADATALISTCHANGED_VERSION);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoRoadPlanStateScriptMetadataListChangedProduct (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_ROADPLANSTATE_SCRIPTMETADATALISTCHANGED_PRODUCT);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoRoadPlanStateScriptMetadataListChangedName (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_ROADPLANSTATE_SCRIPTMETADATALISTCHANGED_NAME);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoRoadPlanStateScriptMetadataListChangedLastModified (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_ROADPLANSTATE_SCRIPTMETADATALISTCHANGED_LASTMODIFIED);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoRoadPlanStateScriptUploadChangedResultCode (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_ROADPLANSTATE_SCRIPTUPLOADCHANGED_RESULTCODE);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoRoadPlanStateScriptDeleteChangedResultCode (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_ROADPLANSTATE_SCRIPTDELETECHANGED_RESULTCODE);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoRoadPlanStatePlayScriptChangedResultCode (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_ROADPLANSTATE_PLAYSCRIPTCHANGED_RESULTCODE);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoSpeedSettingsStateOutdoorChangedOutdoor (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_SPEEDSETTINGSSTATE_OUTDOORCHANGED_OUTDOOR);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoMediaStreamingStateVideoEnableChangedEnabled (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_MEDIASTREAMINGSTATE_VIDEOENABLECHANGED_ENABLED);
}

JNIEXPORT jstring JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeStaticGetKeyJumpingSumoVideoSettingsStateAutorecordChangedEnabled (JNIEnv *env , jclass class)
{
    return (*env)->NewStringUTF(env, ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO_VIDEOSETTINGSSTATE_AUTORECORDCHANGED_ENABLED);
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendPilotingPCMD (JNIEnv *env, jobject thizz, jlong jFeature, jbyte _flag, jbyte _speed, jbyte _turn)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendPilotingPCMD (nativeFeature, _flag, _speed, _turn);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSetPilotingPCMD (JNIEnv *env, jobject thizz, jlong jFeature, jbyte _flag, jbyte _speed, jbyte _turn)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->setPilotingPCMD (nativeFeature, _flag, _speed, _turn);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSetPilotingPCMDFlag (JNIEnv *env, jobject thizz, jlong jFeature, jbyte _flag)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->setPilotingPCMDFlag (nativeFeature, _flag);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSetPilotingPCMDSpeed (JNIEnv *env, jobject thizz, jlong jFeature, jbyte _speed)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->setPilotingPCMDSpeed (nativeFeature, _speed);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSetPilotingPCMDTurn (JNIEnv *env, jobject thizz, jlong jFeature, jbyte _turn)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->setPilotingPCMDTurn (nativeFeature, _turn);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendPilotingPosture (JNIEnv *env, jobject thizz, jlong jFeature, jint _type)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendPilotingPosture (nativeFeature, _type);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendPilotingAddCapOffset (JNIEnv *env, jobject thizz, jlong jFeature, jfloat _offset)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendPilotingAddCapOffset (nativeFeature, _offset);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendAnimationsJumpStop (JNIEnv *env, jobject thizz, jlong jFeature)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendAnimationsJumpStop (nativeFeature);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendAnimationsJumpCancel (JNIEnv *env, jobject thizz, jlong jFeature)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendAnimationsJumpCancel (nativeFeature);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendAnimationsJumpLoad (JNIEnv *env, jobject thizz, jlong jFeature)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendAnimationsJumpLoad (nativeFeature);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendAnimationsJump (JNIEnv *env, jobject thizz, jlong jFeature, jint _type)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendAnimationsJump (nativeFeature, _type);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendAnimationsSimpleAnimation (JNIEnv *env, jobject thizz, jlong jFeature, jint _id)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendAnimationsSimpleAnimation (nativeFeature, _id);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendMediaRecordPicture (JNIEnv *env, jobject thizz, jlong jFeature, jbyte _mass_storage_id)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendMediaRecordPicture (nativeFeature, _mass_storage_id);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendMediaRecordVideo (JNIEnv *env, jobject thizz, jlong jFeature, jint _record, jbyte _mass_storage_id)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendMediaRecordVideo (nativeFeature, _record, _mass_storage_id);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendMediaRecordPictureV2 (JNIEnv *env, jobject thizz, jlong jFeature)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendMediaRecordPictureV2 (nativeFeature);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendMediaRecordVideoV2 (JNIEnv *env, jobject thizz, jlong jFeature, jint _record)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendMediaRecordVideoV2 (nativeFeature, _record);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendNetworkSettingsWifiSelection (JNIEnv *env, jobject thizz, jlong jFeature, jint _type, jint _band, jbyte _channel)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendNetworkSettingsWifiSelection (nativeFeature, _type, _band, _channel);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendNetworkWifiScan (JNIEnv *env, jobject thizz, jlong jFeature, jint _band)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendNetworkWifiScan (nativeFeature, _band);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendNetworkWifiAuthChannel (JNIEnv *env, jobject thizz, jlong jFeature)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendNetworkWifiAuthChannel (nativeFeature);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendAudioSettingsMasterVolume (JNIEnv *env, jobject thizz, jlong jFeature, jbyte _volume)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendAudioSettingsMasterVolume (nativeFeature, _volume);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendAudioSettingsTheme (JNIEnv *env, jobject thizz, jlong jFeature, jint _theme)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendAudioSettingsTheme (nativeFeature, _theme);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendRoadPlanAllScriptsMetadata (JNIEnv *env, jobject thizz, jlong jFeature)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendRoadPlanAllScriptsMetadata (nativeFeature);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendRoadPlanScriptUploaded (JNIEnv *env, jobject thizz, jlong jFeature, jstring _uuid, jstring _md5Hash)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    const char *nativeUuid = (*env)->GetStringUTFChars(env, _uuid, 0);
    const char *nativeMd5Hash = (*env)->GetStringUTFChars(env, _md5Hash, 0);
    
    error = nativeFeature->sendRoadPlanScriptUploaded (nativeFeature, (char *)nativeUuid, (char *)nativeMd5Hash);

    // cleanup
    (*env)->ReleaseStringUTFChars(env, _uuid, nativeUuid);
    (*env)->ReleaseStringUTFChars(env, _md5Hash, nativeMd5Hash);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendRoadPlanScriptDelete (JNIEnv *env, jobject thizz, jlong jFeature, jstring _uuid)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    const char *nativeUuid = (*env)->GetStringUTFChars(env, _uuid, 0);
    
    error = nativeFeature->sendRoadPlanScriptDelete (nativeFeature, (char *)nativeUuid);

    // cleanup
    (*env)->ReleaseStringUTFChars(env, _uuid, nativeUuid);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendRoadPlanPlayScript (JNIEnv *env, jobject thizz, jlong jFeature, jstring _uuid)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    const char *nativeUuid = (*env)->GetStringUTFChars(env, _uuid, 0);
    
    error = nativeFeature->sendRoadPlanPlayScript (nativeFeature, (char *)nativeUuid);

    // cleanup
    (*env)->ReleaseStringUTFChars(env, _uuid, nativeUuid);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendSpeedSettingsOutdoor (JNIEnv *env, jobject thizz, jlong jFeature, jbyte _outdoor)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendSpeedSettingsOutdoor (nativeFeature, _outdoor);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendMediaStreamingVideoEnable (JNIEnv *env, jobject thizz, jlong jFeature, jbyte _enable)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendMediaStreamingVideoEnable (nativeFeature, _enable);

    return error;
}

JNIEXPORT jint JNICALL
Java_com_parrot_arsdk_arcontroller_ARFeatureJumpingSumo_nativeSendVideoSettingsAutorecord (JNIEnv *env, jobject thizz, jlong jFeature, jbyte _enabled)
{
    // local declarations
    ARCONTROLLER_FEATURE_JumpingSumo_t *nativeFeature = (ARCONTROLLER_FEATURE_JumpingSumo_t*) (intptr_t) jFeature;
    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    
    error = nativeFeature->sendVideoSettingsAutorecord (nativeFeature, _enabled);

    return error;
}

