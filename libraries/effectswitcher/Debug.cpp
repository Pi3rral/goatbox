#include "Debug.h"

bool Debug::debug = false;

void Debug::init(bool _debug) {
    debug = _debug;
    if (debug) {
        Serial.begin(SERIAL_RATE);
    }
}

void Debug::print(String message) {
    if (debug) {
        Serial.println(message);
    }
}
