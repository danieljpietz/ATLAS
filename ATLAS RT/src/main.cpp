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

int main(){

	// Pin Setup. 
	// Board pin-numbering scheme
	
	signal(SIGINT, signalHandler);

	GPIO::setmode(GPIO::BOARD);
	Motor legMotor(1, 33, 18);
	legMotorP = &legMotor;
	Logger logger(&legMotor);
	loggerP = &logger;

	legMotor.reset();
	legMotor.setTarget(1080);
	legMotor.setControlActive();

	while(true) {
		std::cout << legMotor.getSpeedAveraged() << std::endl;
		usleep(10000);
	}

	legMotor.join();
	return 0;
}