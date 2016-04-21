#include "sass.h"
#include "scanner.h"
#include "eyeinsky.h"
using namespace std;
using namespace cv;

/*Can be changed, just value we used*/
#define BUF_SIZE 70000
/*Global Variable*/

/*Creating a Mat Object*/
Mat frame_Mat;

/*Object Detector */
ObjectDetector OD1;

/* UMat */
UMat Frame_UMat;

CommunicationBox commBox;
Drone dactive(1, commBox);//, db(2, commBox);

//Desired drone to pilot
int desired_drone = 1;

void analyzeVideoRunDrones();
int determineNeedToSendCommands();
void receiveFrames();
void sendCommands();
void runEyeinSky();
void runScanner();

Sass::Sass()
{
    Matcher::templateMatchingMethod = 1;
}

Sass::Sass(int matchingMethod)
{
    Matcher::templateMatchingMethod = matchingMethod;
}

void Sass::runSystem()
{   
    // Start server for sending commands
    thread sendComThread(sendCommands);
    
    // Start server which receives frames from drones
	thread receiveFrameThread(receiveFrames);
    
    // Start thread for processing drone & kinect video feeds
    thread videoThread(analyzeVideoRunDrones);

    // Start thread for kinect in sky
    //thread skyThread(runEyeinSky);

    // Start thread for scanner kinect
   // thread scanThread(runScanner);
   
   receiveFrameThread.join();
   sendComThread.join();
   videoThread.join();

}
void runEyeinSky()
{
    eyeinsky eye(Matcher::templateMatchingMethod,commBox);// or 4
}
void runScanner()
{
    Scanner s;
    s.runIndependently();
}
void analyzeVideoRunDrones()
{
	UMat current;
	commBox.okayToDecide = true;
	
    while(true)
    {
		
		// get latest drone images
		if(!commBox.droneFrame.data) {
			continue;
		}		
		
		
		
		if(commBox.okayToDecide == true) {
			commBox.droneFrame.copyTo(current);
        	dactive.decide(current);
		}		
		
		imshow("Active Drone Feed", current);
		
		waitKey(1);

		
		
    }   
}

void convert_file_to_mat()
{
	imread("tmp.jpg",CV_LOAD_IMAGE_COLOR).copyTo(frame_Mat);
	frame_Mat.copyTo(Frame_UMat);
	
	commBox.droneFrame = frame_Mat;
}

// Send commands when there's commands to send
void sendCommands()
{

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
	
	int current_drone = desired_drone;
    
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
		perror("ERROR opening socket");
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
		perror("ERROR on binding");
    }
	 
    /*Listen: make this socket ready to accept connection requests*/
    if(listen(parentfd,2) < 0)
    {
		perror("ERROR on listen");
    }
    /*Getting the length of the client message*/
    clientlen = sizeof(client_addr_drone_1);
	
    /*************Setting up the Connections for Both Drones***********************/
    /*Accepting Connection from Drone 1*/
    drone_1 = accept(parentfd, (struct sockaddr*) &client_addr_drone_2, &clientlen);
    if(drone_1 < 0)
    {
		perror("ERROR on accept Drone 2");
    }
    printf("Drone 1 accepted for commands socket\n");
    /*Accepting Connection from Drone 2*/
    drone_2 = accept(parentfd, (struct sockaddr*) &client_addr_drone_1, &clientlen);
    if(drone_2 < 0)
    {
		perror("ERROR on accept Drone 1");
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
		perror("ERROR reading from Drone 1");
    }
    /**************************/
	printf("In main function received %s\n",buffer);
    /*Reading info from Drone 2*/
    bzero(buffer,BUF_SIZE);
    n_2 = read(drone_2, buffer, 1);
    if(n_2 < 0)
    {
		perror("ERROR reading from Drone 2");
    }
    /**************************/
    /*Just putting a NULL value in buffer so there is something in there*/
    buffer[0] = ' ';
    /* Sending sync info to Drone 1*/
    n_1=write(drone_1, buffer, strlen(buffer));
    if(n_1 < 0)
    {
		perror("ERROR writing to Drone 1");	
    }
    /**************************/
    
    /* Sending sync info to Drone 1*/
    n_2=write(drone_2, buffer, strlen(buffer));
    if(n_2 < 0)
    {
		perror("ERROR writing to Drone 2");
    }
    /**************************/
    /**********************************************************************/
    
    /*Main Loop: wait for a connection request*/
    // Send to server
    while (true) 
    {	
		if(commBox.okayToDecide == true) {
			continue;
		}
		
		if (current_drone != desired_drone)
			current_drone = desired_drone;
		
		/*Setting the values in buffer to zero*/
		bzero(buffer, BUF_SIZE);
		
		/*If no values are choose as shown above you will default and control drone 1*/
		if (current_drone != 1 && current_drone != 2)
		{
			current_drone = 1;
		}
		
		cout << "current_drone iss: " << current_drone << endl;

		/*Get desired drone movement/instruction*/
		/*This is scanning in how far you want to go from which instruction you chose from WASD*/
		buffer[0] = commBox.droneMovement;
		/*Move drone by sending buffer desired_length times*/
		for (int i=0; i<commBox.droneMovementValue; i++)
		{
			printf("Sending %s to drone %i", buffer, current_drone);
			cout << "current_drone = " << current_drone << endl << endl;
			if (current_drone == 1)
			{
				n_1=write(drone_1, buffer, strlen(buffer));
				if(n_1 < 0)
				{
					perror("ERROR writing to Drone 1");	
				}
				printf("...Sent\n");
			}
			else
			{
				n_2=write(drone_2, buffer, strlen(buffer));
				if(n_2 < 0)
				{
					perror("ERROR writing to Drone 2");	
				}
				printf("...Sent\n");
			}
		}
		/*Stop moving drone*/
		/*We are sending x as the ending value to determine when to stop moving drone*/
		/*Example: Choose w 3, which really is sending w,w,w,x. So we are sending 3 forward movements followed by the ending
		value x which will stop sending "w" and the drones will wait for next instruction*/
		buffer[0] = 'x';
		printf("Sending %s to drone %i", buffer, current_drone);
		if (current_drone == 1)
		{
			n_1=write(drone_1, buffer, strlen(buffer));
			if(n_1 < 0)
			{
				perror("ERROR writing to Drone 1");	
			}
			printf("...Sent\n");
		}
		else
		{
			n_2=write(drone_2, buffer, strlen(buffer));
			if(n_2 < 0)
			{
				perror("ERROR writing to Drone 2");	
			}
			printf("...Sent\n");
		}
		
		sleep(1);
		commBox.okayToDecide = true;
	}
     
    /*Closing the connection from the server as well as the two clients when you quit*/	
    close(parentfd);
    close(drone_1);
    close(drone_2);
}

/*This the second server that is being multi-thread to capture the frames from both drones.*/
void receiveFrames()
{
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
    /*Quit boolean*/
    int quit_flag = 0;
	/*Used for eliminating Binding ERROR*/
	int optval = 1;
	
	int current_drone = desired_drone;
    
    /*Assign Port Number*/
    /*This port number is different to distinguish between which server is for command/control of drones and receving the video*/
    portno = 12345;

    /*Create server socket*/
    parentfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    /*Checking Socket Creation*/
    if (parentfd < 0)
    {	 
		perror("ERROR opening socket");
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
		perror("ERROR on binding");
    }
	printf("Binding was a success\n");
    /*Listen: make this socket ready to accept connection requests*/
    if(listen(parentfd,2) < 0)
    {
		perror("ERROR on listen");
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
    int counter = 0;
    int totalRead = 0;
	int read_Size = 1448;
	
	int img;
	img = open("tmp.jpg",O_RDWR);
    
    /*This is receiving the frames that we are sending over first and then receiving the data.*/
    while (1) 
    {
		frameSizeSyncFlag = 0;
		
		if (current_drone != desired_drone)
			current_drone = desired_drone;
		
		/*Checking to make sure we get the exact frame size from the client*/
		while (!frameSizeSyncFlag)
		{
			bzero(buffer,BUF_SIZE);
			if (current_drone == 1)
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
			if (current_drone == 1)
			{
				temp_n = write(drone_1_video, buffer, 6);
			}
			else
			{
				temp_n = write(drone_2_video, buffer, 6);
			}
			
			// Read back if valid or not (1 means valid, 0 means invalid)
			bzero(buffer,BUF_SIZE);
			if (current_drone == 1)
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
			
			if (current_drone == 1)
			{
				//printf("Read 1......\n");
				n_1 = read(drone_1_video, &buffer2[totalRead], read_Size);
				buffer2[strlen(buffer2)] = '\0';
			}
			else
			{
				//printf("Read 2......\n");
				n_1 = read(drone_2_video, &buffer2[totalRead], read_Size);
				buffer2[strlen(buffer2)] = '\0';
			}
			if(n_1 < 0)
			{
				perror("Couldn't finish off frame read:");
			}
			
			
			totalRead += n_1;
			//cout << "n_1:" << n_1 << "   totalRead:" << totalRead << "   FrameSize:" << frameSize << endl;
		}
		
		if (current_drone == 1)
		{
			temp_n = write(drone_1_video, "1", 1);
		}
		else
		{
			temp_n = write(drone_2_video, "1", 1);
		}
		
		lseek(img,0,SEEK_SET);
		n_1=write(img,buffer2,frameSize);
		if(n_1 < 0)
		{
			perror("Couldnt write to jpeg:");
		}	
		
		thread Convert_to_mat(convert_file_to_mat);
		Convert_to_mat.join();
		
	}

}

int main(int argc, char *argv[]) 
{
	cout << "In main" << endl;
    Sass sassy;
    sassy.runSystem();

    return 0;
}
