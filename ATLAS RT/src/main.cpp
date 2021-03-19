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
	//legMotorP->kill();
	//loggerP->setInActive();
	//loggerP->join();
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
	Motor legMotor(1, 33, 18, 11, 13);
	legMotorP = &legMotor;
	//Logger logger(&legMotor);
	//loggerP = &logger;

	legMotor.reset();
	//legMotor.resetTrinket();

	legMotor.setTarget(360);
	legMotor.setControlActive();
	

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