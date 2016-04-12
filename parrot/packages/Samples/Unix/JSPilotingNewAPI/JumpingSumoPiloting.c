/**
 * @file JumpingSumoReceiveStream.c
 * @brief This file contains sources about basic arsdk example receiving video stream from a JumpingSumo
 * @date 08/01/2015
 */

/*****************************************
 *
 *             include file :
 *
 *****************************************/

#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <libARSAL/ARSAL.h>
#include <libARSAL/ARSAL_Print.h>
#include <libARNetwork/ARNetwork.h>
#include <libARNetworkAL/ARNetworkAL.h>
#include <libARCommands/ARCommands.h>
#include <libARDiscovery/ARDiscovery.h>
#include <libARStream/ARStream.h>

#include "JumpingSumoPiloting.h"
#include "ihm.h"

/*****************************************
 *
 *             define :
 *
 *****************************************/
#define TAG "JumpingSumoPiloting"
#define JS_IP_ADDRESS "192.168.2.1"
#define JS_DISCOVERY_PORT 44444
#define JS_C2D_PORT 54321 // should be read from Json
#define JS_D2C_PORT 43210

#define JS_NET_CD_NONACK_ID 10
#define JS_NET_CD_ACK_ID 11
#define JS_NET_CD_VIDEO_ACK_ID 13
#define JS_NET_DC_NONACK_ID 127
#define JS_NET_DC_ACK_ID 126
#define JS_NET_DC_VIDEO_ID 125

#define DISPLAY_WITH_FFPLAY 1

#define ERROR_STR_LENGTH 2048

/*****************************************
 *
 *             implementation :
 *
 *****************************************/

static char fifo_file_name[256] = "";

static ARNETWORK_IOBufferParam_t c2dParams[] = {
    {
        .ID = JS_NET_CD_NONACK_ID,
        .dataType = ARNETWORKAL_FRAME_TYPE_DATA,
        .sendingWaitTimeMs = 5,
        .ackTimeoutMs = -1,
        .numberOfRetry = -1,
        .numberOfCell = 10,
        .dataCopyMaxSize = 128,
        .isOverwriting = 0,
    },
    {
        .ID = JS_NET_CD_ACK_ID,
        .dataType = ARNETWORKAL_FRAME_TYPE_DATA_WITH_ACK,
        .sendingWaitTimeMs = 20,
        .ackTimeoutMs = 500,
        .numberOfRetry = 3,
        .numberOfCell = 20,
        .dataCopyMaxSize = 128,
        .isOverwriting = 0,
    },
    {
        .ID = JS_NET_CD_VIDEO_ACK_ID,
        .dataType = ARNETWORKAL_FRAME_TYPE_UNINITIALIZED,
        .sendingWaitTimeMs = 0,
        .ackTimeoutMs = 0,
        .numberOfRetry = 0,
        .numberOfCell = 0,
        .dataCopyMaxSize = 0,
        .isOverwriting = 0,
    }
};
static const size_t numC2dParams = sizeof(c2dParams) / sizeof(ARNETWORK_IOBufferParam_t);

static ARNETWORK_IOBufferParam_t d2cParams[] = {
    {
        .ID = ((ARNETWORKAL_MANAGER_WIFI_ID_MAX / 2) - 1),
        .dataType = ARNETWORKAL_FRAME_TYPE_DATA,
        .sendingWaitTimeMs = 20,
        .ackTimeoutMs = -1,
        .numberOfRetry = -1,
        .numberOfCell = 10,
        .dataCopyMaxSize = 128,
        .isOverwriting = 0,
    },
    {
        .ID = ((ARNETWORKAL_MANAGER_WIFI_ID_MAX / 2) - 2),
        .dataType = ARNETWORKAL_FRAME_TYPE_DATA_WITH_ACK,
        .sendingWaitTimeMs = 20,
        .ackTimeoutMs = 500,
        .numberOfRetry = 3,
        .numberOfCell = 20,
        .dataCopyMaxSize = 128,
        .isOverwriting = 0,
    },
    {
        .ID = JS_NET_DC_VIDEO_ID,
        .dataType = ARNETWORKAL_FRAME_TYPE_UNINITIALIZED,
        .sendingWaitTimeMs = 0,
        .ackTimeoutMs = 0,
        .numberOfRetry = 0,
        .numberOfCell = 0,
        .dataCopyMaxSize = 0,
        .isOverwriting = 0,
    }
};
static const size_t numD2cParams = sizeof(d2cParams) / sizeof(ARNETWORK_IOBufferParam_t);

static int commandBufferIds[] = {
    JS_NET_DC_NONACK_ID,
    JS_NET_DC_ACK_ID,
};
static const size_t numOfCommandBufferIds = sizeof(commandBufferIds) / sizeof(int);

static int idToIndex(ARNETWORK_IOBufferParam_t* params, size_t num_params, int id)
{
    int i = 0;
    for (i = 0; i < num_params; i ++)
    {
        if (params[i].ID == id)
        {
            return i;
        }
    }
    return -1;
}

int gIHMRun = 1;
char gErrorStr[ERROR_STR_LENGTH];

int main (int argc, char *argv[])
{
    /* local declarations */
    int failed = 0;
    int writeImgs = 0;
    JS_MANAGER_t *jsManager = malloc(sizeof(JS_MANAGER_t));

    pid_t child = 0;

    ARSAL_PRINT(ARSAL_PRINT_INFO, TAG, "-- Jumping Sumo Receive Video Stream --");
    
    if (DISPLAY_WITH_FFPLAY)
    {
        // fork the process to launch ffplay
        if ((child = fork()) == 0)
        {
            execlp("ffplay", "ffplay", "-i", "video_fifo", "-f", "mjpeg", NULL);
            ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "Missing avplay, you will not see the video. Please install avplay.");
            return -1;
        }
    }
    else
    {
        // create the video folder to store video images
        char answer = 'N';
        ARSAL_PRINT(ARSAL_PRINT_INFO, TAG, "Do you want to write image files on your file system ? You should have at least 50Mb. Y or N");
        scanf("%c", &answer);
        if (answer == 'Y' || answer == 'y')
        {
            ARSAL_PRINT(ARSAL_PRINT_INFO, TAG, "You choose to write image files.");
            writeImgs = 1;
            mkdir("video", S_IRWXU);
        }
        else
        {
            ARSAL_PRINT(ARSAL_PRINT_INFO, TAG, "You did not choose to write image files.");
        }
    }
    
    ARSAL_PRINT (ARSAL_PRINT_INFO, TAG, "-- Starting --");
    ARSAL_PRINT(ARSAL_PRINT_INFO, TAG, "-- Jumping Sumo Piloting --");

    IHM_t *ihm = IHM_New (&onInputEvent);
		if (ihm != NULL)
		{
			gErrorStr[0] = '\0';
			ARSAL_Print_SetCallback (customPrintCallback); //use a custom callback to print, for not disturb ncurses IHM
			
			IHM_PrintHeader(ihm, "-- Jumping Sumo Piloting --");
			registerARCommandsCallbacks (ihm);
		}
		else
		{
			ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "Creation of IHM failed.");
			failed = 1;
		}

    if (jsManager != NULL)
    {
        // initialize jsMnager
        jsManager->alManager = NULL;
        jsManager->netManager = NULL;
        jsManager->streamReader = NULL;
        jsManager->rxThread = NULL;
        jsManager->txThread = NULL;
        jsManager->videoRxThread = NULL;
        jsManager->videoTxThread = NULL;
        jsManager->d2cPort = JS_D2C_PORT;
        jsManager->c2dPort = JS_C2D_PORT; //jsManager->c2dPort = 0; // Should be read from json
        jsManager->arstreamAckDelay = 0; // Should be read from json
        jsManager->arstreamFragSize = 65000; // Should be read from json
        jsManager->arstreamFragNb   = 4; // Should be read from json
        
        if (DISPLAY_WITH_FFPLAY)
        {
            jsManager->video_out = fopen("./video_fifo", "w");
        }
        jsManager->frameNb = 0;
        jsManager->writeImgs = writeImgs;
				
		jsManager->looperThread = NULL;
		jsManager->readerThreads = NULL;
		jsManager->readerThreadsData = NULL;
				
		jsManager->run = 1;
				
		jsManager->dataPCMD.flag = 0;
		jsManager->dataPCMD.speed = 0;
		jsManager->dataPCMD.turn = 0;
		
		IHM_setCustomData(ihm, jsManager);
    }
    else
    {
        failed = 1;
        ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "jsManager alloc error !");
    }

    if (!failed)
    {
        failed = ardiscoveryConnect (jsManager);
    }

    if (!failed)
    {
        ARSTREAM_Reader_InitStreamDataBuffer (&d2cParams[2], JS_NET_DC_VIDEO_ID, jsManager->arstreamFragSize, jsManager->arstreamFragNb);
        ARSTREAM_Reader_InitStreamAckBuffer (&c2dParams[2], JS_NET_CD_VIDEO_ACK_ID);
    }

    if (!failed)
    {
        /* start */
        failed = startNetwork (jsManager);
    }
    
    if (!failed)
	{
		// Create and start looper thread.
		if (ARSAL_Thread_Create(&(jsManager->looperThread), looperRun, jsManager) != 0)
		{
			ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "Creation of looper thread failed.");
			failed = 1;
		}
	}
	
	if (!failed)
	{
		// allocate reader thread array.
		jsManager->readerThreads = calloc(numOfCommandBufferIds, sizeof(ARSAL_Thread_t));
		
		if (jsManager->readerThreads == NULL)
		{
			ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "Allocation of reader threads failed.");
			failed = 1;
		}
	}
	
	if (!failed)
	{
		// allocate reader thread data array.
		jsManager->readerThreadsData = calloc(numOfCommandBufferIds, sizeof(READER_THREAD_DATA_t));
		
		if (jsManager->readerThreadsData == NULL)
		{
			ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "Allocation of reader threads data failed.");
			failed = 1;
		}
	}
	
	if (!failed)
	{
		// Create and start reader threads.
		int readerThreadIndex = 0;
		for (readerThreadIndex = 0 ; readerThreadIndex < numOfCommandBufferIds ; readerThreadIndex++)
		{
			// initialize reader thread data
			jsManager->readerThreadsData[readerThreadIndex].jsManager = jsManager;
			jsManager->readerThreadsData[readerThreadIndex].readerBufferId = commandBufferIds[readerThreadIndex];
			
			if (ARSAL_Thread_Create(&(jsManager->readerThreads[readerThreadIndex]), readerRun, &(jsManager->readerThreadsData[readerThreadIndex])) != 0)
			{
				ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "Creation of reader thread failed.");
				failed = 1;
			}
		}
	}

    if (!failed)
    {
        failed = startVideo (jsManager);
    }

    if (!failed)
    {
        int cmdSend = 0;

        cmdSend = sendBeginStream(jsManager);

        //sleep(30);

		if (!failed)
		{
			IHM_PrintInfo(ihm, "Running ... (Arrow keys to move ; Spacebar to jump ; 'q' to quit)");
			
			while (gIHMRun)
			{
				usleep(50);
			}
			
			IHM_PrintInfo(ihm, "Disconnecting ...");
		}
    }


    if (jsManager != NULL)
    {
        /* stop */
        jsManager->run = 0; // break threads loops
        
        // Stop looper Thread
        if (jsManager->looperThread != NULL)
        {
            ARSAL_Thread_Join(jsManager->looperThread, NULL);
            ARSAL_Thread_Destroy(&(jsManager->looperThread));
            jsManager->looperThread = NULL;
        }
        
        if (jsManager->readerThreads != NULL)
        {
            // Stop reader Threads
            int readerThreadIndex = 0;
            for (readerThreadIndex = 0 ; readerThreadIndex < numD2cParams ; readerThreadIndex++)
            {
                if (jsManager->readerThreads[readerThreadIndex] != NULL)
                {
                    ARSAL_Thread_Join(jsManager->readerThreads[readerThreadIndex], NULL);
                    ARSAL_Thread_Destroy(&(jsManager->readerThreads[readerThreadIndex]));
                    jsManager->readerThreads[readerThreadIndex] = NULL;
                }
            }
            
            // free reader thread array
            free (jsManager->readerThreads);
            jsManager->readerThreads = NULL;
        }
        
        if (jsManager->readerThreadsData != NULL)
        {
            // free reader thread data array
            free (jsManager->readerThreadsData);
            jsManager->readerThreadsData = NULL;
        }
        
        stopVideo (jsManager);
        stopNetwork (jsManager);

        if (DISPLAY_WITH_FFPLAY)
        {
            fclose (jsManager->video_out);
        }
        free (jsManager);
    }

    ARSAL_PRINT(ARSAL_PRINT_INFO, TAG, "-- END --");

    if (DISPLAY_WITH_FFPLAY)
    {
        if (child > 0)
        {
            kill(child, SIGKILL);
        }
    }
    
    system("reset");

    return 0;
}

int ardiscoveryConnect (JS_MANAGER_t *jsManager)
{
    int failed = 0;

    ARSAL_PRINT(ARSAL_PRINT_INFO, TAG, "- ARDiscovery Connection");

    eARDISCOVERY_ERROR err = ARDISCOVERY_OK;
    ARDISCOVERY_Connection_ConnectionData_t *discoveryData = ARDISCOVERY_Connection_New (ARDISCOVERY_Connection_SendJsonCallback, ARDISCOVERY_Connection_ReceiveJsonCallback, jsManager, &err);
    if (discoveryData == NULL || err != ARDISCOVERY_OK)
    {
        ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "Error while creating discoveryData : %s", ARDISCOVERY_Error_ToString(err));
        failed = 1;
    }

    if (!failed)
    {
        eARDISCOVERY_ERROR err = ARDISCOVERY_Connection_ControllerConnection(discoveryData, JS_DISCOVERY_PORT, JS_IP_ADDRESS);
        if (err != ARDISCOVERY_OK)
        {
            ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "Error while opening discovery connection : %s", ARDISCOVERY_Error_ToString(err));
            failed = 1;
        }
    }

    ARDISCOVERY_Connection_Delete(&discoveryData);

    return failed;
}

int startNetwork (JS_MANAGER_t *jsManager)
{
    int failed = 0;
    eARNETWORK_ERROR netError = ARNETWORK_OK;
    eARNETWORKAL_ERROR netAlError = ARNETWORKAL_OK;
    int pingDelay = 0; // 0 means default, -1 means no ping

    ARSAL_PRINT(ARSAL_PRINT_INFO, TAG, "- Start ARNetwork");

    // Create the ARNetworkALManager
    jsManager->alManager = ARNETWORKAL_Manager_New(&netAlError);
    if (netAlError != ARNETWORKAL_OK)
    {
        failed = 1;
    }

    if (!failed)
    {
        // Initilize the ARNetworkALManager
        netAlError = ARNETWORKAL_Manager_InitWifiNetwork(jsManager->alManager, JS_IP_ADDRESS, JS_C2D_PORT, JS_D2C_PORT, 1);
        if (netAlError != ARNETWORKAL_OK)
        {
            failed = 1;
        }
    }

    if (!failed)
    {
        // Create the ARNetworkManager.
        jsManager->netManager = ARNETWORK_Manager_New(jsManager->alManager, numC2dParams, c2dParams, numD2cParams, d2cParams, pingDelay, onDisconnectNetwork, jsManager, &netError);
        if (netError != ARNETWORK_OK)
        {
            failed = 1;
        }
    }

    if (!failed)
    {
        // Create and start Tx and Rx threads.
        if (ARSAL_Thread_Create(&(jsManager->rxThread), ARNETWORK_Manager_ReceivingThreadRun, jsManager->netManager) != 0)
        {
            ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "Creation of Rx thread failed.");
            failed = 1;
        }

        if (ARSAL_Thread_Create(&(jsManager->txThread), ARNETWORK_Manager_SendingThreadRun, jsManager->netManager) != 0)
        {
            ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "Creation of Tx thread failed.");
            failed = 1;
        }
    }

    // Print net error
    if (failed)
    {
        if (netAlError != ARNETWORKAL_OK)
        {
            ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "ARNetWorkAL Error : %s", ARNETWORKAL_Error_ToString(netAlError));
        }

        if (netError != ARNETWORK_OK)
        {
            ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "ARNetWork Error : %s", ARNETWORK_Error_ToString(netError));
        }
    }

    return failed;
}

void stopNetwork (JS_MANAGER_t *jsManager)
{
    int failed = 0;
    eARNETWORK_ERROR netError = ARNETWORK_OK;
    eARNETWORKAL_ERROR netAlError = ARNETWORKAL_OK;
    int pingDelay = 0; // 0 means default, -1 means no ping

    ARSAL_PRINT(ARSAL_PRINT_INFO, TAG, "- Stop ARNetwork");

    // ARNetwork cleanup
    if (jsManager->netManager != NULL)
    {
        ARNETWORK_Manager_Stop(jsManager->netManager);
        if (jsManager->rxThread != NULL)
        {
            ARSAL_Thread_Join(jsManager->rxThread, NULL);
            ARSAL_Thread_Destroy(&(jsManager->rxThread));
            jsManager->rxThread = NULL;
        }

        if (jsManager->txThread != NULL)
        {
            ARSAL_Thread_Join(jsManager->txThread, NULL);
            ARSAL_Thread_Destroy(&(jsManager->txThread));
            jsManager->txThread = NULL;
        }
    }

    if (jsManager->alManager != NULL)
    {
        ARNETWORKAL_Manager_Unlock(jsManager->alManager);

        ARNETWORKAL_Manager_CloseWifiNetwork(jsManager->alManager);
    }

    ARNETWORK_Manager_Delete(&(jsManager->netManager));
    ARNETWORKAL_Manager_Delete(&(jsManager->alManager));
}

void onDisconnectNetwork (ARNETWORK_Manager_t *manager, ARNETWORKAL_Manager_t *alManager, void *customData)
{
    ARSAL_PRINT(ARSAL_PRINT_DEBUG, TAG, "onDisconnectNetwork ...");
    gIHMRun = 0; //stop IHM
}

int startVideo(JS_MANAGER_t *jsManager)
{
    int failed = 0;
    eARSTREAM_ERROR err = ARSTREAM_OK;

    ARSAL_PRINT(ARSAL_PRINT_INFO, TAG, "- Start ARStream");

    jsManager->videoFrameSize = jsManager->arstreamFragSize * jsManager->arstreamFragNb;
    jsManager->videoFrame = malloc (jsManager->videoFrameSize);
    if (jsManager->videoFrame == NULL)
    {
        failed = 1;
    }

    if (! failed)
    {
        jsManager->streamReader = ARSTREAM_Reader_New (jsManager->netManager, JS_NET_DC_VIDEO_ID, JS_NET_CD_VIDEO_ACK_ID, frameCompleteCallback, jsManager->videoFrame, jsManager->videoFrameSize, jsManager->arstreamFragSize, jsManager->arstreamAckDelay, jsManager, &err);
        if (err != ARSTREAM_OK)
        {
            ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "Error during ARStream creation : %s", ARSTREAM_Error_ToString(err));
            failed = 1;
        }
    }

    if (!failed)
    {
        // Create and start Tx and Rx threads.
        if (ARSAL_Thread_Create(&(jsManager->videoRxThread), ARSTREAM_Reader_RunDataThread, jsManager->streamReader) != 0)
        {
            ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "Creation of video Rx thread failed.");
            failed = 1;
        }

        if (ARSAL_Thread_Create(&(jsManager->videoTxThread), ARSTREAM_Reader_RunAckThread, jsManager->streamReader) != 0)
        {
            ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "Creation of video Tx thread failed.");
            failed = 1;
        }
    }

    return failed;
}

void stopVideo(JS_MANAGER_t *jsManager)
{
    int failed = 0;
    eARSTREAM_ERROR err = ARSTREAM_OK;

    ARSAL_PRINT(ARSAL_PRINT_INFO, TAG, "- Stop ARStream");

    if (jsManager->streamReader)
    {
        ARSTREAM_Reader_StopReader(jsManager->streamReader);

        // Optionnal, but better for speed:
        ARNETWORKAL_Manager_Unlock(jsManager->alManager);

        if (jsManager->videoRxThread != NULL)
        {
            ARSAL_Thread_Join(jsManager->videoRxThread, NULL);
            ARSAL_Thread_Destroy(&(jsManager->videoRxThread));
            jsManager->videoRxThread = NULL;
        }
        if (jsManager->videoTxThread != NULL)
        {
            ARSAL_Thread_Join(jsManager->videoTxThread, NULL);
            ARSAL_Thread_Destroy(&(jsManager->videoTxThread));
            jsManager->videoTxThread = NULL;
        }

        ARSTREAM_Reader_Delete (&(jsManager->streamReader));
    }

    if (jsManager->videoFrame)
    {
        free (jsManager->videoFrame);
        jsManager->videoFrame = NULL;
    }
}

uint8_t *frameCompleteCallback (eARSTREAM_READER_CAUSE cause, uint8_t *frame, uint32_t frameSize, int numberOfSkippedFrames, int isFlushFrame, uint32_t *newBufferCapacity, void *custom)
{
    uint8_t *ret = NULL;
    JS_MANAGER_t *jsManager = (JS_MANAGER_t *)custom;
    
    switch(cause)
    {
        case ARSTREAM_READER_CAUSE_FRAME_COMPLETE:
            /* Here, the mjpeg video frame is in the "frame" pointer, with size "frameSize" bytes
             You can do what you want, but keep it as short as possible, as the video is blocked until you return from this callback.
             Typically, you will either copy the frame and return the same buffer to the library, or store the buffer
             in a fifo for pending operations, and provide a new one.
             In this sample, we do nothing and just pass the buffer back*/
            
            ret = jsManager->videoFrame;
            *newBufferCapacity = jsManager->videoFrameSize;
            
            /* Again, don't write files in this thread, that is just for the example :) */
            if (DISPLAY_WITH_FFPLAY)
            {
                // write img files
                fwrite(frame, frameSize, 1, jsManager->video_out);
                fflush (jsManager->video_out);
            }
            else if (jsManager->writeImgs)
            {
                
                char filename[20];
                snprintf(filename, sizeof(filename), "video/img_%d.jpg", jsManager->frameNb);
                
                jsManager->frameNb++;
                FILE *img = fopen(filename, "w");
                fwrite(frame, frameSize, 1, img);
                fclose(img);
            }
            
            // Send frame through a socket
            /*int sockfd, mlen;
			struct sockaddr_in serv_addr;
			int addrsize = sizeof(struct sockaddr_in);
			sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (sockfd < 0) 
				perror("ERROR opening socket");
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_port = htons(32324);
			unsigned int in_address = 127 << 24 | 0 << 16 | 0 << 8 | 1;
			serv_addr.sin_addr.s_addr = htonl (in_address);

			printf("Frame size = %i\n", frameSize);
			 printf("Sending frame...\n");
			 if (sendto(sockfd, frame, 500000, 0, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0)
					perror("ERROR with sendto()");
			 printf("...Sent\n");*/
			 
            
            break;
        case ARSTREAM_READER_CAUSE_FRAME_TOO_SMALL:
            /* This case should not happen, as we've allocated a frame pointer to the maximum possible size. */
            ret = jsManager->videoFrame;
            *newBufferCapacity = jsManager->videoFrameSize;
            break;
        case ARSTREAM_READER_CAUSE_COPY_COMPLETE:
            /* Same as before ... but return value are ignored, so we just do nothing */
            break;
        case ARSTREAM_READER_CAUSE_CANCEL:
            /* Called when the library closes, return values ignored, so do nothing here */
            break;
        default:
            break;
    }

    return ret;
}

int sendBeginStream(JS_MANAGER_t *jsManager)
{
    int sentStatus = 1;
    u_int8_t cmdBuffer[128];
    int32_t cmdSize = 0;
    eARCOMMANDS_GENERATOR_ERROR cmdError;
    eARNETWORK_ERROR netError = ARNETWORK_ERROR;
    
    ARSAL_PRINT(ARSAL_PRINT_INFO, TAG, "- Send Streaming Begin");
    
    // Send Streaming begin command
    cmdError = ARCOMMANDS_Generator_GenerateJumpingSumoMediaStreamingVideoEnable(cmdBuffer, sizeof(cmdBuffer), &cmdSize, 1);
    if (cmdError == ARCOMMANDS_GENERATOR_OK)
    {
        netError = ARNETWORK_Manager_SendData(jsManager->netManager, JS_NET_CD_ACK_ID, cmdBuffer, cmdSize, NULL, &(arnetworkCmdCallback), 1);
    }
    
    if ((cmdError != ARCOMMANDS_GENERATOR_OK) || (netError != ARNETWORK_OK))
    {
        ARSAL_PRINT(ARSAL_PRINT_WARNING, TAG, "Failed to send Streaming command. cmdError:%d netError:%s", cmdError, ARNETWORK_Error_ToString(netError));
        sentStatus = 0;
    }
    
    return sentStatus;
}


void registerARCommandsCallbacks (IHM_t *ihm)
{
    ARCOMMANDS_Decoder_SetCommonCommonStateBatteryStateChangedCallback(batteryStateChangedCallback, ihm);
}

void unregisterARCommandsCallbacks ()
{
    ARCOMMANDS_Decoder_SetCommonCommonStateBatteryStateChangedCallback (NULL, NULL);
}

void *looperRun (void* data)
{
    JS_MANAGER_t *jsManager = (JS_MANAGER_t *)data;
    
    if(jsManager != NULL)
    {
        while (jsManager->run)
        {
            sendPCMD(jsManager);
            
            usleep(50000);
        }
    }

    return NULL;
}

void *readerRun (void* data)
{
    JS_MANAGER_t *jsManager = NULL;
    int bufferId = 0;
    int failed = 0;
    
    // Allocate some space for incoming data.
    const size_t maxLength = 128 * 1024;
    void *readData = malloc (maxLength);
    if (readData == NULL)
    {
        failed = 1;
    }
    
    if (!failed)
    {
        // get thread data.
        if (data != NULL)
        {
            bufferId = ((READER_THREAD_DATA_t *)data)->readerBufferId;
            jsManager = ((READER_THREAD_DATA_t *)data)->jsManager;
            
            if (jsManager == NULL)
            {
                failed = 1;
            }
        }
        else
        {
            failed = 1;
        }
    }
    
    if (!failed)
    {
        while (jsManager->run)
        {
            eARNETWORK_ERROR netError = ARNETWORK_OK;
            int length = 0;
            int skip = 0;
            
            // read data
            netError = ARNETWORK_Manager_ReadDataWithTimeout (jsManager->netManager, bufferId, readData, maxLength, &length, 1000);
            if (netError != ARNETWORK_OK)
            {
                if (netError != ARNETWORK_ERROR_BUFFER_EMPTY)
                {
                    ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "ARNETWORK_Manager_ReadDataWithTimeout () failed : %s", ARNETWORK_Error_ToString(netError));
                }
                skip = 1;
            }
            
            if (!skip)
            {
                // Forward data to the CommandsManager
                eARCOMMANDS_DECODER_ERROR cmdError = ARCOMMANDS_DECODER_OK;
                cmdError = ARCOMMANDS_Decoder_DecodeBuffer ((uint8_t *)readData, length);
                if ((cmdError != ARCOMMANDS_DECODER_OK) && (cmdError != ARCOMMANDS_DECODER_ERROR_NO_CALLBACK))
                {
                    char msg[128];
                    ARCOMMANDS_Decoder_DescribeBuffer ((uint8_t *)readData, length, msg, sizeof(msg));
                    ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "ARCOMMANDS_Decoder_DecodeBuffer () failed : %d %s", cmdError, msg);
                }
            }
        }
    }
    
    if (readData != NULL)
    {
        free (readData);
        readData = NULL;
    }
    
    return NULL;
}

int sendPCMD(JS_MANAGER_t *jsManager)
{
    int sentStatus = 1;
    u_int8_t cmdBuffer[128];
    int32_t cmdSize = 0;
    eARCOMMANDS_GENERATOR_ERROR cmdError;
    eARNETWORK_ERROR netError = ARNETWORK_ERROR;
    
    // Send Posture command
    cmdError = ARCOMMANDS_Generator_GenerateJumpingSumoPilotingPCMD(cmdBuffer, sizeof(cmdBuffer), &cmdSize, jsManager->dataPCMD.flag, jsManager->dataPCMD.speed, jsManager->dataPCMD.turn);
    if (cmdError == ARCOMMANDS_GENERATOR_OK)
    {
        // The commands sent in loop should be sent to a buffer not acknowledged ; here JS_NET_CD_NONACK_ID
        netError = ARNETWORK_Manager_SendData(jsManager->netManager, JS_NET_CD_NONACK_ID, cmdBuffer, cmdSize, NULL, &(arnetworkCmdCallback), 1);
    }
    
    if ((cmdError != ARCOMMANDS_GENERATOR_OK) || (netError != ARNETWORK_OK))
    {
        sentStatus = 0;
    }
    
    return sentStatus;
}

int sendJump(JS_MANAGER_t *jsManager)
{
    int sentStatus = 1;
    u_int8_t cmdBuffer[128];
    int32_t cmdSize = 0;
    eARCOMMANDS_GENERATOR_ERROR cmdError;
    eARNETWORK_ERROR netError = ARNETWORK_ERROR;
    
    // Send Posture command
    cmdError = ARCOMMANDS_Generator_GenerateJumpingSumoAnimationsJump(cmdBuffer, sizeof(cmdBuffer), &cmdSize, ARCOMMANDS_JUMPINGSUMO_ANIMATIONS_JUMP_TYPE_HIGH);
    if (cmdError == ARCOMMANDS_GENERATOR_OK)
    {
        // The commands sent by event should be sent to an buffer acknowledged  ; here JS_NET_CD_ACK_ID
        netError = ARNETWORK_Manager_SendData(jsManager->netManager, JS_NET_CD_ACK_ID, cmdBuffer, cmdSize, NULL, &(arnetworkCmdCallback), 1);
    }
    
    if ((cmdError != ARCOMMANDS_GENERATOR_OK) || (netError != ARNETWORK_OK))
    {
        sentStatus = 0;
    }
    
    return sentStatus;
}

eARNETWORK_MANAGER_CALLBACK_RETURN arnetworkCmdCallback(int buffer_id, uint8_t *data, void *custom, eARNETWORK_MANAGER_CALLBACK_STATUS cause)
{
    eARNETWORK_MANAGER_CALLBACK_RETURN retval = ARNETWORK_MANAGER_CALLBACK_RETURN_DEFAULT;

    ARSAL_PRINT(ARSAL_PRINT_DEBUG, TAG, "    - arnetworkCmdCallback %d, cause:%d ", buffer_id, cause);

    if (cause == ARNETWORK_MANAGER_CALLBACK_STATUS_TIMEOUT)
    {
        retval = ARNETWORK_MANAGER_CALLBACK_RETURN_DATA_POP;
    }

    return retval;
}

eARDISCOVERY_ERROR ARDISCOVERY_Connection_SendJsonCallback (uint8_t *dataTx, uint32_t *dataTxSize, void *customData)
{
    JS_MANAGER_t *jsManager = (JS_MANAGER_t *)customData;
    eARDISCOVERY_ERROR err = ARDISCOVERY_OK;

    if ((dataTx != NULL) && (dataTxSize != NULL) && (jsManager != NULL))
    {
        *dataTxSize = sprintf((char *)dataTx, "{ \"%s\": %d,\n \"%s\": \"%s\",\n \"%s\": \"%s\" }",
                              ARDISCOVERY_CONNECTION_JSON_D2CPORT_KEY, jsManager->d2cPort,
                              ARDISCOVERY_CONNECTION_JSON_CONTROLLER_NAME_KEY, "toto",
                              ARDISCOVERY_CONNECTION_JSON_CONTROLLER_TYPE_KEY, "tata") + 1;
    }
    else
    {
        err = ARDISCOVERY_ERROR;
    }

    return err;
}

eARDISCOVERY_ERROR ARDISCOVERY_Connection_ReceiveJsonCallback (uint8_t *dataRx, uint32_t dataRxSize, char *ip, void *customData)
{
    JS_MANAGER_t *jsManager = (JS_MANAGER_t *)customData;
    eARDISCOVERY_ERROR err = ARDISCOVERY_OK;

    if ((dataRx != NULL) && (dataRxSize != 0) && (jsManager != NULL))
    {
        char *json = malloc(dataRxSize + 1);
        strncpy(json, (char *)dataRx, dataRxSize);
        json[dataRxSize] = '\0';

        //read c2dPort ...

        ARSAL_PRINT(ARSAL_PRINT_DEBUG, TAG, "    - ReceiveJson:%s ", json);

        free(json);
    }
    else
    {
        err = ARDISCOVERY_ERROR;
    }

    return err;
}

void batteryStateChangedCallback (uint8_t percent, void *custom)
{
    // callback of changing of battery level
    
    IHM_t *ihm = (IHM_t *) custom;
    
    if (ihm != NULL)
    {
        IHM_PrintBattery (ihm, percent);
    }
}


// IHM callbacks: 

void onInputEvent (eIHM_INPUT_EVENT event, void *customData)
{
    // Manage IHM input events
    JS_MANAGER_t *jsManager = (JS_MANAGER_t *)customData;
    
    switch (event)
    {
        case IHM_INPUT_EVENT_EXIT:
            gIHMRun = 0;
            break;
        case IHM_INPUT_EVENT_JUMP:
            if(jsManager != NULL)
            {
                sendJump (jsManager);
            }
            break;
        case IHM_INPUT_EVENT_FORWARD:
            if(jsManager != NULL)
            {
                jsManager->dataPCMD.flag = 1;
                jsManager->dataPCMD.speed = 35;//50;
            }
            break;
        case IHM_INPUT_EVENT_BACK:
            if(jsManager != NULL)
            {
                jsManager->dataPCMD.flag = 1;
                jsManager->dataPCMD.speed = -35;//-50;
            }
            break;
        case IHM_INPUT_EVENT_RIGHT:
            if(jsManager != NULL)
            {
                jsManager->dataPCMD.flag = 1;
                jsManager->dataPCMD.turn = 25;//50;
            }
            break;
        case IHM_INPUT_EVENT_LEFT:
            if(jsManager != NULL)
            {
                jsManager->dataPCMD.flag = 1;
                jsManager->dataPCMD.turn = -25;//-50;
            }
            break;
        case IHM_INPUT_EVENT_NONE:
            if(jsManager != NULL)
            {
                jsManager->dataPCMD.flag = 0;
                jsManager->dataPCMD.speed = 0;
                jsManager->dataPCMD.turn = 0;
            }
            break;
        default:
            break;
    }
}

int customPrintCallback (eARSAL_PRINT_LEVEL level, const char *tag, const char *format, va_list va)
{
    // Custom callback used when ncurses is runing for not disturb the IHM
    
    if ((level == ARSAL_PRINT_ERROR) && (strcmp(TAG, tag) == 0))
    {
        // save the last Error
        vsnprintf(gErrorStr, (ERROR_STR_LENGTH - 1), format, va);
        gErrorStr[ERROR_STR_LENGTH - 1] = '\0';
    }
    
    return 1;
}

