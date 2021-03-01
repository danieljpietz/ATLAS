#include <iostream>
#include "arduino-serial-lib.h"
#include "Sensor.h"
#include <chrono>

#define now std::chrono::high_resolution_clock::now()


int main()  {

    // Saturation and Timig Testing. Current average response time
    // To Encoder request is 48.7498 microseconds
    // done over 1M requests

    EncoderTrinket encoderTrinket("/dev/ttyACM0");
    auto t1 = now;
    for(int i = 0; i < 100000; ++i) {
        encoderTrinket.getMotor1Value();
    }
    auto t2 = now;
    auto t3 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::cout << float(t3)/100000 << std::endl;


    return 1;
}