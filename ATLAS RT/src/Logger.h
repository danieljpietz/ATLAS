#include "Motor.h"
#include <string>
#include <thread>
#include <chrono>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <ctime>
#include <fstream>
#include <iostream>


class Logger {

std::chrono::_V2::system_clock::time_point startTime;
bool shouldUpdate;

Motor* legMotor;
std::ofstream legMotorLog;

double updateFrequency = 100;
std::thread updateThread;



public:

Logger(Motor* legMotor) {


    std::time_t result = std::time(nullptr);
    const std::string timeString = std::asctime(std::localtime(&result));
    std::cout << "Starting log at " << timeString << std::endl;;

    mkdir("Logs", 0777);


    mkdir(("Logs/" + timeString).c_str(), 0777);

    
    this->legMotorLog.open(("Logs/" + timeString + "/Leg Motor"));

    this->legMotor = legMotor;

    this->shouldUpdate = true;
    startTime = std::chrono::high_resolution_clock::now(); 
    this->updateThread = std::thread(&Logger::update, this);
    
    
}

void setInActive() {
    this->shouldUpdate = false;
}

auto join() {
        return this->updateThread.join();
    }



void update() {

    auto captureTime = std::chrono::high_resolution_clock::now();
    long timeDiff = 0;
    while(this->shouldUpdate) {
        captureTime = std::chrono::high_resolution_clock::now();
        timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(captureTime - this->startTime).count();

        this->legMotorLog << (float)timeDiff/1000.0 << ", " << this->legMotor->voltage() << ", " << this->legMotor->target() << ", " << this->legMotor->get() << ", " <<this->legMotor->getSpeed() << std::endl;    
        usleep(1000000/this->updateFrequency);
    }
    this->legMotorLog.close();
}

};