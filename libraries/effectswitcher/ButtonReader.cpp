#include <Arduino.h>
#include "ButtonReader.h"


ButtonReader::ButtonReader(byte _number_of_buttons, int _long_press_time) {
    number_of_buttons = _number_of_buttons;
    long_press_time = _long_press_time;
    buttons_state = new byte[number_of_buttons];
    buttons_action_time = new unsigned long[number_of_buttons];
    for (int i = 0; i < number_of_buttons; ++i) {
        buttons_state[i] = button_state::rest;
        buttons_action_time[i] = 0;
    }
}

int ButtonReader::get_actionned_button() {
    for (int i = 0; i < number_of_buttons; ++i) {
        if (buttons_state[i] != button_state::rest) {
            return i;
        }
    }
    return -1;
}

byte ButtonReader::get_action_for_button(int _button) {
    return buttons_state[_button];
}

void ButtonReader::update_button_state(byte button_number, byte bit_state) {
    int m = millis();
    if (bit_state == 1) {
        switch(buttons_state[button_number]) {
            case button_state::released:
            case button_state::rest:
                if (m - buttons_action_time[button_number] < DOUBLE_PRESSED_MAX_INTERVAL) {
                    buttons_state[button_number] = button_state::double_pressed;
                } else {
                    buttons_state[button_number] = button_state::pressed;
                    buttons_action_time[button_number] = m;
                }
                break;
            case button_state::long_pressed:
                buttons_state[button_number] = button_state::still_long_pressed;
                break;
            case button_state::pressed:
                buttons_state[button_number] = button_state::still_pressed;
                break;
            case button_state::still_pressed:
                if (m - buttons_action_time[button_number] > long_press_time) {
                    buttons_state[button_number] = button_state::long_pressed;
                }
                break;
        }
    } else {
        switch(buttons_state[button_number]) {
            case button_state::long_pressed:
            case button_state::pressed: 
            case button_state::still_pressed:
            case button_state::still_long_pressed: {
                buttons_state[button_number] = button_state::released;
                buttons_action_time[button_number] = m;
                break;
            }
            default: {
                buttons_state[button_number] = button_state::rest;
                break;
            }
        }
    }
}

// ButtonReaderRegister

ButtonReaderRegister::ButtonReaderRegister(
    byte _number_of_buttons, 
    byte _pin_register_load,
    byte _pin_register_clock,
    byte _pin_register_clock_enable,
    byte _pin_register_data_pin,
    byte _number_of_registers,
    int _long_press_time): 
    ButtonReader(_number_of_buttons, _long_press_time) {
    pin_register_load = _pin_register_load;
    pin_register_clock = _pin_register_clock;
    pin_register_clock_enable = _pin_register_clock_enable;
    pin_register_data_pin = _pin_register_data_pin;
    number_of_registers = _number_of_registers;
}

void ButtonReaderRegister::init() {
    pinMode(pin_register_load, OUTPUT);
    pinMode(pin_register_clock, OUTPUT);
    pinMode(pin_register_clock_enable, OUTPUT); 
    pinMode(pin_register_data_pin, INPUT);

    digitalWrite(pin_register_load, HIGH);
    digitalWrite(pin_register_clock_enable, HIGH);
}

void ButtonReaderRegister::read() {
    digitalWrite(pin_register_load, LOW);
    delayMicroseconds(5);
    digitalWrite(pin_register_load, HIGH);
    delayMicroseconds(5);
    
    digitalWrite(pin_register_clock, HIGH);
    digitalWrite(pin_register_clock_enable, LOW);
    byte incoming[number_of_registers];
    for (int i = 0; i < number_of_registers; ++i) {
        incoming[i] = shiftIn(pin_register_data_pin, pin_register_clock, MSBFIRST);
    }
    digitalWrite(pin_register_clock_enable, HIGH);

    for (int r = 0; r < number_of_registers; ++r) {
        for (int i = 0; i < 8; ++i) {
            int button_number = i + r * 8;
            if (button_number >= number_of_buttons) {
                continue;
            }
            update_button_state(button_number, bitRead(incoming[r], i));
        }
    }
}

// ButtonReaderPins

ButtonReaderPins::ButtonReaderPins(
    byte _number_of_buttons, 
    byte* _buttons_pin,
    int _long_press_time):
    ButtonReader(_number_of_buttons, _long_press_time) {
    buttons_pin = _buttons_pin;
}

void ButtonReaderPins::init() {
    for (int i = 0; i < number_of_buttons; ++i) {
        pinMode(buttons_pin[i], INPUT);
    }
}

void ButtonReaderPins::read() {
    for (int i = 0; i < number_of_buttons; ++i) {
        update_button_state(i, buttons_pin[i]);
    }
}
