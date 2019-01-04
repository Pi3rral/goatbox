#include <Arduino.h>
#include "GoatFather.h"
#include "ButtonReader.h"

ButtonReader::ButtonReader() {
    for (int i = 0; i < NUMBER_OF_BUTTON; ++i) {
        buttons_state[i] = BUTTON_STATE_REST;
        buttons_action_time[i] = 0;
    }
}

void ButtonReader::read() {
    digitalWrite(IN_LOAD_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(IN_LOAD_PIN, HIGH);
    delayMicroseconds(5);
    
    digitalWrite(IN_CLOCK_PIN, HIGH);
    digitalWrite(IN_CLOCK_ENABLE_PIN, LOW);
    byte incoming[NUMBER_OF_SHIFT_REGISTER];
    for (int i = 0; i < NUMBER_OF_SHIFT_REGISTER; ++i) {
        incoming[i] = shiftIn(IN_DATA_PIN, IN_CLOCK_PIN, MSBFIRST);
    }
    digitalWrite(IN_CLOCK_ENABLE_PIN, HIGH);

    Serial.println("=============");
    Serial.println("START READING");
    Serial.println("=============");
    for (int r = 0; r < NUMBER_OF_SHIFT_REGISTER; ++r) {
        for (int i = 0; i < 8; ++i) {
            int button_number = i + r * 8;
            if (button_number >= NUMBER_OF_BUTTON) {
                continue;
            }
            Serial.print("Button ");
            Serial.print(button_number);
            Serial.print(" Current State ");
            Serial.print(buttons_state[button_number]);
            Serial.print(" Value ");
            if (bitRead(incoming[r], i) == 1) {
                Serial.print(1);
                switch(buttons_state[button_number]) {
                    case BUTTON_STATE_RELEASED:
                    case BUTTON_STATE_REST:
                        buttons_state[button_number] = BUTTON_STATE_PRESSED;
                        buttons_action_time[button_number] = millis();
                        break;
                    case BUTTON_STATE_LONG_PRESSED:
                        buttons_state[button_number] = BUTTON_STATE_STILL_LONG_PRESSED;
                        break;
                    case BUTTON_STATE_PRESSED:
                        buttons_state[button_number] = BUTTON_STATE_STILL_PRESSED;
                        break;
                    case BUTTON_STATE_STILL_PRESSED:
                        if (millis() - buttons_action_time[button_number] > LONG_PRESS_TIME) {
                            buttons_state[button_number] = BUTTON_STATE_LONG_PRESSED;
                        }
                        break;
                }
            } else {
                Serial.print(0);
                switch(buttons_state[button_number]) {
                    case BUTTON_STATE_LONG_PRESSED:
                    case BUTTON_STATE_PRESSED: 
                    case BUTTON_STATE_STILL_PRESSED:
                    case BUTTON_STATE_STILL_LONG_PRESSED:
                        buttons_state[button_number] = BUTTON_STATE_RELEASED;
                        break;
                    default:
                        buttons_state[button_number] = BUTTON_STATE_REST;
                        buttons_action_time[button_number] = 0;
                        break;
                }
            }
            Serial.print(" New State ");
            Serial.println(buttons_state[button_number]);

        }
    }
    Serial.println("===========");
    Serial.println("END READING");
    Serial.println("===========");
}

int ButtonReader::get_actionned_button() {
    for (int i = 0; i < NUMBER_OF_BUTTON; ++i) {
        if (buttons_state[i] != BUTTON_STATE_REST) {
            return i;
        }
    }
    return -1;
}

int ButtonReader::get_action_for_button(int _button) {
    return buttons_state[_button];
}
