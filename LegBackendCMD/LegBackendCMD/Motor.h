//
//  Motor.hpp
//  LegBackendCMD
//
//  Created by Daniel Pietz on 2/3/21.
//

#ifndef Motor_h
#define Motor_h

#include <cmath>

class Motor {
    unsigned short PWMPin;
    
    unsigned short GPIOPin;
    
public:
    
    Motor(unsigned short PWMPin, unsigned short GPIOPin) {
        this->PWMPin = PWMPin;
        this->GPIOPin = GPIOPin;
    }
  
    void setVoltage(double voltage) {
        bool GPIOVal = (voltage > 0);
        double PWMVal = std::abs(voltage);
    }
    
};

#endif /* Motor_hpp */
