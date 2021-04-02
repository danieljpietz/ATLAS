#include <JetsonGPIO.h>
#include "Motor.h"
#include <unistd.h>
#include <math.h>
#include <chrono>
#include "Logger.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include "limitSwitchServer.h"

Motor* legMotorP;
Motor* kneeMotorP;

void signalHandler(int s)  {
	legMotorP->kill();
	//loggerP->setInActive();
	//loggerP->join();
	GPIO::cleanup();
	exit(0);
}

void shutdownHandler(int s) {
	std::cout << "Shutdown detected" << std::endl;
}

std::array<Motor*, 2> motors;

#define PORT     9090
#define MAXLINE 1024

void setMotor(int motor, double val) {
    std::cout << "Setting Motor " << motor << " to " << val << std::endl;
	if (motor == 1) {
		legMotorP->set(val);
	}
	else {
		kneeMotorP->set(val);
	}
}

double getMotor(int index) {
	std::cout << "Client asked for motor " << index << std::endl; 
    if (index == 1) {
		return legMotorP->get();
	}
	else {
		return kneeMotorP->get();
	}
}


void matlabSignalThread() {
int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr;
	struct sockaddr_in cliaddr;
      
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
      
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
      
    // Filling server information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
      
    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
      
    ssize_t n;
  
    socklen_t len;
    
    len = sizeof(cliaddr);  //len is value/resuslt
    std::cout << "MATLAB Server Ready" << std::endl;
    while(true) {
        buffer[0] = 0;
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        buffer[n] = '\0';
        std::string command = buffer;
        if (command == "Check") {
            sendto(sockfd, "We're Chillin", strlen("We're Chillin"),
                    0, (const struct sockaddr *) &cliaddr,
                        len);
        }
        
        if (command == "Set Motor") {
            buffer[0] = 0;
            n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
            buffer[n] = '\0';
            int arg1 = atoi(buffer);
            buffer[0] = 0;
            n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
            buffer[n] = '\0';
            double arg2 = atof(buffer);
            setMotor(arg1, arg2);
        }

		if (command == "Get Ankle") {
			const char* result = std::to_string(getMotor(1)).c_str();
			sendto(sockfd, result, strlen(result),
                    0, (const struct sockaddr *) &cliaddr,
                        len);
		}

		if (command == "Calibrate Ankle") {
			legMotorP->encoderCalibrate();
			sendto(sockfd, "Done", strlen("Done"),
                    0, (const struct sockaddr *) &cliaddr,
                        len);
		}

    }
}

int main(){
	// Pin Setup. 
	// Board pin-numbering scheme
	
	signal(SIGINT, signalHandler);
	signal(SIGKILL, shutdownHandler);

	

	GPIO::setmode(GPIO::BOARD);
	Motor legMotor(1, 33, 18, 19, 21);
	Motor kneeMotor(2, 32, 12, 22, 24);

	legMotorP = &legMotor;
	kneeMotorP = &kneeMotor;

	motors[0] = legMotorP;
	motors[1] = kneeMotorP;


	std::thread limitSwitchClient(clientThread);
	std::thread limitSwitchThread(serverThread, motors);


	usleep(1000000);
	legMotor.encoderCalibrate();

	usleep(1000);
	
	std::thread matlabThread(matlabSignalThread);

	
#if 0
	legMotor.setTarget(90);
	legMotor.setControlActive();
	
	
	while(true) {
		legMotor.setTarget(-legMotor.target());
		usleep(2000000);
	}
#endif
	//Logger logger(&legMotor);
	//loggerP = &logger;

	//legMotor.reset();
	//legMotor.resetTrinket();

	//legMotor.setTarget(45);
	//legMotor.setControlActive();


	

#if 0
	std::cout << "H" << std::endl;

	double x;

	std::cin >> x;

	legMotor.setTarget(-15);
	legMotor.setControlActive();
#if 0
	while(true) {
		legMotor.setTarget(-legMotor.target());
		usleep(2000000);
	}
#endif
#endif
	legMotor.join();
	return 0;
}