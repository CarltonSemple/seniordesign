#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>


void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, mlen;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[1];
	int addrsize = sizeof(struct sockaddr_in);
	int amount;
    
    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	 if (sockfd < 0) 
		error("ERROR opening socket");
	 bzero((char *) &serv_addr, sizeof(serv_addr));
	 serv_addr.sin_family = AF_INET;
	 serv_addr.sin_addr.s_addr = INADDR_ANY;
	 serv_addr.sin_port = htons(32323);
	 if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		error("ERROR on binding");
	 addrsize = sizeof(struct sockaddr_in);
	 
	 bzero(buffer,256);
	 printf("Waiting...\n");
	 mlen = recvfrom(sockfd, buffer, 1, 0, (struct sockaddr *) &serv_addr, &addrsize);
	 printf("Received \"%s\" from client %s\n", buffer, inet_ntoa (serv_addr.sin_addr));
	
	// Send to server
	while (buffer[0] != 'q') {
		bzero(buffer,256);
		
		// Get input
		printf("Enter input for drone movement\n(A: Left, W: Forward, S: Backward, D: Right, Q: Quit)\n");
		scanf("%s", buffer);
		if (buffer[0] != 'q') {
			if (buffer[0] == 'a' || buffer[0] == 'd')
				printf("Enter number of 40 degrees to turn\n");
			else if (buffer[0] == 'w' || buffer[0] == 's')
				printf("Enter number of inches to move\n");
			scanf("%i", &amount);
			
			// Move drone by sending buffer amount times
			for (int i=0; i<amount; i++) {
				printf("Sending %s", buffer);
				if (sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0)
					error("ERROR with sento()");
				printf("...Sent\n");
			}
			
			// Stop moving drone
			buffer[0] = 'x';
			printf("Sending %s", buffer);
			if (sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0)
				error("ERROR with sento()");
			printf("...Sent\n");
		}
		else {
			// Send quit to drone
			printf("Sending %s", buffer);
			if (sendto(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0)
				error("ERROR with sento()");
			printf("...Sent\n");
		}
		
	}
		
    close(sockfd);
    return 0;
}

