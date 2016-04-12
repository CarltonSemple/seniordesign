/*
    Copyright (C) 2014 Parrot SA

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the 
      distribution.
    * Neither the name of Parrot nor the names
      of its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
    OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
    AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
    OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
*/
/**
 * @file ihm.c
 * @brief This file contains sources about ncurses IHM used by arsdk example "JumpingSumoPiloting"
 * @date 15/01/2015
 */

/*****************************************
 *
 *             include file :
 *
 *****************************************/

#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <libARSAL/ARSAL.h>

#include "ihm.h"

/*****************************************
 *
 *             define :
 *
 *****************************************/

#define HEADER_X 0
#define HEADER_Y 0

#define INFO_X 0
#define INFO_Y 2

#define BATTERY_X 0
#define BATTERY_Y 4

#define BUF_SIZE 1024

/*****************************************
 *
 *             private header:
 *
 ****************************************/
void *IHM_InputProcessing(void *data);

/*****************************************
 *
 *             implementation :
 *
 *****************************************/
 
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

IHM_t *IHM_New (IHM_onInputEvent_t onInputEventCallback)
{
    int failed = 0;
    IHM_t *newIHM = NULL;
    
    // check parameters
    if (onInputEventCallback == NULL)
    {
        failed = 1;
    }
    
    if (!failed)
    {
        //  Initialize IHM
        newIHM = malloc(sizeof(IHM_t));
        if (newIHM != NULL)
        {
            //  Initialize ncurses
            newIHM->mainWindow = initscr();
            newIHM->inputThread = NULL;
            newIHM->run = 1;
            newIHM->onInputEventCallback = onInputEventCallback;
            newIHM->customData = NULL;
        }
        else
        {
            failed = 1;
        }
    }
    
    if (!failed)
    {
        raw();                  // Line buffering disabled
        keypad(stdscr, TRUE);
        noecho();               // Don't echo() while we do getch
        timeout(100);
        
        refresh();
    }
    
    if (!failed)
    {
        //start input thread
        if(ARSAL_Thread_Create(&(newIHM->inputThread), IHM_InputProcessing, newIHM) != 0)
        {
            failed = 1;
        }
    }
    
    if (failed)
    {
        IHM_Delete (&newIHM);
    }

    return  newIHM;
}

void IHM_Delete (IHM_t **ihm)
{
    //  Clean up

    if (ihm != NULL)
    {
        if ((*ihm) != NULL)
        {
            (*ihm)->run = 0;
            
            if ((*ihm)->inputThread != NULL)
            {
                ARSAL_Thread_Join((*ihm)->inputThread, NULL);
                ARSAL_Thread_Destroy(&((*ihm)->inputThread));
                (*ihm)->inputThread = NULL;
            }
            
            delwin((*ihm)->mainWindow);
            (*ihm)->mainWindow = NULL;
            endwin();
            refresh();
            
            free (*ihm);
            (*ihm) = NULL;
        }
    }
}

void IHM_setCustomData(IHM_t *ihm, void *customData)
{
    if (ihm != NULL)
    {
        ihm->customData = customData;
    }
}

void *IHM_InputProcessing(void *data)
{
    IHM_t *ihm = (IHM_t *) data;
    int key = 0, i, desired_length;
    int sockfd, mlen;
    char buffer[BUF_SIZE];
    struct sockaddr_in serv_addr;
	int addrsize = sizeof(struct sockaddr_in);
    
    if (ihm != NULL)
    {
		// Set up client socket
		sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sockfd < 0)
		{
			error("ERROR opening socket");
		}
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(32323);
		unsigned int in_address = 192 << 24 | 168 << 16 | 0 << 8 | 106;//127 << 24 | 0 << 16 | 0 << 8 | 1;
		serv_addr.sin_addr.s_addr = htonl (in_address);
			
		// Connect to the server socket 
		if (connect (sockfd, (struct sockaddr *) &serv_addr, sizeof (struct sockaddr_in)) == -1)
		{	
			perror("Could not connect");
			exit(-1);
		} 
		
		// Send synchronization message to server socket
		 bzero(buffer,BUF_SIZE);
		 buffer[0] = 't';
		 printf("Sending synchronization message...\n");
		 i = write(sockfd, buffer, strlen(buffer));
		 if (i<0)
		 {
			 perror("Could not send synchronization message");
		 }
		 printf("...Sent\n");
		 			 
		 bzero(buffer,BUF_SIZE);
		 printf("Waiting...\n");
		 mlen = read(sockfd, buffer, 1);
		 if (mlen < 0)
		 {
			 perror("Could not receive synchronization reply");
		 }
		 printf("Received \"%s\" from server %s\n", buffer, inet_ntoa (serv_addr.sin_addr));
			 
        while (ihm->run)
        {
            //key = getch();
			 
			 // Obtain first instruction from server
			 bzero(buffer,BUF_SIZE);
			 printf("Waiting...\n");
			 mlen = read(sockfd, buffer, 1);
			 if (mlen < 0)
			 {
				 perror("Could not receive commands");
			 }
			 printf("Received \"%s\" from server %s\n", buffer, inet_ntoa (serv_addr.sin_addr));
			 key = buffer[0];
			 
			 
			 if (key != 'q')
			 {
				 // Obtain second instruction from server
				 bzero(buffer,BUF_SIZE);
				 printf("Waiting...\n");
				 mlen = read(sockfd, buffer, 1);
				 if (mlen < 0)
				 {
					 perror("Could not receive commands");
				 }
				 printf("Received \"%s\" from server %s\n", buffer, inet_ntoa (serv_addr.sin_addr));
				 key = buffer[0];
				 
				 // Obtain third instruction from server
				 bzero(buffer,BUF_SIZE);
				 printf("Waiting...\n");
				 mlen = read(sockfd, buffer, 1);
				 if (mlen < 0)
				 {
					 perror("Could not receive commands");
				 }
				 printf("Received \"%s\" from server %s\n", buffer, inet_ntoa (serv_addr.sin_addr));
				 desired_length = buffer[0] - '0';
				 desired_length = 4;

			 }
            for (int j=0; j<desired_length; j++) {
				if (j+1 == desired_length)
					key = 'x';
            if ((key == 27) || (key == 'q'))
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_EXIT, ihm->customData);
                }
            }
            else if(key == KEY_UP || key == 'w')
            {
                if(ihm->onInputEventCallback != NULL)
                {
					for (i=0; i<3000; i++) {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_FORWARD, ihm->customData);
                    usleep(10);
					}
                }
            }
            else if(key == KEY_DOWN || key == 's')
            {
                if(ihm->onInputEventCallback != NULL)
                {
					for (i=0; i<3000; i++) {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_BACK, ihm->customData);
                    usleep(10);
					}
                }
            }
            else if(key == KEY_LEFT || key == 'a')
            {
                if(ihm->onInputEventCallback != NULL)
                {
					// Change angle 40 degrees left
					for (i=0; i<3000; i++) {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_LEFT, ihm->customData);
                    usleep(10);
					}
                }
            }
            else if(key == KEY_RIGHT || key == 'd')
            {
                if(ihm->onInputEventCallback != NULL)
                {
					// Change angle 40 degrees right
					for (i=0; i<3000; i++) {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_RIGHT, ihm->customData);
                    usleep(10);
					}
                }
            }
            else
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_NONE, ihm->customData);
                }
            }
            
            usleep(10);
		}
        }
		close(sockfd);
    }
    
    return NULL;
}

void IHM_PrintHeader(IHM_t *ihm, char *headerStr)
{
    if (ihm != NULL)
    {
        move(HEADER_Y, 0);   // move to begining of line
        clrtoeol();          // clear line
        mvprintw(HEADER_Y, HEADER_X, headerStr);
    }
}

void IHM_PrintInfo(IHM_t *ihm, char *infoStr)
{
    if (ihm != NULL)
    {
        move(INFO_Y, 0);    // move to begining of line
        clrtoeol();         // clear line
        mvprintw(INFO_Y, INFO_X, infoStr);
    }
}

void IHM_PrintBattery(IHM_t *ihm, uint8_t percent)
{
    if (ihm != NULL)
    {
        move(BATTERY_Y, 0);     // move to begining of line
        clrtoeol();             // clear line
        mvprintw(BATTERY_Y, BATTERY_X, "Battery: %d", percent);
    }
}



