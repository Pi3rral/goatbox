#include "EffectSwitcher.h"
#include "ButtonReader.h"

#define SERIAL_RATE 115200
#define READING_RATE 100
#define BASIC_MODE_BANK 0


EffectSwitcher::EffectSwitcher(
    ButtonReader * _button_reader, 
    BankManager* _bank_manager, 
    LiquidCrystal_I2C* _lcd,
    byte _pin_register_clock,
    byte _pin_register_latch,
    byte _pin_register_output_enable,
    byte _pin_register_data) {
    bank_manager = _bank_manager;
    button_reader = _button_reader;
    lcd = _lcd;
    pin_register_clock = _pin_register_clock;
    pin_register_latch = _pin_register_latch;
    pin_register_output_enable = _pin_register_output_enable;
    pin_register_data = _pin_register_data;
    mode = effects_mode::basic;
    current_patch = 0;
    current_effects = 0;
}

void EffectSwitcher::init_lcd() {
    if (lcd) {
        lcd->init();
        lcd->backlight();
        lcd->clear();
        lcd->setCursor(0,0);
    }
}

void EffectSwitcher::init_register() {
    //set pins to output for TPIC6B595N
    pinMode(pin_register_clock, OUTPUT); 
    pinMode(pin_register_latch, OUTPUT);
    pinMode(pin_register_output_enable, OUTPUT);
    pinMode(pin_register_data, OUTPUT);
    // Enable output
    digitalWrite(pin_register_output_enable, LOW);
}

void EffectSwitcher::init() {
    Serial.begin(SERIAL_RATE);
    init_lcd();
    display("Starting...", 0, 0, true);
    init_register();
    button_reader->init();
    bank_manager->init();
    unselect_all();
    display("Effect Mode", 0, 0, true);
    Serial.println("GoatFather initialized");
}

void EffectSwitcher::clear_display() {
    if (lcd) {
        lcd->clear();
        lcd->setCursor(0, 0);
    }
}

void EffectSwitcher::display(String message, int row, int col, bool clear) {
    if (lcd) {
        if (clear) {
            clear_display();
        }
        lcd->setCursor(col, row);
        lcd->print(message);
    }
}

void EffectSwitcher::read_and_apply() {
    button_reader->read();
    int button_actionned = button_reader->get_actionned_button();
    if (button_actionned >= 0) {
        int action = button_reader->get_action_for_button(button_actionned);
        // Serial.print("Button Actionned: ");
        // Serial.print(button_actionned);
        // Serial.print(" With State ");
        // Serial.println(button_reader->get_action_for_button(button_actionned));
        if (action == button_state::pressed) {
            if (button_actionned == PREV_BANK_BUTTON) {
                if (mode == effects_mode::edit) {
                    save_patch();
                } else {
                    bank_manager->previous();
                    Serial.println(String("Previous Bank: ") + bank_manager->get_current_bank_number());
                    if (bank_manager->get_current_bank_number() == BASIC_MODE_BANK) {
                        mode = effects_mode::basic;
                        lcd->clear();
                        lcd->setCursor(0,1);
                        lcd->print("Effect Mode");
                    } else {
                        mode = effects_mode::bank;
                        lcd->clear();
                        lcd->setCursor(0,1);
                        lcd->print(String("Bank ") + bank_manager->get_current_bank_number());
                    }
                    unselect_all();
                }
            } else if (button_actionned == NEXT_BANK_BUTTON) {
                if (mode == effects_mode::edit) {
                    cancel_edit();
                } else {
                    bank_manager->next();
                    Serial.println(String("Next Bank: ") + bank_manager->get_current_bank_number());
                    if (bank_manager->get_current_bank_number() == BASIC_MODE_BANK) {
                        mode = effects_mode::basic;
                        lcd->clear();
                        lcd->setCursor(0,1);
                        lcd->print("Effect Mode");
                    } else {
                        mode = effects_mode::bank;
                        lcd->clear();
                        lcd->setCursor(0,1);
                        lcd->print(String("Bank ") + bank_manager->get_current_bank_number());
                    }
                    unselect_all();
                }
            } else if (button_actionned == BOOST_BUTTON && mode != effects_mode::edit) {
                toggle_boost();
            } else {
                switch(mode) {
                    case effects_mode::basic: {
                        Serial.println("Read Basic Mode");
                        read_basic_mode(button_actionned);
                        break;            
                    }
                    case effects_mode::bank: {
                        Serial.println("Read Bank Mode");
                        if (bitRead(current_patch, button_actionned)) {
                            unselect_all();
                        } else {
                            select_bank_patch(button_actionned);
                        }
                        break;
                    }
                    case effects_mode::edit: {
                        Serial.println("Read Edit Mode");
                        read_edit_mode(button_actionned);
                        break;
                    }
                }
            }
        } else if (action == button_state::long_pressed && mode == effects_mode::bank) {
            if (button_actionned != NEXT_BANK_BUTTON && button_actionned != PREV_BANK_BUTTON) {
                Serial.println("Now in EDIT MODE");
                lcd->setCursor(0,2);
                lcd->print(String("Edit Patch ") + bank_manager->get_current_bank()->get_selected_patch_number() + 1);
                mode = effects_mode::edit;
            }
        }
    }
    delay(READING_RATE);
}

void EffectSwitcher::read_basic_mode(int button_actionned) {
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

void EffectSwitcher::read_edit_mode(int button_actionned) {
    if (bitRead(current_effects, button_actionned)) {
        Serial.println(String("Edit Mode: Deactivate ") + button_actionned);
        bitWrite(current_effects, button_actionned, LOW);
    } else {
        Serial.println(String("Edit Mode: Activate ") + button_actionned);
        bitWrite(current_effects, button_actionned, HIGH);
    }
    select_patch_and_effect(current_patch, current_effects);
}

void EffectSwitcher::toggle_boost() {
    if (bitRead(current_effects, BOOST_PATCH_POSITION)) {
        Serial.println(String("Deactivate Boost "));
        bitWrite(current_effects, BOOST_PATCH_POSITION, LOW);
    } else {
        Serial.println(String("Activate Boost"));
        bitWrite(current_effects, BOOST_PATCH_POSITION, HIGH);
    }
    select_patch_and_effect(current_patch, current_effects);
}

void EffectSwitcher::save_patch() {
    lcd->setCursor(0,2);
    lcd->print("Saving Preset...");
    Serial.println("Save Effects");
    bank_manager->get_current_bank()->get_selected_patch()->save(current_effects);
    delay(500);
    cancel_edit();
}

void EffectSwitcher::cancel_edit() {
    Serial.println("Cancel Edit");
    mode = effects_mode::bank;
    select_bank_patch(bank_manager->get_current_bank()->get_selected_patch_number());
    lcd->clear();
    lcd->setCursor(0,1);
    lcd->print(String("Bank ") + bank_manager->get_current_bank_number());
}

void EffectSwitcher::unselect_all() {
    Serial.println(String("Unselect All"));
    select_patch_and_effect(0, 0);
}

void EffectSwitcher::select_bank_patch(byte patch_number) {
    unselect_all();
    byte effects = bank_manager->get_current_bank()->select_patch(patch_number)->get_selected_effects();
    byte patch_activated = 0;
    bitWrite(patch_activated, patch_number, HIGH);
    Serial.println(String("Select effects: ") + effects);
    select_patch_and_effect(patch_activated, effects);
}

void EffectSwitcher::select_patch_and_effect(byte patch, byte effects) {
    // turn off the output so the pins don't light up
    // while you're shifting bits:
    digitalWrite(pin_register_latch, LOW);
    digitalWrite(pin_register_output_enable, HIGH);
    
    // shift the bits for patch
    shiftOut(pin_register_data, pin_register_clock, MSBFIRST, patch);
    
    // shift the bits for effects
    shiftOut(pin_register_data, pin_register_clock, MSBFIRST, effects);

    // turn on the output so the LEDs can light up:
    digitalWrite(pin_register_latch, HIGH);
    digitalWrite(pin_register_output_enable, LOW);
    current_patch = patch;
    current_effects = effects;
    delay(10);
}
