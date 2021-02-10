#ifndef Motor_h
#define Motor_h
#include <JetsonGPIO.h>
#include <assert.h>
#include "Sensor.h"
#include "CircularBuffer.h"
#define motorGPIOFreq 240
#define clicksPerRevolution 5264

EncoderTrinket encoderTrinket("/dev/ttyACM0");

class Motor {
    unsigned short port1;
    unsigned short port2;
    int index;
    ATLAS::CircularBuffer<100, long> timeStamps;
    ATLAS::CircularBuffer<10, double> speedBuffer;
    ATLAS::CircularBuffer<100, double> history;
    GPIO::PWM* p;
    double _voltage;
    
public:

    Motor(const unsigned short index, const unsigned short port1, const unsigned short port2) {
        this->port1 = port1;
        this->port2 = port2;
        assert(index == 1 || index == 2);
        this->index = index;
        assert(port1 == 32 || port1 == 33);
        GPIO::setup(port1, GPIO::OUT, GPIO::LOW);
        GPIO::setup(port2, GPIO::OUT, GPIO::LOW);
	    this->p = new GPIO::PWM(port1, motorGPIOFreq);
    }

    void set(double scale) {
        int mult = scale > 0 ? 1 : -1;
        GPIO::output(this->port2, (mult == 1 ? GPIO::HIGH : GPIO::LOW));
        if (scale > 1) {
            scale = 1;
        } else if (scale < -1) {
            scale = -1;
        }

        this->_voltage = mult*scale;

        (scale != 0) ? this->p->start(this->_voltage) : this->p->stop(); 
    }

    double voltage() {
        return this->_voltage;
    }

    void update() {
        double val = (360.0 * (this->index == 1 ? encoderTrinket.getMotor1Value() : encoderTrinket.getMotor2Value())) / clicksPerRevolution;
        this->history.insert(val);
        this->timeStamps.insert(getTime<std::chrono::microseconds>());
        this->speedBuffer.insert((1000000 * (this->history.at() - this->history.at(this->history.index() - 1))) / (this->timeStamps.at() - this->timeStamps.at(this->timeStamps.index() - 1)));
    }

    double get() {
        this->update();
        return this->history.at();
    }

    double getSpeed() {
        this->update();
        return this->speedBuffer.at();
    }

    double getSpeedAveraged() {
        this->update();
        return this->speedBuffer.mean();
    }

    void reset() {
        encoderTrinket.resetMotorCounts();
    }

};

#endif