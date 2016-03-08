/**********************************************************
 *            AUTOGENERATED FILE                          *
 *             DO NOT MODIFY IT                           *
 *                                                        *
 * To add new commands :                                  *
 *  - Modify ../Xml/commands.xml file                     *
 *  - Re-run generateDictionaryKeyEnum.py script          *
 *                                                        *
 **********************************************************/

/**
* @file ARCONTROLLER_DICTIONARY_Key.c
* @brief .h
*/

#include <stdio.h>

#include <libARController/ARCONTROLLER_DICTIONARY_Key.h>

eARCONTROLLER_DICTIONARY_KEY ARCONTROLLER_DICTIONARY_Key_GetFeatureFromCommandKey (eARCONTROLLER_DICTIONARY_KEY commandKey)
{
    // -- Get Feature From Command Key --
    
    eARCONTROLLER_DICTIONARY_KEY featrueKey = ARCONTROLLER_DICTIONARY_DICTIONARY_KEY_MAX;
    
    // find feature parameters
    if (ARCONTROLLER_DICTIONARY_KEY_ARDRONE3 <= commandKey && commandKey < ARCONTROLLER_DICTIONARY_KEY_ARDRONE3DEBUG)
    {
        featrueKey = ARCONTROLLER_DICTIONARY_KEY_ARDRONE3;
    }
    else if (ARCONTROLLER_DICTIONARY_KEY_ARDRONE3DEBUG <= commandKey && commandKey < ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO)
    {
        featrueKey = ARCONTROLLER_DICTIONARY_KEY_ARDRONE3DEBUG;
    }
    else if (ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO <= commandKey && commandKey < ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMODEBUG)
    {
        featrueKey = ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMO;
    }
    else if (ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMODEBUG <= commandKey && commandKey < ARCONTROLLER_DICTIONARY_KEY_MINIDRONE)
    {
        featrueKey = ARCONTROLLER_DICTIONARY_KEY_JUMPINGSUMODEBUG;
    }
    else if (ARCONTROLLER_DICTIONARY_KEY_MINIDRONE <= commandKey && commandKey < ARCONTROLLER_DICTIONARY_KEY_MINIDRONEDEBUG)
    {
        featrueKey = ARCONTROLLER_DICTIONARY_KEY_MINIDRONE;
    }
    else if (ARCONTROLLER_DICTIONARY_KEY_MINIDRONEDEBUG <= commandKey && commandKey < ARCONTROLLER_DICTIONARY_KEY_SKYCONTROLLER)
    {
        featrueKey = ARCONTROLLER_DICTIONARY_KEY_MINIDRONEDEBUG;
    }
    else if (ARCONTROLLER_DICTIONARY_KEY_SKYCONTROLLER <= commandKey && commandKey < ARCONTROLLER_DICTIONARY_KEY_SKYCONTROLLERDEBUG)
    {
        featrueKey = ARCONTROLLER_DICTIONARY_KEY_SKYCONTROLLER;
    }
    else if (ARCONTROLLER_DICTIONARY_KEY_SKYCONTROLLERDEBUG <= commandKey && commandKey < ARCONTROLLER_DICTIONARY_KEY_COMMON)
    {
        featrueKey = ARCONTROLLER_DICTIONARY_KEY_SKYCONTROLLERDEBUG;
    }
    else if (ARCONTROLLER_DICTIONARY_KEY_COMMON <= commandKey && commandKey < ARCONTROLLER_DICTIONARY_KEY_COMMONDEBUG)
    {
        featrueKey = ARCONTROLLER_DICTIONARY_KEY_COMMON;
    }
    else if (ARCONTROLLER_DICTIONARY_KEY_COMMONDEBUG <= commandKey && commandKey < ARCONTROLLER_DICTIONARY_KEY_PRO)
    {
        featrueKey = ARCONTROLLER_DICTIONARY_KEY_COMMONDEBUG;
    }
    
    return featrueKey;
}
