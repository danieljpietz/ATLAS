// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include "Motor.h"

#define PORT	 8080
#define MAXLINE 1024



void clientThread() {
	int rc = system("sudo python3 switchTest.py");
	std::cout << "Client ended with " << rc << std::endl;
}

// Driver code
void serverThread(std::array<Motor*, 2> motors) {
	int sockfd;
	char buffer[MAXLINE];
	struct sockaddr_in servaddr, cliaddr;
	
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
	
	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
	
	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	
	int n;

    char switchPort[2];

	socklen_t len = sizeof(cliaddr); //len is value/resuslt

    std::cout << "Limit Server Starting" << std::endl;
	while(true) {
        n = recvfrom(sockfd, (char *)buffer, MAXLINE,
				MSG_WAITALL, ( struct sockaddr *) &cliaddr,
				&len);
	    buffer[n] = '\0';
        switchPort[0] = buffer[0];
        switchPort[1] = buffer[1];
        if (strcmp(switchPort, "19") == 0) {
            motors[0]->limitSwitchHandler(0, buffer[n-1] == '1');
        } 
        else if (strcmp(switchPort, "21") == 0) {
            motors[0]->limitSwitchHandler(1, buffer[n-1] == '1');
        } 
#if 0
		if (strcmp(switchPort, "22") == 0) {
            motors[1]->limitSwitchHandler(0, buffer[n-1] == '1');
        } 
        else if (strcmp(switchPort, "24") == 0) {
            motors[1]->limitSwitchHandler(1, buffer[n-1] == '1');
        } 
#endif
    }
}

