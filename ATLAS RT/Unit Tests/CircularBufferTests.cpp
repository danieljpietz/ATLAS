//
//  main.cpp
//  CircularBuffer
//
//  Created by Daniel Pietz on 11/5/20.
//

#include <iostream>
#include "CircularBuffer.h"
#include <thread>
using namespace ATLAS;

static CircularBuffer<100000, int> buffer;


void write() {
    for (size_t i = 0; i < 4*buffer.size(); ++i) {
        buffer.insert(static_cast<int>(i+1));
    }
}

void read() {
    for (size_t i = 0; i < 4*buffer.size(); ++i) {
        assert(buffer.at() != 0);
    }
}

void CircularBufferTests() {
    std::cout << "Testing MUTEX Read" << std::endl;
    std::cout << buffer.mutex() << std::endl;
    std::cout << "Test passed" << std::endl;

    std::cout << "Testing insert and read" << std::endl;
    for (size_t i = 0; i < 2*buffer.size(); ++i) {
        buffer.insert(static_cast<int>(i));
        assert(buffer.at() == static_cast<int>(i));
    }
    std::cout << "Test passed" << std::endl;

    std::cout << "Testing scrub" << std::endl;
    buffer.scrub();
    for (size_t i = 0; i < 2*buffer.size(); ++i) {
        assert(buffer.at() == 0);
    }
    std::cout << "Test passed" << std::endl;


    std::cout << "Testing threadsafe access" << std::endl;
    for (int i = 0; i < 1000; ++i) {
        std::cout << i << ", ";
        std::thread first(write);
        std::thread second(read);
        first.join();
        second.join();
    }
    std::cout << "Test passed" << std::endl;
}
