#include <JetsonGPIO.h>
#include "Motor.h"
#include <unistd.h>
#include <math.h>
#include <chrono>
#include "Logger.h"
#include <signal.h>

Motor* legMotorP;
Logger* loggerP;
void signalHandler(int s)  {
	legMotorP->kill();
	loggerP->setInActive();
	loggerP->join();
	exit(0);
}

void shutdownHandler(int s) {
	std::cout << "Shutdown detected" << std::endl;
}

int main(){

	// Pin Setup. 
	// Board pin-numbering scheme
	
	signal(SIGINT, signalHandler);
	signal(SIGKILL, shutdownHandler);

	GPIO::setmode(GPIO::BOARD);
	Motor legMotor(1, 33, 18);
	legMotorP = &legMotor;
	Logger logger(&legMotor);
	loggerP = &logger;

	legMotor.reset();
	legMotor.resetTrinket();

	std::cout << "H" << std::endl;

	while(true) {
		legMotor.set(1);
	}

	legMotor.join();
	return 0;
}