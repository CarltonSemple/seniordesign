/*******************************************************
* Copyright (C) 2016-2017 Justin Villani, Carlton Semple, Jake Soccorso, Iba ba <juvillan@syr.edu, jpsoccor@syr.edu, cosemple@syr.edu, ieba@syr.edu>
* 
* This file is part of SASS(Surveillance & Apprenhensive Security System).
* 
* SASS can not be copied and/or distributed without the express
* permission of Justin Villani, Carlton Semple, Jake Soccorse, Iba ba
*******************************************************/
 
// How to compile
// g++ -o tcp_server tcp_server.cpp -std=c++11 -pthread


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>
#include <thread>
#include <iostream>
#include "videoCodec.h"
#include <vector>

#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/ocl.hpp> // opencl

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

/*This is include for the conversion from c to c++*/
using namespace std;
using namespace cv;

/*Can be changed, just value we used*/
#define BUF_SIZE 70000

/*Creating a Mat Object*/
Mat frame_Mat;

/*Function that captures errors from error checking in code*/
void error(const char *msg)
{
    perror(msg);
    exit(0);
}


// Thread function to perform OpenCV functions, such as imread and imshow
// Being threaded from the receiveFrames() threaded function
//TODO: Try HOG and other algorithms in this thread...
void convert_file_to_mat()
{
	// Read in jpeg file to Mat
	imread("tmp.jpg",CV_LOAD_IMAGE_COLOR).copyTo(frame_Mat);
	
	// Display Mat properties
	cout << "******************************************************" << endl;
	cout << "frame_Mat Height:" << frame_Mat.size().height << endl;
	cout << "frame_Mat Width:" << frame_Mat.size().width << endl;
	cout << "frame_Mat Type:" << frame_Mat.type() << endl;
	
	// Show Mat in window
	imshow("Mat Image",frame_Mat);
	waitKey(1);
}


/*This the second server that is being multi-thread to capture the frames from both drones.*/
void receiveFrames()
{
	/*This is opening a file descriptor to the video_fifo which is storing the data to open the video frames*/
    FILE *video_in = fopen("./ActiveDroneFeed","w+"); // This is for the active drone's frames
    /*Server File and port number being used*/
    int parentfd, portno;
    /*Two Child processes for both unqiue Drones*/
    int drone_1_video, drone_2_video;
    /*Byte Size of Client Addr*/
    socklen_t clientlen;
    /*Server and Client Address Setup*/
    struct sockaddr_in serv_addr;
    struct sockaddr_in client_addr_drone_1_video;
    struct sockaddr_in client_addr_drone_2_video;
    /*Client Host Info*/
    struct hostent *hostp;
    /*This is the buffer holding our data being sent*/
    char buffer[BUF_SIZE];
    /*Dotted decimal Host address String*/
    char *hostaddrp;
    /*Message Byte Size*/
    int n_1, n_2;
    /*Desired distance to move or turn*/
    int desired_length;
    /*Desired drone to pilot*/
    int desired_drone;
    /*Quit boolean*/
    int quit_flag = 0;
	/*Used for eliminating Binding ERROR*/
	int optval = 1;
    
    /*Assign Port Number*/
    /*This port number is different to distinguish between which server is for command/control of drones and receving the video*/
    portno = 12345;

    /*Create server socket*/
    parentfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    /*Checking Socket Creation*/
    if (parentfd < 0)
    {	 
		error("ERROR opening socket");
    }
	/*Fixing the binding ERROR which occurs */	
	setsockopt(parentfd,SOL_SOCKET,(SO_REUSEADDR | SO_REUSEPORT),(const void *)&optval,sizeof(int));
    /* Building the Server's Internet Address*/ 
    bzero((char *) &serv_addr, sizeof(serv_addr));
    /*Internet Address*/
    serv_addr.sin_family = AF_INET;
    /*Accept any address from the system*/
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    /*Port being listened on*/
    serv_addr.sin_port = htons((unsigned short)portno);
    /*Bind: associate the parent socket with port*/
    if (bind(parentfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
    {
		error("ERROR on binding");
    }
	printf("Binding was a success\n");
    /*Listen: make this socket ready to accept connection requests*/
    if(listen(parentfd,2) < 0)
    {
		error("ERROR on listen");
    }
    printf("Listening was a success\n");
    /*Getting the length of the client message*/
    clientlen = sizeof(client_addr_drone_1_video);
    
    /*************Setting up the Connections for Both Drones***********************/
    /******************************************************************************/
    /*Accepting Connection from Drone 1 for video*/
    drone_1_video = accept(parentfd, (struct sockaddr*) &client_addr_drone_1_video, &clientlen);
    if(drone_1_video < 0)
    {
		perror("ERROR on accept Drone 1");
    }
    printf("Drone 1 accepted for video socket\n");
    /*Accepting Connection from Drone 2 for video*/
    drone_2_video = accept(parentfd, (struct sockaddr*) &client_addr_drone_2_video, &clientlen);
    if(drone_2_video < 0)
    {
		perror("ERROR on accept Drone 2");
    }
    /***************************************************************************/
    printf("Drone 2 accepted for video socket\n");
    printf("\nBoth Drones connected to System\n");
    /****************Syncing the Drones sockets with server socket**************/
    /*Reading info from Drone 1*/		
    bzero(buffer,BUF_SIZE);
    n_1 = read(drone_1_video, buffer, 1);
    if(n_1<0)
    {
		perror("ERROR reading from Drone 1");
    }

	printf("In thread received %s\n",buffer);
    /*Reading info from Drone 2*/
    bzero(buffer,BUF_SIZE);
    n_2 = read(drone_2_video, buffer, 1);
    if(n_2 < 0)
    {
		perror("ERROR reading from Drone 2");
    }

    /*Just putting a NULL value in buffer so there is something in there*/
    buffer[0] = ' ';
    /* Sending sync info to Drone 1*/
    //*******************************
    n_1=write(drone_1_video, buffer, strlen(buffer));
    if(n_1 < 0)
    {
		perror("ERROR writing to Drone 1");	
    }
    /* Sending sync info to Drone 2*/
    //*******************************
    n_2=write(drone_2_video, buffer, strlen(buffer));
    if(n_2 < 0)
    {
		perror("ERROR writing to Drone 2");	
    }

    int frameSizeSyncFlag = 0;
    int frameSize;
    int temp_n;
    int activeDrone = 1; // Keeps track of which drone is active (1 or 2)
    int counter = 0;
    int totalRead = 0;
	int read_Size = 1448;
	
	int img;
	img = open("tmp.jpg",O_RDWR);
    
    /*This is receiving the frames that we are sending over first and then receiving the data.*/
    while (1) 
    {
		frameSizeSyncFlag = 0;
		counter++;
		if(counter == 100)
		{
			if(activeDrone == 1)
			{
				activeDrone=2;
			}
			else
			{
				activeDrone=1;
			}
			counter = 0;
		}
		/*Checking to make sure we get the exact frame size from the client*/
		while (!frameSizeSyncFlag)
		{
			bzero(buffer,BUF_SIZE);
			if (activeDrone == 1)
			{
				n_1 = read(drone_1_video, buffer, 6);
			}
			else
			{
				n_1 = read(drone_2_video, buffer, 6);
			}
						
			frameSize = atoi(buffer);
			//printf("frameSize = %i\n", frameSize);
			
			// Send frame size back to validate
			temp_n = 0;
			if (activeDrone == 1)
			{
				temp_n = write(drone_1_video, buffer, 6);
			}
			else
			{
				temp_n = write(drone_2_video, buffer, 6);
			}
			
			// Read back if valid or not (1 means valid, 0 means invalid)
			bzero(buffer,BUF_SIZE);
			if (activeDrone == 1)
			{
				n_1 = read(drone_1_video, buffer, 1);
			}
			else
			{
				n_1 = read(drone_2_video, buffer, 1);
			}

			// Correctly got frame size
			if (buffer[0] == '1')
			{
				// jump out of loop
				frameSizeSyncFlag = 1;
			}
			else
			{
				cout << "WRONG FRAME SIZE RECEIVED***********\n";
				frameSizeSyncFlag = 0;
			}
		}
		/*********************************************/
		/*At this point we should have the frame size*/
		/*********************************************/
		
		char* buffer2 = new char(frameSize);
		totalRead = 0;
		/*We are making sure that at the frameSize and data have been received and once it has been, we then write the the video_fifo to be able to display the video*/
		while (totalRead < frameSize)
		{
			read_Size = 1448;
			/*Check to see if your going to read onto the next frame size*/
			if(totalRead-frameSize > 1448)
			{
				read_Size = totalRead-frameSize;
			}
			
			if (activeDrone == 1)
			{
				printf("Read 1......\n");
				n_1 = read(drone_1_video, &buffer2[totalRead], read_Size);
				buffer2[strlen(buffer2)] = '\0';
			}
			else
			{
				printf("Read 2......\n");
				n_1 = read(drone_2_video, &buffer2[totalRead], read_Size);
				buffer2[strlen(buffer2)] = '\0';
			}
			if(n_1 < 0)
			{
				perror("Couldn't finish off frame read:");
			}
			
			
			totalRead += n_1;
			cout << "n_1:" << n_1 << "   totalRead:" << totalRead << "   FrameSize:" << frameSize << endl;
		}
		
		if (activeDrone == 1)
		{
			temp_n = write(drone_1_video, "1", 1);
		}
		else
		{
			temp_n = write(drone_2_video, "1", 1);
		}
		///**************************/
		fwrite(buffer2,frameSize,1,video_in);
		fflush(video_in);
		
		// Reset file desciptor to beginning of file
		lseek(img,0,SEEK_SET);
		
		// Write raw frame data to a jpeg file
		n_1=write(img,buffer2,frameSize);
		if(n_1 < 0)
		{
			perror("Couldnt write to jpeg:");
		}	
		
		// Convert the jpeg file to a Mat
		thread Convert_to_mat(convert_file_to_mat);
		Convert_to_mat.join();
	}
	
	//close(img);
}

// ******************************* MAIN *********************************
// **********************************************************************
int main(int argc, char *argv[]) 
{
	// Start server which receives frames from drones
	thread receiveFrameThread(receiveFrames);
	/*Name window for MAt Image*/
	//namedWindow("Mat Image",WINDOW_AUTOSIZE);
	//resizeWindow("Mat Image",800,800);
	//This is opening up the video to display frames
	int child;
	if ((child = fork()) == 0)
	{
		execlp("ffplay", "ffplay", "-i", "ActiveDroneFeed", "-f", "mjpeg", NULL);
	}
	//Server File and port number being used
    int parentfd, portno;
    //Two Child processes for both unqiue Drones
    int drone_1, drone_2;
    //Byte Size of Client Addr
    socklen_t clientlen;
    //Server and Client Address Setup
    struct sockaddr_in serv_addr;
    struct sockaddr_in client_addr_drone_1;
    struct sockaddr_in client_addr_drone_2;
    //Client Host Info
    struct hostent *hostp;
    //This is the buffer holding our data being sent
    char buffer[BUF_SIZE];
    //Dotted decimal Host address String
    char *hostaddrp;
    //Message Byte Size
    int n_1, n_2;
    //Desired distance to move or turn
    int desired_length;
    //Desired drone to pilot
    int desired_drone;
    //Quit boolean
    int quit_flag = 0;
	/*Used for eliminating Binding ERROR*/
	int optval = 1;
    
    //Assign Port Number
    portno = 32323;

    /*Create server socket*/
    parentfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    /*Checking Socket Creation*/
    if (parentfd < 0)
    {	 
		error("ERROR opening socket");
    }	
	/*Fixing the binding ERROR which occurs */	
	setsockopt(parentfd,SOL_SOCKET,(SO_REUSEADDR | SO_REUSEPORT),(const void *)&optval,sizeof(int));
    /* Building the Server's Internet Address*/ 
    bzero((char *) &serv_addr, sizeof(serv_addr));
    /*Internet Address*/
    serv_addr.sin_family = AF_INET;
    /*Accept any address from the system*/
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    /*Port being listened on*/
    serv_addr.sin_port = htons((unsigned short)portno);
    /*Bind: associate the parent socket with port*/
    if (bind(parentfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
    {
		error("ERROR on binding");
    }
	 
    /*Listen: make this socket ready to accept connection requests*/
    if(listen(parentfd,2) < 0)
    {
		error("ERROR on listen");
    }
    /*Getting the length of the client message*/
    clientlen = sizeof(client_addr_drone_1);
	
    /*************Setting up the Connections for Both Drones***********************/
    /*Accepting Connection from Drone 1*/
    drone_2 = accept(parentfd, (struct sockaddr*) &client_addr_drone_2, &clientlen);
    if(drone_2 < 0)
    {
		error("ERROR on accept Drone 2");
    }
    printf("Drone 1 accepted for commands socket\n");
    /*Accepting Connection from Drone 2*/
    drone_1 = accept(parentfd, (struct sockaddr*) &client_addr_drone_1, &clientlen);
    if(drone_1 < 0)
    {
		error("ERROR on accept Drone 1");
    }
    /***************************************************************************/
    printf("Drone 2 accepted for commands socket\n");
    printf("\nBoth Drones connected to System\n");
    /****************Syncing the Drones sockets with server socket**************/
    /*Reading info from Drone 1*/		
    bzero(buffer,BUF_SIZE);
    n_1 = read(drone_1, buffer, 1);
    if(n_1<0)
    {
		error("ERROR reading from Drone 1");
    }
    /**************************/
	printf("In main function received %s\n",buffer);
    /*Reading info from Drone 2*/
    bzero(buffer,BUF_SIZE);
    n_2 = read(drone_2, buffer, 1);
    if(n_2 < 0)
    {
		error("ERROR reading from Drone 2");
    }
    /**************************/
    /*Just putting a NULL value in buffer so there is something in there*/
    buffer[0] = ' ';
    /* Sending sync info to Drone 1*/
    n_1=write(drone_1, buffer, strlen(buffer));
    if(n_1 < 0)
    {
		error("ERROR writing to Drone 1");	
    }
    /**************************/
    
    /* Sending sync info to Drone 1*/
    n_2=write(drone_2, buffer, strlen(buffer));
    if(n_2 < 0)
    {
		error("ERROR writing to Drone 2");
    }
    /**************************/
    /**********************************************************************/
    
    /*Main Loop: wait for a connection request*/
    // Send to server
    while (buffer[0] != 'q') 
    {	
		if (buffer[0] != 'q') 
		{
			/*Setting the values in buffer to zero*/
			bzero(buffer, BUF_SIZE);

			/*Get desired drone to pilot*/
			printf("Enter which drone you wish to pilot (1 or 2, or 3 to Quit): ");
			scanf("%i", &desired_drone);
			/*If 3 is received we are going to quit the connection to both drones*/
			if (desired_drone == 3)
			{
				printf("Setting quit_flag to 1\n");
				quit_flag = 1;
			}
			/*If no values are choose as shown above you will default and control drone 1*/
			else if (desired_drone != 1 && desired_drone != 2)
			{
				desired_drone = 1;
			}
			/*If quit_flag is not triggered, then we proceed in sending instruction to desired drone*/
			if (!quit_flag)
			{
				/*Get desired drone movement/instruction*/
				printf("Enter input for drone movement\n(A: Left, W: Forward, S: Backward, D: Right)\n");
				scanf("%s", buffer);
				if (buffer[0] == 'a' || buffer[0] == 'd')
				{
					printf("Enter number of 40 degrees to turn\n");
				}
				else if (buffer[0] == 'w' || buffer[0] == 's')
				{
					printf("Enter number of inches to move\n");
				}
				/*This is scanning in how far you want to go from which instruction you chose from WASD*/
				scanf("%i", &desired_length);
				/*Move drone by sending buffer desired_length times*/
				for (int i=0; i<desired_length; i++)
				{
					printf("Sending %s to drone %i", buffer, desired_drone);
					if (desired_drone == 1)
					{
						n_1=write(drone_1, buffer, strlen(buffer));
						if(n_1 < 0)
						{
							error("ERROR writing to Drone 1");	
						}
						printf("...Sent\n");
					}
					else
					{
						n_2=write(drone_2, buffer, strlen(buffer));
						if(n_2 < 0)
						{
							error("ERROR writing to Drone 2");	
						}
						printf("...Sent\n");
					}
				}
				/*Stop moving drone*/
				/*We are sending x as the ending value to determine when to stop moving drone*/
				/*Example: Choose w 3, which really is sending w,w,w,x. So we are sending 3 forward movements followed by the ending
				value x which will stop sending "w" and the drones will wait for next instruction*/
				buffer[0] = 'x';
				printf("Sending %s to drone %i", buffer, desired_drone);
				if (desired_drone == 1)
				{
					n_1=write(drone_1, buffer, strlen(buffer));
					if(n_1 < 0)
					{
						error("ERROR writing to Drone 1");	
					}
					printf("...Sent\n");
				}
				else
				{
					n_2=write(drone_2, buffer, strlen(buffer));
					if(n_2 < 0)
					{
						error("ERROR writing to Drone 2");	
					}
					printf("...Sent\n");
				}
			}
			else
			{
				/*Sending quit to drone both drones and cut client connection to server*/
				buffer[0] = 'q';
				printf("Sending %s to drone %i", buffer, desired_drone);
				n_1=write(drone_1, buffer, strlen(buffer));
				if(n_1 < 0)
				{
					error("ERROR writing to Drone 1");	
				}
				printf("...Sent\n");
				n_2=write(drone_2, buffer, strlen(buffer));
				if(n_2 < 0)
				{
					error("ERROR writing to Drone 2");	
				}
				printf("...Sent\n");
			}
		}
	}
     
    /*Closing the connection from the server as well as the two clients when you quit*/	
    close(parentfd);
    close(drone_1);
    close(drone_2);

	// Wait for other server thread to close
    receiveFrameThread.join();

    return 0;
}




