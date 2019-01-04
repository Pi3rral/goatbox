#include <Arduino.h>
#include "ButtonReader.h"
#include "GoatFather.h"

ButtonReader::ButtonReader() {
    for (int i = 0; i < NUMBER_OF_BUTTON; ++i) {
        buttons_state[i] = BUTTON_STATE_REST;
        buttons_action_time[i] = 0;
    }
}

void ButtonReader::read_states() {
    digitalWrite(IN_LOAD_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(IN_LOAD_PIN, HIGH);
    delayMicroseconds(5);
    
    digitalWrite(IN_CLOCK_PIN, HIGH);
    digitalWrite(IN_CLOCK_ENABLE_PIN, LOW);
    byte incoming[NUMBER_OF_SHIFT_REGISTER]
    for (int i = 0; i < NUMBER_OF_SHIFT_REGISTER) {
        incoming[i] = shiftIn(IN_DATA_PIN, IN_CLOCK_PIN, LSBFIRST);
    }
    digitalWrite(IN_CLOCK_ENABLE_PIN, HIGH);
    Serial.println(incoming[0]);
    Serial.println(incoming[1]);

    for (int i = 0; i < NUMBER_OF_BUTTON; ++i) {
        if (bitRead(incoming, i) == 1) {
            switch(buttons_state[i]) {
                case BUTTON_STATE_RELEASED:
                case BUTTON_STATE_REST: {
                    buttons_state[i] = BUTTON_STATE_PRESSED;
                    buttons_action_time[i] = millis();
                    break;
                }
                case BUTTON_STATE_LONG_PRESSED:
                case BUTTON_STATE_PRESSED: {
                    buttons_state[i] = BUTTON_STATE_STILL_PRESSED;
                    break;
                }
                case BUTTON_STATE_STILL_PRESSED: {
                    if (millis() - buttons_action_time[i] > LONG_PRESS_TIME) {
                        buttons_state[i] = BUTTON_STATE_LONG_PRESSED;
                    }
                    break;
                }
            }
        } else {
            switch(buttons_state[i]) {
                case BUTTON_STATE_LONG_PRESSED:
                case BUTTON_STATE_PRESSED: 
                case BUTTON_STATE_STILL_PRESSED: {
                    buttons_state[i] = BUTTON_STATE_RELEASED;
                    break;
                }
                default: {
                    buttons_state[i] = BUTTON_STATE_REST;
                    break;
                }
            }
        }
    }
    return buttons_state;
}
