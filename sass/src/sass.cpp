#include "sass.h"

using namespace std;
using namespace cv;

/*Can be changed, just value we used*/
#define BUF_SIZE 70000

CommunicationBox commBox;
Drone da(1, commBox), db(2, commBox);

void analyzeVideoRunDrones();
int determineNeedToSendCommands();
void receiveFrames();
void sendCommands();

Sass::Sass()
{
    commBox.drone1Command = "";
    commBox.drone2Command = "";
    commBox.drone1Wait = false;
    commBox.drone2Wait = false;
    Matcher::templateMatchingMethod = 1;
}

Sass::Sass(int matchingMethod)
{
    commBox.drone1Command = "";
    commBox.drone2Command = "";
    commBox.drone1Wait = false;
    commBox.drone2Wait = false;
    Matcher::templateMatchingMethod = matchingMethod;
}

void Sass::runSystem()
{
    // Start server which receives frames from drones
    // and converts them to a cv::Mat
	thread receiveFrameThread(receiveFrames);
    
    // Start server for sending commands
    thread sendComThread(sendCommands);
    
    // Start thread for processing drone & kinect video feeds
    thread videoThread(analyzeVideoRunDrones);
}

void analyzeVideoRunDrones()
{
    
    
    while(true)
    {
        da.decide();
        db.decide();
    }   
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
    //Desired drone to pilot
    int desired_drone;
    //Quit boolean
    int quit_flag = 0;
    
    //Assign Port Number
    portno = 32323;

    /*Create server socket*/
    parentfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    /*Checking Socket Creation*/
    if (parentfd < 0)
    {	 
		perror("ERROR opening socket");
    }	
	
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
    drone_1 = accept(parentfd, (struct sockaddr*) &client_addr_drone_1, &clientlen);
    if(drone_1 < 0)
    {
		perror("ERROR on accept Drone 1");
    }
    printf("Drone 1 accepted\n");
    /*Accepting Connection from Drone 2*/
    drone_2 = accept(parentfd, (struct sockaddr*) &client_addr_drone_2, &clientlen);
    if(drone_2 < 0)
    {
		perror("ERROR on accept Drone 2");
    }
    /***************************************************************************/
    printf("Drone 2 accepted\n");
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
    while (true) //(buffer[0] != 'q') 
    {	
        
        
        if (buffer[0] != 'q') 
        {
            /*Setting the values in buffer to zero*/
            bzero(buffer, BUF_SIZE);





            // Check for communication box data
            // Continue if there are no commands needed by the drone
            switch(determineNeedToSendCommands())
            {
                // simply iterate through the command strings as they
                // already have the correct format
                case 1:
                    break;  
                case 2:
                    break;
                case 3:
                    break;
            }




            // SELECT DRONE, or QUIT
            //
            //
            //
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
            
            
            
            // SEND INSTRUCTIONS
            //
            //
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
                printf("Sending %s to drone %i", buffer, desired_drone);
                if (desired_drone == 1)
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
                
                
                // END SENDING ACTION
                          
                
                
            }
            else
            {
                // QUIT ACTION
                //
                //
                /*Sending quit to drone both drones and cut client connection to server*/
                buffer[0] = 'q';
                printf("Sending %s to drone %i", buffer, desired_drone);
                n_1=write(drone_1, buffer, strlen(buffer));
                if(n_1 < 0)
                {
                    perror("ERROR writing to Drone 1");	
                }
                printf("...Sent\n");
                n_2=write(drone_2, buffer, strlen(buffer));
                if(n_2 < 0)
                {
                    perror("ERROR writing to Drone 2");	
                }
                printf("...Sent\n");
            }
            
            
        }
    }
}

/*This the second server that is being multi-thread to capture the frames from both drones.*/
void receiveFrames()
{
	/*This is opening a file descriptor to the video_fifo which is storing the data to open the video frames*/
    FILE *video_in = fopen("./video_fifo","w+");
    /*Making sure that the file descriptor was opened correctly*/
    if(video_in == NULL)
    {
		perror("Not opening video fifo");
    }
    else
    {
		cout << "Successfully opened the video fifo" << endl;
    }
    cout << "Thread has started" << endl;
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
    //drone_2_video = accept(parentfd, (struct sockaddr*) &client_addr_drone_2_video, &clientlen);
    //if(drone_2_video < 0)
    //{
	//perror("ERROR on accept Drone 2");
    //}
    ///***************************************************************************/
    //printf("Drone 2 accepted\n");
    //printf("\nBoth Drones connected to System\n");
    /****************Syncing the Drones sockets with server socket**************/
    /*Reading info from Drone 1*/		
    bzero(buffer,BUF_SIZE);
    n_1 = read(drone_1_video, buffer, 1);
    if(n_1<0)
    {
		perror("ERROR reading from Drone 1");
    }
    /**************************/
	printf("In thread received %s\n",buffer);
    /*Reading info from Drone 2*/
    //bzero(buffer,BUF_SIZE);
    //n_2 = read(drone_2_video, buffer, 1);
    //if(n_2 < 0)
    //{
	//error("ERROR reading from Drone 2");
    //}
    ///**************************/
    /*Just putting a NULL value in buffer so there is something in there*/
    buffer[0] = ' ';
    /* Sending sync info to Drone 1*/
    n_1=write(drone_1_video, buffer, strlen(buffer));
    if(n_1 < 0)
    {
		perror("ERROR writing to Drone 1");	
    }
    /**************************/
    char * buffer2;
    /*This is receiving the frames that we are sending over first and then receiving the data.*/
    while (1) 
    {
		bzero(buffer,BUF_SIZE);
		n_1 = read(drone_1_video, buffer, 6);
		if(n_1<0)
		{
			perror("ERROR reading from Drone 1 at getting frame size");
		}
		/**************************/
		printf("In thread received %s\n",buffer);
		
		int frameSize = atoi(buffer);
		printf("frameSize = %i\n", frameSize);
		buffer2 = new char(frameSize);
		bzero(buffer2,BUF_SIZE);
		int totalRead = 0;
		/*We are making sure that att he frameSize and data have been received and once it has been, we then write the the video_fifo to be able to display the video*/
		while (totalRead < frameSize)
		{
			n_1 = read(drone_1_video, &buffer2[totalRead], frameSize);
			if(n_1<0)
			{
				perror("ERROR reading from Drone 1 at getting frame");
			}
			printf("N_1=%i\ttotalRead=%i\tframeSize=%i\n", n_1, totalRead, frameSize);
			totalRead += n_1;
		}
		///**************************/
		//printf("In thread received %s\n",buffer2);
		fwrite(buffer2,frameSize,1,video_in);
		fflush(video_in);
		//delete(buffer2);
	}
    /* Sending sync info to Drone 1*/
    //n_2=write(drone_2_video, buffer, strlen(buffer));
    //if(n_2 < 0)
    //{
	//perror("ERROR writing to Drone 2");
    //}
    /**************************/
    /**********************************************************************/
	
}

int determineNeedToSendCommands()
{
    // BOTH need to send commands
    if(commBox.drone1Command.size() != 0 && commBox.drone2Command.size() != 0)
    {
        commBox.drone1Wait = true;
        commBox.drone2Wait = true;
        return 3;
    }
    if(commBox.drone1Command.size() != 0) {
        commBox.drone1Wait = true;
        return 1;
    }
    if(commBox.drone2Command.size() != 0) {
        commBox.drone2Wait = true;
        return 2;
    }
}