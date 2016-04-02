#ifndef _SDK_EXAMPLE_JS_H_
#define _SDK_EXAMPLE_JS_H_

#include <ihm.h>

typedef struct
{
    uint8_t flag;
    int8_t  speed;
    int8_t  turn;
}JS_PCMD_t;

typedef struct READER_THREAD_DATA_t READER_THREAD_DATA_t;


typedef struct
{
    ARNETWORKAL_Manager_t *alManager;
    ARNETWORK_Manager_t *netManager;
    ARSTREAM_Reader_t *streamReader;
    ARSAL_Thread_t rxThread;
    ARSAL_Thread_t txThread;
    ARSAL_Thread_t videoTxThread;
    ARSAL_Thread_t videoRxThread;
    int d2cPort;
    int c2dPort;
    int arstreamFragSize;
    int arstreamFragNb;
    int arstreamAckDelay;
    uint8_t *videoFrame;
    uint32_t videoFrameSize;
    
    FILE *video_out;
    int writeImgs;
    int frameNb;
    
    ARSAL_Thread_t looperThread;
    ARSAL_Thread_t *readerThreads;
    READER_THREAD_DATA_t *readerThreadsData;
    
    int run;
    JS_PCMD_t dataPCMD;
    
} JS_MANAGER_t;

struct READER_THREAD_DATA_t
{
    JS_MANAGER_t *jsManager;
    int readerBufferId;
};

int ardiscoveryConnect (JS_MANAGER_t *jsManager);
eARDISCOVERY_ERROR ARDISCOVERY_Connection_SendJsonCallback (uint8_t *dataTx, uint32_t *dataTxSize, void *customData);
eARDISCOVERY_ERROR ARDISCOVERY_Connection_ReceiveJsonCallback (uint8_t *dataRx, uint32_t dataRxSize, char *ip, void *customData);

int startNetwork (JS_MANAGER_t *jsManager);
void onDisconnectNetwork (ARNETWORK_Manager_t *manager, ARNETWORKAL_Manager_t *alManager, void *customData);
void stopNetwork (JS_MANAGER_t *jsManager);

void registerARCommandsCallbacks (IHM_t *ihm);
void unregisterARCommandsCallbacks();

int startVideo (JS_MANAGER_t *jsManager);
uint8_t *frameCompleteCallback (eARSTREAM_READER_CAUSE cause, uint8_t *frame, uint32_t frameSize, int numberOfSkippedFrames, int isFlushFrame, uint32_t *newBufferCapacity, void *custom);
void stopVideo (JS_MANAGER_t *jsManager);

int sendBeginStream(JS_MANAGER_t *jsManager);

void *looperRun(void* data);
void *readerRun(void* data);

int sendJump(JS_MANAGER_t *jsManager);
int sendPCMD(JS_MANAGER_t *jsManager);

eARNETWORK_MANAGER_CALLBACK_RETURN arnetworkCmdCallback(int buffer_id, uint8_t *data, void *custom, eARNETWORK_MANAGER_CALLBACK_STATUS cause);

void batteryStateChangedCallback (uint8_t percent, void *custom);

/* IHM callbacks: */
void onInputEvent (eIHM_INPUT_EVENT event, void *customData);
int customPrintCallback (eARSAL_PRINT_LEVEL level, const char *tag, const char *format, va_list va);

#endif /* _SDK_EXAMPLE_JS_H_ */
