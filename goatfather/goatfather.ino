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
byte current_bank = 0;
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
        Serial.print("Button Actionned: ");
        Serial.print(button_actionned);
        Serial.print(" With State ");
        Serial.println(button_reader.get_action_for_button(button_actionned));
        if (action == BUTTON_STATE_PRESSED) {
            if (button_actionned == PREV_BANK_BUTTON) {
                if (mode == EDIT_MODE) {
                    save_patch();
                } else {

                }
            } else if (button_actionned == NEXT_BANK_BUTTON) {
                if (mode == EDIT_MODE) {
                    cancel_edit();
                } else {

                }
            } else {
                switch(mode) {
                    case BASIC_MODE: {
                        read_basic_mode(button_actionned);
                        break;            
                    }
                    case BANK_MODE: {
                        read_bank_mode(button_actionned);
                        break;
                    }
                    case EDIT_MODE: {
                        read_edit_mode(button_actionned);
                        break;
                    }
                }
            }
        } else if (action == BUTTON_STATE_LONG_PRESSED) {
    delay(1000);
    return;
}

void read_basic_mode() {
    // if (button_action.long_pressed && button_action.button == CHANGE_BANK_BUTTON) {
    //     set_change_bank_mode_on();
    // } else if (button_action.pressed) {
    //     if (bitRead(current_patch_and_effects, button_action.button)) {
    //         Serial.println(String("Basic Mode: Deactivate ") + button_action.button);
    //         bitWrite(current_patch_and_effects, button_action.button, LOW);
    //         bitWrite(current_patch_and_effects, button_action.button + NUMBER_OF_EFFECTS, LOW);
    //     } else {
    //         Serial.println(String("Basic Mode: Activate ") + button_action.button);
    //         bitWrite(current_patch_and_effects, button_action.button, HIGH);
    //         bitWrite(current_patch_and_effects, button_action.button + NUMBER_OF_EFFECTS, HIGH);
    //     }
    //     select_patch_and_effect(current_patch_and_effects);
    //     Serial.println("wait_for_release - read_basic_mode");
    //     wait_for_release = true;
    // }
}

void activate_selected_bank() {
    // Serial.println(String("Selected bank: ") + current_bank);
    // if (current_bank == BASIC_MODE_BANK) {
    //     Serial.println("Set Basic Mode");
    //     mode = BASIC_MODE;
    // } else {
    //     Serial.println("Set Bank Mode");
    //     mode = BANK_MODE;
    //     current_patch = 0;
    // }
    // set_change_bank_mode_off();
}

void read_bank_mode() {
    // if (button_action.long_pressed && button_action.button == CHANGE_BANK_BUTTON) {
    //     set_change_bank_mode_on();
    // } else if (button_action.pressed) {
    //     Serial.println(String("Bank Mode Reading"));
    //     if (button_action.button != current_patch) {
    //         Serial.println(String("Select Patch: ") + button_action.button);
    //         select_bank_patch(button_action.button);
    //     } else {
    //         unselect_all();
    //     }
    //     Serial.println("wait_for_release - read_bank_mode - previous");
    //     wait_for_release = true;
    // }
}

void read_edit_mode() {

}

void save_patch() {

}

void cancel_edit() {
    
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
    int effects = bank_manager.get_current_bank()->select_patch(current_patch)->get_selected_effects();
    Serial.println(String("Select effects: ") + int(current_effects));
    select_patch_and_effect(patch_number, (byte)effects);
}
