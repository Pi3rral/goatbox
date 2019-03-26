#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

#define SERIAL_RATE 115200

class Debug {
private:
    static bool debug;
public:
    static void init(bool _debug = false);
    static void print(String message);
};

#endif //DEBUG_H