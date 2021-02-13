#include "arduino-serial-lib.h"
#include <string.h>
#include <stdlib.h>
#include <array>
#include <iostream>
#include <unistd.h>
#include <chrono>

#define buf_max 256
#define baudrate 2000000
#define timeout 1000
#define eolchar '\n'

std::chrono::_V2::system_clock::time_point startTime = std::chrono::high_resolution_clock::now();

template <typename timeType>
    long getTime() {
        return std::chrono::duration_cast<timeType>(std::chrono::high_resolution_clock::now() - startTime).count();
    }

class Trinket {
    const char* address;
protected:

    
    int fd = -1;
    char serialport[buf_max];
    char buf[buf_max];
    int rc,n;

    Trinket(const char* address) {
        this->address = address;
        this->fd = serialport_init(address, baudrate);
    }

    void sendRequest(const char* request, char* result) {
serialWrite:
        try {
            serialport_write(fd, request);
        }
        catch (int x){
            usleep(500);       // Resource was busy, wait 1ms and try again
            goto serialWrite;
        }
        char discard[1];
        serialport_read_until(fd,  result,  eolchar,  buf_max,  timeout);
        serialport_read_until(fd,  discard,  eolchar,  buf_max,  timeout);
    }

};


class EncoderTrinket : public Trinket {
public:

    EncoderTrinket(const char* address) : Trinket(address) {
        
    }

    void resetMotorCounts() {
        char result[8];
        sendRequest("?", result);
        std::cout << "RESET MESSAGE: " << std::string(result);
        #if 0
        if (std::string(result) !=  "RESET\n") {
            std::cout << "ERROR RESETTING MOTOR COUNT: " << result;
        }
        
        else {
            std::cout << "Reset Succesful" << std::endl;
        }
        #endif
    }

    int getMotor1Value() {
        char buf[12];
        sendRequest("0", buf);
        int ret = atoi(buf);
        if (ret > 4290000000) {
            ret -= 4294967295;
        }
        return ret;
    }

    int getMotor2Value() {
        char buf[12];
        sendRequest("1", buf);
        return static_cast<int>(atoi(buf));
    }

    std::array<int, 2> getMotorValues() {
        return {getMotor1Value(), getMotor2Value()};
    }

};
