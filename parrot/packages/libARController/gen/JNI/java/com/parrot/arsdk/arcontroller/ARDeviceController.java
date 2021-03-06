/**********************************************************
 *            AUTOGENERATED FILE                          *
 *             DO NOT MODIFY IT                           *
 *                                                        *
 * To add new commands :                                  *
 *  - Modify ../Xml/commands.xml file                     *
 *  - Re-run generateDeviceControllers.py script          *
 *                                                        *
 **********************************************************/

/**
* @file ARDeviceController.java
* @brief Device controller allow to drive a device.
*/

package com.parrot.arsdk.arcontroller;

import com.parrot.arsdk.ardiscovery.ARDISCOVERY_PRODUCT_ENUM;
import com.parrot.arsdk.arsal.ARSALPrint;
import com.parrot.arsdk.arcommands.*;
import com.parrot.arsdk.ardiscovery.ARDiscoveryDevice;

import com.parrot.arsdk.arsal.ARNativeData;

import java.util.List;
import java.util.ArrayList;

public class ARDeviceController
{
    private static String TAG = "ARDeviceController";
    
    private static native void nativeStaticInit ();
    
    private native long nativeNew(long jdevice) throws ARControllerException;
    private native void nativeDelete(long jARDeviceController);
    
    private native int nativeStart (long jDeviceController);
    private native int nativeStop (long jDeviceController);
    
    private native long nativeGetFeatureARDrone3 (long jDeviceController);
    private native long nativeGetFeatureARDrone3Debug (long jDeviceController);
    private native long nativeGetFeatureJumpingSumo (long jDeviceController);
    private native long nativeGetFeatureJumpingSumoDebug (long jDeviceController);
    private native long nativeGetFeatureMiniDrone (long jDeviceController);
    private native long nativeGetFeatureMiniDroneDebug (long jDeviceController);
    private native long nativeGetFeatureSkyController (long jDeviceController);
    private native long nativeGetFeatureSkyControllerDebug (long jDeviceController);
    private native long nativeGetFeatureCommon (long jDeviceController);
    private native long nativeGetFeatureCommonDebug (long jDeviceController);
    private native long nativeGetFeaturePro (long jDeviceController);
    private native int nativeGetState (long jDeviceController) throws ARControllerException;
    private native int nativeGetExtensionState (long jDeviceController);
    private native String nativeGetExtensionName (long jDeviceController);
    private native int nativeGetExtensionProduct (long jDeviceController);
    private native long nativeGetCommandElements(long jDeviceController, int commandKey) throws ARControllerException;

    private long jniDeviceController;
    private boolean initOk;
    
    private List<ARDeviceControllerListener> listeners;
    private List<ARDeviceControllerStreamListener> streamlisteners;
    ARFeatureARDrone3 featureARDrone3;
    ARFeatureARDrone3Debug featureARDrone3Debug;
    ARFeatureJumpingSumo featureJumpingSumo;
    ARFeatureJumpingSumoDebug featureJumpingSumoDebug;
    ARFeatureMiniDrone featureMiniDrone;
    ARFeatureMiniDroneDebug featureMiniDroneDebug;
    ARFeatureSkyController featureSkyController;
    ARFeatureSkyControllerDebug featureSkyControllerDebug;
    ARFeatureCommon featureCommon;
    ARFeatureCommonDebug featureCommonDebug;
    ARFeaturePro featurePro;
    
    static
    {
        nativeStaticInit();
    }

    /**
     * Constructor
     */
    public ARDeviceController (ARDiscoveryDevice device) throws ARControllerException
    {
        initOk = false;
        ARCONTROLLER_ERROR_ENUM error = ARCONTROLLER_ERROR_ENUM.ARCONTROLLER_OK;
        
        if (device != null)
        {
            jniDeviceController = nativeNew(device.getNativeDevice());
        }
        else
        {
            error = ARCONTROLLER_ERROR_ENUM.ARCONTROLLER_ERROR_BAD_PARAMETER;
        }
        
        if (jniDeviceController != 0)
        {
            listeners = new ArrayList<ARDeviceControllerListener>();
            streamlisteners = new ArrayList<ARDeviceControllerStreamListener>();
            initOk = true;
            
            reloadFeatures();
        }

        if (error != ARCONTROLLER_ERROR_ENUM.ARCONTROLLER_OK)
        {
            throw new ARControllerException (error);
        }
    }

    /**
     * Dispose
     */
    public void dispose()
    {
        ARCONTROLLER_ERROR_ENUM error = ARCONTROLLER_ERROR_ENUM.ARCONTROLLER_OK;
        synchronized (this)
        {
            if(initOk == true)
            {
                nativeDelete(jniDeviceController);
                jniDeviceController = 0;
                initOk = false;
                
                if (featureARDrone3 != null)
                {
                    featureARDrone3.dispose();
                    featureARDrone3 = null;
                }
                
                if (featureARDrone3Debug != null)
                {
                    featureARDrone3Debug.dispose();
                    featureARDrone3Debug = null;
                }
                
                if (featureJumpingSumo != null)
                {
                    featureJumpingSumo.dispose();
                    featureJumpingSumo = null;
                }
                
                if (featureJumpingSumoDebug != null)
                {
                    featureJumpingSumoDebug.dispose();
                    featureJumpingSumoDebug = null;
                }
                
                if (featureMiniDrone != null)
                {
                    featureMiniDrone.dispose();
                    featureMiniDrone = null;
                }
                
                if (featureMiniDroneDebug != null)
                {
                    featureMiniDroneDebug.dispose();
                    featureMiniDroneDebug = null;
                }
                
                if (featureSkyController != null)
                {
                    featureSkyController.dispose();
                    featureSkyController = null;
                }
                
                if (featureSkyControllerDebug != null)
                {
                    featureSkyControllerDebug.dispose();
                    featureSkyControllerDebug = null;
                }
                
                if (featureCommon != null)
                {
                    featureCommon.dispose();
                    featureCommon = null;
                }
                
                if (featureCommonDebug != null)
                {
                    featureCommonDebug.dispose();
                    featureCommonDebug = null;
                }
                
                if (featurePro != null)
                {
                    featurePro.dispose();
                    featurePro = null;
                }
                
            }
            else
            {
                error = ARCONTROLLER_ERROR_ENUM.ARCONTROLLER_ERROR_JNI_INIT;
            }
        }
    }

    /**
     * Destructor
     */
    public void finalize () throws Throwable
    {
        try
        {
            dispose ();
        }
        finally
        {
            super.finalize ();
        }
    }
    
    public ARCONTROLLER_ERROR_ENUM start ()
    {
        ARCONTROLLER_ERROR_ENUM error = ARCONTROLLER_ERROR_ENUM.ARCONTROLLER_OK;
        synchronized (this)
        {
            if(initOk == true)
            {
                int nativeError = nativeStart(jniDeviceController);
                error = ARCONTROLLER_ERROR_ENUM.getFromValue(nativeError);
            }
            else
            {
                error = ARCONTROLLER_ERROR_ENUM.ARCONTROLLER_ERROR_JNI_INIT;
            }
        }
        
        return error;
    }
    
    public ARCONTROLLER_ERROR_ENUM stop ()
    {
        ARCONTROLLER_ERROR_ENUM error = ARCONTROLLER_ERROR_ENUM.ARCONTROLLER_OK;
        synchronized (this)
        {
            if(initOk == true)
            {
                int nativeError = nativeStop(jniDeviceController);
                error = ARCONTROLLER_ERROR_ENUM.getFromValue(nativeError);
            }
            else
            {
                error = ARCONTROLLER_ERROR_ENUM.ARCONTROLLER_ERROR_JNI_INIT;
            }
        }
        
        return error;
    }
    
    public ARFeatureARDrone3 getFeatureARDrone3 ()
    {
        return featureARDrone3;
    }
    
    public ARFeatureARDrone3Debug getFeatureARDrone3Debug ()
    {
        return featureARDrone3Debug;
    }
    
    public ARFeatureJumpingSumo getFeatureJumpingSumo ()
    {
        return featureJumpingSumo;
    }
    
    public ARFeatureJumpingSumoDebug getFeatureJumpingSumoDebug ()
    {
        return featureJumpingSumoDebug;
    }
    
    public ARFeatureMiniDrone getFeatureMiniDrone ()
    {
        return featureMiniDrone;
    }
    
    public ARFeatureMiniDroneDebug getFeatureMiniDroneDebug ()
    {
        return featureMiniDroneDebug;
    }
    
    public ARFeatureSkyController getFeatureSkyController ()
    {
        return featureSkyController;
    }
    
    public ARFeatureSkyControllerDebug getFeatureSkyControllerDebug ()
    {
        return featureSkyControllerDebug;
    }
    
    public ARFeatureCommon getFeatureCommon ()
    {
        return featureCommon;
    }
    
    public ARFeatureCommonDebug getFeatureCommonDebug ()
    {
        return featureCommonDebug;
    }
    
    public ARFeaturePro getFeaturePro ()
    {
        return featurePro;
    }
    
    public ARControllerDictionary getCommandElements (ARCONTROLLER_DICTIONARY_KEY_ENUM commandKey) throws ARControllerException
    {
        ARControllerDictionary elementDictionary = null;
        
        synchronized (this)
        {
            if(initOk == true)
            {
                if(commandKey != null)
                {
                    long nativeElementDictionary = nativeGetCommandElements(jniDeviceController, commandKey.getValue());
                    
                    elementDictionary = new ARControllerDictionary(nativeElementDictionary);
                }
            }
        }
        
        return elementDictionary;
    }
    public ARCONTROLLER_DEVICE_STATE_ENUM getState () throws ARControllerException
    {
        ARCONTROLLER_DEVICE_STATE_ENUM state = ARCONTROLLER_DEVICE_STATE_ENUM.ARCONTROLLER_DEVICE_STATE_MAX;
        synchronized (this)
        {
            if(initOk == true)
            {
                int nativeState = nativeGetState(jniDeviceController);
                state = ARCONTROLLER_DEVICE_STATE_ENUM.getFromValue(nativeState);
            }
            else
            {
                throw new ARControllerException(ARCONTROLLER_ERROR_ENUM.ARCONTROLLER_ERROR_JNI_INIT);
            }
        }
        
        return state;
    }
    
    public ARCONTROLLER_DEVICE_STATE_ENUM getExtensionState ()
    {
        ARCONTROLLER_DEVICE_STATE_ENUM extensionState = ARCONTROLLER_DEVICE_STATE_ENUM.ARCONTROLLER_DEVICE_STATE_MAX;
        synchronized (this)
        {
            if(initOk == true)
            {
                int nativeExtensionState = nativeGetExtensionState(jniDeviceController);
                extensionState = ARCONTROLLER_DEVICE_STATE_ENUM.getFromValue(nativeExtensionState);
            }
        }
        
        return extensionState;
    }
    
    public String getExtensionName ()
    {
        String extensionName = null;
        synchronized (this)
        {
            if(initOk == true)
            {
                extensionName = nativeGetExtensionName(jniDeviceController);
            }
        }
        
        return extensionName;
    }
    
    public ARDISCOVERY_PRODUCT_ENUM getExtensionProduct ()
    {
        ARDISCOVERY_PRODUCT_ENUM extensionProduct = ARDISCOVERY_PRODUCT_ENUM.ARDISCOVERY_PRODUCT_MAX;
        synchronized (this)
        {
            if(initOk == true)
            {
                int nativeExtensionProduct = nativeGetExtensionProduct(jniDeviceController);
                extensionProduct = ARDISCOVERY_PRODUCT_ENUM.getFromValue(nativeExtensionProduct);
            }
        }
        
        return extensionProduct;
    }
    
    public synchronized void addListener (ARDeviceControllerListener listener)
    {
       if (! listeners.contains(listener))
       {
           listeners.add (listener);
       }
    }
    
    public synchronized void removeListener (ARDeviceControllerListener listener)
    {
        listeners.remove (listener);
    }
    
    public synchronized void addStreamListener (ARDeviceControllerStreamListener listener)
    {
       if (! streamlisteners.contains(listener))
       {
           streamlisteners.add (listener);
       }
    }

    public synchronized void removeStreamListener (ARDeviceControllerStreamListener listener)
    {
        streamlisteners.remove (listener);
    }
    
    private void reloadFeatures()
    {
        long nativeFeatureARDrone3 = nativeGetFeatureARDrone3 (jniDeviceController);
        if ((featureARDrone3 == null) && (nativeFeatureARDrone3 != 0))
        {
            featureARDrone3 = new ARFeatureARDrone3(nativeFeatureARDrone3);
        }
        else
        {
            featureARDrone3 = null;
        }
        
        long nativeFeatureARDrone3Debug = nativeGetFeatureARDrone3Debug (jniDeviceController);
        if ((featureARDrone3Debug == null) && (nativeFeatureARDrone3Debug != 0))
        {
            featureARDrone3Debug = new ARFeatureARDrone3Debug(nativeFeatureARDrone3Debug);
        }
        else
        {
            featureARDrone3Debug = null;
        }
        
        long nativeFeatureJumpingSumo = nativeGetFeatureJumpingSumo (jniDeviceController);
        if ((featureJumpingSumo == null) && (nativeFeatureJumpingSumo != 0))
        {
            featureJumpingSumo = new ARFeatureJumpingSumo(nativeFeatureJumpingSumo);
        }
        else
        {
            featureJumpingSumo = null;
        }
        
        long nativeFeatureJumpingSumoDebug = nativeGetFeatureJumpingSumoDebug (jniDeviceController);
        if ((featureJumpingSumoDebug == null) && (nativeFeatureJumpingSumoDebug != 0))
        {
            featureJumpingSumoDebug = new ARFeatureJumpingSumoDebug(nativeFeatureJumpingSumoDebug);
        }
        else
        {
            featureJumpingSumoDebug = null;
        }
        
        long nativeFeatureMiniDrone = nativeGetFeatureMiniDrone (jniDeviceController);
        if ((featureMiniDrone == null) && (nativeFeatureMiniDrone != 0))
        {
            featureMiniDrone = new ARFeatureMiniDrone(nativeFeatureMiniDrone);
        }
        else
        {
            featureMiniDrone = null;
        }
        
        long nativeFeatureMiniDroneDebug = nativeGetFeatureMiniDroneDebug (jniDeviceController);
        if ((featureMiniDroneDebug == null) && (nativeFeatureMiniDroneDebug != 0))
        {
            featureMiniDroneDebug = new ARFeatureMiniDroneDebug(nativeFeatureMiniDroneDebug);
        }
        else
        {
            featureMiniDroneDebug = null;
        }
        
        long nativeFeatureSkyController = nativeGetFeatureSkyController (jniDeviceController);
        if ((featureSkyController == null) && (nativeFeatureSkyController != 0))
        {
            featureSkyController = new ARFeatureSkyController(nativeFeatureSkyController);
        }
        else
        {
            featureSkyController = null;
        }
        
        long nativeFeatureSkyControllerDebug = nativeGetFeatureSkyControllerDebug (jniDeviceController);
        if ((featureSkyControllerDebug == null) && (nativeFeatureSkyControllerDebug != 0))
        {
            featureSkyControllerDebug = new ARFeatureSkyControllerDebug(nativeFeatureSkyControllerDebug);
        }
        else
        {
            featureSkyControllerDebug = null;
        }
        
        long nativeFeatureCommon = nativeGetFeatureCommon (jniDeviceController);
        if ((featureCommon == null) && (nativeFeatureCommon != 0))
        {
            featureCommon = new ARFeatureCommon(nativeFeatureCommon);
        }
        else
        {
            featureCommon = null;
        }
        
        long nativeFeatureCommonDebug = nativeGetFeatureCommonDebug (jniDeviceController);
        if ((featureCommonDebug == null) && (nativeFeatureCommonDebug != 0))
        {
            featureCommonDebug = new ARFeatureCommonDebug(nativeFeatureCommonDebug);
        }
        else
        {
            featureCommonDebug = null;
        }
        
        long nativeFeaturePro = nativeGetFeaturePro (jniDeviceController);
        if ((featurePro == null) && (nativeFeaturePro != 0))
        {
            featurePro = new ARFeaturePro(nativeFeaturePro);
        }
        else
        {
            featurePro = null;
        }
        
    }
    
    private void onStateChanged (int newState, int error)
    {
        for (ARDeviceControllerListener l : listeners)
        {
            l.onStateChanged (this, ARCONTROLLER_DEVICE_STATE_ENUM.getFromValue(newState), ARCONTROLLER_ERROR_ENUM.getFromValue(error));
        }
    }
    
    private void onExtensionStateChanged (int newState, int product, String name, int error)
    {
        // reload the features because they might have changed
        reloadFeatures();
        for (ARDeviceControllerListener l : listeners)
        {
            l.onExtensionStateChanged (this, ARCONTROLLER_DEVICE_STATE_ENUM.getFromValue(newState), ARDISCOVERY_PRODUCT_ENUM.getFromValue(product), name, ARCONTROLLER_ERROR_ENUM.getFromValue(error));
        }
    }
    
    private void onCommandReceived (int nativeCommandKey, long elementDictionary)
    {
        ARCONTROLLER_DICTIONARY_KEY_ENUM commandKey = ARCONTROLLER_DICTIONARY_KEY_ENUM.getFromValue(nativeCommandKey);
        
        ARControllerDictionary dictionary = new ARControllerDictionary(elementDictionary);
        
        for (ARDeviceControllerListener l : listeners)
        {
            l.onCommandReceived (this, ARCONTROLLER_DICTIONARY_KEY_ENUM.getFromValue(nativeCommandKey), dictionary);
        }
    }
    
    private int decoderConfigCallback (ARControllerCodec codec)
    {
        boolean failed = false;
        
        for (ARDeviceControllerStreamListener l : streamlisteners)
        {
            ARCONTROLLER_ERROR_ENUM error = l.configureDecoder(this, codec);
            
            if (error != ARCONTROLLER_ERROR_ENUM.ARCONTROLLER_OK)
            {
                failed = true;
            }
        }
        
        codec.dispose();
        
        return (failed) ? ARCONTROLLER_ERROR_ENUM.ARCONTROLLER_ERROR.getValue() : ARCONTROLLER_ERROR_ENUM.ARCONTROLLER_OK.getValue();
    }
    
    private int didReceiveFrameCallback (long data, int dataCapacity, int dataSize, int nativeIsIFrame, int missed)
    {
        boolean failed = false;
        boolean isIFrame = (nativeIsIFrame != 0);
        ARFrame frame = new ARFrame (data, dataCapacity, dataSize, isIFrame, missed);
        
        for (ARDeviceControllerStreamListener l : streamlisteners)
        {
            ARCONTROLLER_ERROR_ENUM error = l.onFrameReceived (this, frame);
            
            if (error != ARCONTROLLER_ERROR_ENUM.ARCONTROLLER_OK)
            {
                failed = true;
            }
        }
        
        frame.dispose();
        
        return (failed) ? ARCONTROLLER_ERROR_ENUM.ARCONTROLLER_ERROR.getValue() : ARCONTROLLER_ERROR_ENUM.ARCONTROLLER_OK.getValue();
    }

    private void  timeoutFrameCallback ()
    {
        for (ARDeviceControllerStreamListener l : streamlisteners)
        {
            l.onFrameTimeout (this);
        }
    }
}

