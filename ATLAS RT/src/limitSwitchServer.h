#ifndef limitSwitch__h
#define limitSwitch__h

#include <array>
#include "Motor.h"

void clientThread();
void serverThread(std::array<Motor*, 2> motors);
#endif