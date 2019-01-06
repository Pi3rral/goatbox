#include <Arduino.h>
#include "GoatFather.h"
#include "BankManager.h"
#include "ButtonReader.h"

#define BASIC_MODE 1
#define BANK_MODE 2
#define EDIT_MODE 3

// bank manager
BankManager bank_manager(0, NUMBER_OF_BANKS, PATCHES_PER_BANK);
// button reader
ButtonReader button_reader;

// loop variables
byte mode = BASIC_MODE;
byte current_patch = 0;
byte current_effects = 0;


void setup() {
    //set pins to output for TPIC6B595N
    pinMode(CLOCK_PIN, OUTPUT); 
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(OUTPUT_ENABLE_PIN, OUTPUT);
    pinMode(DATA_PIN, OUTPUT);

    //set pins to output for 74HC165 
    pinMode(IN_LOAD_PIN, OUTPUT);
    pinMode(IN_CLOCK_PIN, OUTPUT);
    pinMode(IN_CLOCK_ENABLE_PIN, OUTPUT); 
    pinMode(IN_DATA_PIN, INPUT);

    Serial.begin(SERIAL_RATE);
    
    // initialize bank manager
    bank_manager.init(LOAD_PREDEFINED);

    // Enable output
    digitalWrite(OUTPUT_ENABLE_PIN, LOW);
    // Disable inputs
    digitalWrite(IN_LOAD_PIN, HIGH);
    digitalWrite(IN_CLOCK_ENABLE_PIN, HIGH);

    Serial.println("GoatFather initialized");

    unselect_all();
}

void loop() {
    button_reader.read();
    int button_actionned = button_reader.get_actionned_button();
    if (button_actionned >= 0) {
        int action = button_reader.get_action_for_button(button_actionned);
        // Serial.print("Button Actionned: ");
        // Serial.print(button_actionned);
        // Serial.print(" With State ");
        // Serial.println(button_reader.get_action_for_button(button_actionned));
        if (action == BUTTON_STATE_PRESSED) {
            if (button_actionned == PREV_BANK_BUTTON) {
                if (mode == EDIT_MODE) {
                    save_patch();
                } else {
                    bank_manager.previous();
                    Serial.println(String("Previous Bank: ") + bank_manager.get_current_bank_number());
                    if (bank_manager.get_current_bank_number() == BASIC_MODE_BANK) {
                        mode = BASIC_MODE;
                    } else {
                        mode = BANK_MODE;
                    }
                    unselect_all();
                }
            } else if (button_actionned == NEXT_BANK_BUTTON) {
                if (mode == EDIT_MODE) {
                    cancel_edit();
                } else {
                    bank_manager.next();
                    Serial.println(String("Next Bank: ") + bank_manager.get_current_bank_number());
                    if (bank_manager.get_current_bank_number() == BASIC_MODE_BANK) {
                        mode = BASIC_MODE;
                    } else {
                        mode = BANK_MODE;
                    }
                    unselect_all();
                }
            } else if (button_actionned == BOOST_BUTTON && mode != EDIT_MODE) {
                toggle_boost();
            } else {
                switch(mode) {
                    case BASIC_MODE: {
                        Serial.println("Basic Mode");
                        read_basic_mode(button_actionned);
                        break;            
                    }
                    case BANK_MODE: {
                        Serial.println("Bank Mode");
                        if (bitRead(current_patch, button_actionned)) {
                            unselect_all();
                        } else {
                            select_bank_patch(button_actionned);
                        }
                        break;
                    }
                    case EDIT_MODE: {
                        Serial.println("Edit Mode");
                        read_edit_mode(button_actionned);
                        break;
                    }
                }
            }
        } else if (action == BUTTON_STATE_LONG_PRESSED && mode == BANK_MODE) {
            if (button_actionned != NEXT_BANK_BUTTON && button_actionned != PREV_BANK_BUTTON) {
                Serial.println("Now in EDIT MODE");
                mode = EDIT_MODE;
            }
        }
    }
    delay(READING_RATE);
    return;
}

void read_basic_mode(int button_actionned) {
    if (bitRead(current_patch, button_actionned)) {
        Serial.println(String("Basic Mode: Deactivate ") + button_actionned);
        bitWrite(current_patch, button_actionned, LOW);
        bitWrite(current_effects, button_actionned, LOW);
    } else {
        Serial.println(String("Basic Mode: Activate ") + button_actionned);
        bitWrite(current_patch, button_actionned, HIGH);
        bitWrite(current_effects, button_actionned, HIGH);
    }
    select_patch_and_effect(current_patch, current_effects);
}

void read_edit_mode(int button_actionned) {
    if (bitRead(current_effects, button_actionned)) {
        Serial.println(String("Edit Mode: Deactivate ") + button_actionned);
        bitWrite(current_effects, button_actionned, LOW);
    } else {
        Serial.println(String("Edit Mode: Activate ") + button_actionned);
        bitWrite(current_effects, button_actionned, HIGH);
    }
    select_patch_and_effect(current_patch, current_effects);
}

void toggle_boost() {
    if (bitRead(current_effects, BOOST_PATCH_POSITION)) {
        Serial.println(String("Deactivate Boost "));
        bitWrite(current_effects, BOOST_PATCH_POSITION, LOW);
    } else {
        Serial.println(String("Activate Boost"));
        bitWrite(current_effects, BOOST_PATCH_POSITION, HIGH);
    }
    select_patch_and_effect(current_patch, current_effects);
}

void save_patch() {
    Serial.println("Save Effects");
    bank_manager.get_current_bank()->get_selected_patch()->save(current_effects);
    cancel_edit();
}

void cancel_edit() {
    Serial.println("Cancel Edit");
    mode = BANK_MODE;
    select_bank_patch(bank_manager.get_current_bank()->get_selected_patch_number());
}

void select_patch_and_effect(byte patch, byte effects) {
    // turn off the output so the pins don't light up
    // while you're shifting bits:
    digitalWrite(LATCH_PIN, LOW);
    digitalWrite(OUTPUT_ENABLE_PIN, HIGH);
    
    // shift the bits for patch
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, patch);
    
    // shift the bits for effects
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, effects);

    // turn on the output so the LEDs can light up:
    digitalWrite(LATCH_PIN, HIGH);
    digitalWrite(OUTPUT_ENABLE_PIN, LOW);
    current_patch = patch;
    current_effects = effects;
    delay(10);
}

void unselect_all() {
    Serial.println(String("Unselect All"));
    select_patch_and_effect(0, 0);
}

void select_bank_patch(byte patch_number) {
    unselect_all();
    byte effects = bank_manager.get_current_bank()->select_patch(patch_number)->get_selected_effects();
    byte patch_activated = 0;
    bitWrite(patch_activated, patch_number, HIGH);
    Serial.println(String("Select effects: ") + effects);
    select_patch_and_effect(patch_activated, effects);
}
