#include <JetsonGPIO.h>
#include "Motor.h"
#include <unistd.h>
#include <math.h>
#include <chrono>
int main(){

	// Pin Setup. 
	// Board pin-numbering scheme
	GPIO::setmode(GPIO::BOARD);

	auto t1 = std::chrono::high_resolution_clock::now();
	auto t2 = std::chrono::high_resolution_clock::now();
	auto t3 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

	Motor legMotor(1, 33, 18);
	legMotor.reset();
	
	double P = 0.1;
	double D = 0.05;
	
	while(t3 < 1000*100) {
		t2 = std::chrono::high_resolution_clock::now();
		t3 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		legMotor.set(-sin(static_cast<double>(t3)/1000.0));
		//legMotor.set(P * (180 - legMotor.get()) - D * legMotor.getSpeedAveraged());
		std::cout <<  legMotor.voltage() << " , " << legMotor.get() << std::endl;
		usleep(5000);
	}
	legMotor.set(0);
	return 0;
}