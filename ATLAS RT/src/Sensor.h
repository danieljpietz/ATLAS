#include "arduino-serial-lib.h"
#include <string.h>
#include <stdlib.h>
#include <array>
#include <iostream>
#include <unistd.h>

#define buf_max 256
#define baudrate 4000000
#define timeout 1000
#define eolchar '\n'

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
        serialport_read_until(fd,  result,  eolchar,  buf_max,  timeout);
    }

};


class EncoderTrinket : public Trinket {
public:

    EncoderTrinket(const char* address) : Trinket(address) {
        
    }

    void resetMotorCounts() {
        char result[6];
        sendRequest("?", result);
        if (strcmp(result, "RESET\n")) {
            std::cout << "ERROR RESETTING MOTOR COUNT:" << result << std::endl;
        }
        
        else {
            std::cout << "Reset Succesful" << std::endl;
        }
        
    }

    short getMotor1Value() {
        char buf[6];
        sendRequest("1", buf);
        return static_cast<short>(atoi(buf));
    }

    short getMotor2Value() {
        char buf[6];
        sendRequest("2", buf);
        return static_cast<short>(atoi(buf));
    }

    std::array<short, 2> getMotorValues() {
        return {getMotor1Value(), getMotor2Value()};
    }

};