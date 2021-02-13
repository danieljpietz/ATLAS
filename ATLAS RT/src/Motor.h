#ifndef Motor_h
#define Motor_h
#include <JetsonGPIO.h>
#include <assert.h>
#include "Sensor.h"
#include "CircularBuffer.h"
#include <thread>
#include <functional>
#include <mutex>
#define motorGPIOFreq 64*128
#define clicksPerRevolution 5264

EncoderTrinket encoderTrinket("/dev/ttyACM0");

class Motor {
    unsigned short port1;
    unsigned short port2;
    int index;
    ATLAS::CircularBuffer<100, long> timeStamps;
    ATLAS::CircularBuffer<100, double> speedBuffer;
    ATLAS::CircularBuffer<100, double> history;
    GPIO::PWM* p;
    double _voltage = 0;
    double _target = 0;
    double _error = 0;
    std::thread controlThread;

    std::function<double(double target)> controlLaw;

    bool useControlLaw;
    bool shouldKeepAlive;
    std::mutex mtx;


    double P = 0.5;
    double I = 0.005;
    double D = 00;
    
    double PID(double target) {
        return this->P*(target - this->get()) + I*this->error();
    }

    void motorControlThread() {

        while (this->shouldKeepAlive) {
        this->mtx.lock();
        std::cout << "UNLOCKED" << std::endl;
        this->mtx.unlock();
        while (this->useControlLaw) {
            this->set(this->PID(this->_target));
            usleep(5000/2);
        }
        }
    }


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
        this->mtx.lock();
        this->controlThread = std::thread(&Motor::motorControlThread, this);
        this->shouldKeepAlive = true;
    }


    void set(double scale) {
        int mult = scale > 0 ? 1 : -1;
        GPIO::output(this->port2, (mult == 1 ? GPIO::LOW : GPIO::HIGH));
        if (scale > 1) {
            scale = 1;
        } else if (scale < -1) {
            scale = -1;
        }
        this->_voltage = mult*scale;

        (scale != 0) ? this->p->start(100* this->_voltage) : this->p->stop(); 
       
    }
    double voltage() {
        return this->_voltage;
    }

    auto join() {
        this->set(0);
        return this->controlThread.join();
    }

    void setControlActive() {
        this->mtx.unlock();
    }

    void setControlInActive() {
        this->mtx.lock();
        this->useControlLaw = false;
    }

    auto kill() {
        this->shouldKeepAlive = false;
        this->useControlLaw = false;
        this->mtx.unlock();
        return this->join();
    }

    void setTarget(double target) {
        this->_error = 0;
        this->_target = target;
    }

    double target() {
        return this->_target;
    }

    void update() {
        double val = (360.0 * (this->index == 1 ? encoderTrinket.getMotor1Value() : encoderTrinket.getMotor2Value())) / clicksPerRevolution;
        double errorTemp = this->_target - val;
        this->_error += (abs(errorTemp) < 1) ? errorTemp : 0;
        if(errorTemp > 1) {
            this->_error = 1;
        } else if(errorTemp < -1) {
            this->_error = -1;
        }
        this->history.insert(val);
        this->timeStamps.insert(getTime<std::chrono::microseconds>());
        this->speedBuffer.insert((1000000 * (this->history.at() - this->history.at(this->history.index() - 1))) / (this->timeStamps.at() - this->timeStamps.at(this->timeStamps.index() - 1)));
    }

    double get() {
        this->update();
        return this->history.at();
    }

    double error() {
        return this->_error;
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