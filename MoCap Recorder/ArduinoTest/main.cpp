/*
 * arduino-serial
 * --------------
 *
 * A simple command-line example program showing how a computer can
 * communicate with an Arduino board. Works on any POSIX system (Mac/Unix/PC)
 *
 *
 * Compile with something like:
 *   gcc -o arduino-serial arduino-serial-lib.c arduino-serial.c
 * or use the included Makefile
 *
 * Mac: make sure you have Xcode installed
 * Windows: try MinGW to get GCC
 *
 *
 * Originally created 5 December 2006
 * 2006-2013, Tod E. Kurt, http://todbot.com/blog/
 *
 *
 * Updated 8 December 2006:
 *  Justin McBride discovered B14400 & B28800 aren't in Linux's termios.h.
 *  I've included his patch, but commented out for now.  One really needs a
 *  real make system when doing cross-platform C and I wanted to avoid that
 *  for this little program. Those baudrates aren't used much anyway. :)
 *
 * Updated 26 December 2007:
 *  Added ability to specify a delay (so you can wait for Arduino Diecimila)
 *  Added ability to send a binary byte number
 *
 * Update 31 August 2008:
 *  Added patch to clean up odd baudrates from Andy at hexapodia.org
 *
 * Update 6 April 2012:
 *  Split into a library and app parts, put on github
 *
 * Update 20 Apr 2013:
 *  Small updates to deal with flushing and read backs
 *  Fixed re-opens
 *  Added --flush option
 *  Added --sendline option
 *  Added --eolchar option
 *  Added --timeout option
 *  Added -q/-quiet option
 *
 */
#include <stdio.h>    // Standard input/output definitions
#include "Sensor.h"
#include <chrono>
#include <fstream>
#include <array>
#include <queue>
#include <thread>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define BUFFER_SIZE 100000

void fileQueue(void);
void pythonLaunchThread(void);

std::queue<long> timeQueue;
std::queue<long> emg0Queue;
std::queue<long> emg1Queue;
std::queue<long> emg2Queue;
std::queue<long> emg3Queue;

bool isDone = false;
bool fileThreadSetupDone = false;

std::string direc;
std::string file;

std::thread fileThread;
std::thread pythonThread;

void fileQueue() {
    
    time_t _tm =time(NULL );
    struct tm * curtime = localtime ( &_tm );
    direc = "Results/" + std::string(asctime(curtime));
    direc = direc.substr(0, direc.length() - 1);
    file = direc + "/EMG.csv";
    mkdir(direc.c_str(), 0777);
    std::ofstream myfile;
    myfile.open (file);
    
    fileThreadSetupDone = true;
    
    while (!isDone) {
        if (!timeQueue.empty() && (timeQueue.size() == emg0Queue.size()) && (timeQueue.size() == emg1Queue.size()) && (timeQueue.size() == emg2Queue.size()) && (timeQueue.size() == emg3Queue.size())) {
            myfile << timeQueue.front() << ", " << emg0Queue.front() <<
            ", " << emg1Queue.front() << ", " << emg2Queue.front() <<
            ", " << emg3Queue.front() << std::endl;
            
            timeQueue.pop();
            emg0Queue.pop();
            emg1Queue.pop();
            emg2Queue.pop();
            emg3Queue.pop();
            
        }
    }
    
}

void pythonLaunchThread() {
    const long vIn = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::string call("python3 pythonFile.py ");
    system((call + std::to_string(vIn) + " '" + file + "'").c_str());
    isDone = true;
}


int main(int argc, char *argv[]) {
   
    std::cout << "Bootup" << std::endl;
    
    fileThread = std::thread(fileQueue);
    
    EMGTrinket trinket("/dev/cu.usbmodem14101");
    trinket.reset();
    
    while (!fileThreadSetupDone) {
        usleep(1);
    }
    
    pythonThread = std::thread(pythonLaunchThread);
    
    auto t1 = std::chrono::high_resolution_clock::now();
    long currentTime = 0;
    while (!isDone) {
        currentTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - t1).count();
        timeQueue.push(currentTime);
        emg0Queue.push(trinket.getEMG(0));
        emg1Queue.push(trinket.getEMG(1));
        emg2Queue.push(trinket.getEMG(2));
        emg3Queue.push(trinket.getEMG(3));

    }
    pythonThread.join();
    fileThread.join();
    return 1;
}
