#include "EffectSwitcher.h"
#include "ButtonReader.h"

EffectSwitcher::EffectSwitcher(
    ButtonReader * _button_reader, 
    BankManager* _bank_manager, 
    OLED* _oled,
    byte _pin_register_clock,
    byte _pin_register_latch,
    byte _pin_register_output_enable,
    byte _pin_register_data) {
    bank_manager = _bank_manager;
    button_reader = _button_reader;
    oled = _oled;
    pin_register_clock = _pin_register_clock;
    pin_register_latch = _pin_register_latch;
    pin_register_output_enable = _pin_register_output_enable;
    pin_register_data = _pin_register_data;
    mode = effects_mode::basic;
    register_1 = 0;
    register_2 = 0;
    current_patch = 0;
    current_effects = 0;
}

void EffectSwitcher::init_oled() {
    if (oled) {
        oled->init();
        oled->clearDisplay();
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
    init_oled();
    display("Starting...", 0, 0, true);
    init_register();
    button_reader->init();
    bank_manager->init();
    unselect_all();
    display("Effect Mode", 0, 0, true);
    Serial.println("GoatFather initialized");
}

void EffectSwitcher::clear_display() {
    if (oled) {
        oled->clearDisplay();
    }
}

void EffectSwitcher::display(String message, int row, int col, bool clear) {
    if (oled) {
        if (clear) {
            clear_display();
        }
        oled->print(message, row, col);
    }
}

void EffectSwitcher::read_and_apply() {
    button_reader->read();
    int button_actionned = button_reader->get_actionned_button();
    if (button_actionned >= 0) {
        int action = button_reader->get_action_for_button(button_actionned);
        if (action == button_state::pressed) {
            if (button_actionned == PREV_BANK_BUTTON) {
                if (mode == effects_mode::edit) {
                    save_patch();
                } else {
                    Serial.println(String("Previous Bank: ") + bank_manager->get_current_bank_number());
                    bank_manager->previous();
                    set_bank_mode();
                    unselect_all();
                }
            } else if (button_actionned == NEXT_BANK_BUTTON) {
                if (mode == effects_mode::edit) {
                    cancel_edit();
                } else {
                    Serial.println(String("Next Bank: ") + bank_manager->get_current_bank_number());
                    bank_manager->next();
                    set_bank_mode();
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
                display(String("Edit Patch ") + bank_manager->get_current_bank()->get_selected_patch_number() + 1, 2);
                mode = effects_mode::edit;
            }
        }
    }
    delay(READING_RATE);
}

void EffectSwitcher::set_bank_mode() {
    if (bank_manager->get_current_bank_number() == BASIC_MODE_BANK) {
        mode = effects_mode::basic;
        Serial.println("Effect Mode");
        display("Effect Mode", 1, 0, true);
    } else {
        mode = effects_mode::bank;
        Serial.println(String("Bank Mode - Bank ") + bank_manager->get_current_bank_number());
        display(String("Bank ") + bank_manager->get_current_bank_number(), 1, 0, true);
    }
}

void EffectSwitcher::read_basic_mode(int button_actionned) {
    byte next_state = bitRead(current_patch, button_actionned) ? LOW : HIGH;
    bitWrite(current_patch, button_actionned, next_state);
    bitWrite(current_effects, button_actionned, next_state);
    select_patch_and_effect(current_patch, current_effects);
}

void EffectSwitcher::read_edit_mode(int button_actionned) {
    byte effect_state = bitRead(current_effects, button_actionned) ? LOW : HIGH;
    bitWrite(current_effects, button_actionned, effect_state);
    select_patch_and_effect(current_patch, current_effects);
}

void EffectSwitcher::toggle_boost() {
    byte boost_state = bitRead(current_effects, BOOST_PATCH_POSITION) ? LOW : HIGH;
    bitWrite(current_effects, BOOST_PATCH_POSITION, boost_state);
    select_patch_and_effect(current_patch, current_effects);
}

void EffectSwitcher::save_patch() {
    display("Saving...", 2);
    Serial.println("Save Effects");
    bank_manager->get_current_bank()->get_selected_patch()->save(current_effects);
    delay(500);
    cancel_edit();
}

void EffectSwitcher::cancel_edit() {
    Serial.println("Cancel Edit");
    mode = effects_mode::bank;
    select_bank_patch(bank_manager->get_current_bank()->get_selected_patch_number());
    display(String("Bank ") + bank_manager->get_current_bank_number(), 1, 0, true);
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

void EffectSwitcher::compute_registers(byte patch, byte effects) {
    register_1 = effects;
    register_2 = patch;
}

void EffectSwitcher::select_patch_and_effect(byte patch, byte effects) {
    compute_registers(patch, effects);
    // turn off the output so the pins don't light up
    // while you're shifting bits:
    digitalWrite(pin_register_latch, LOW);
    digitalWrite(pin_register_output_enable, HIGH);
    
    // shift the bits for patch
    shiftOut(pin_register_data, pin_register_clock, MSBFIRST, register_2);
    
    // shift the bits for effects
    shiftOut(pin_register_data, pin_register_clock, MSBFIRST, register_1);

    // turn on the output so the LEDs can light up:
    digitalWrite(pin_register_latch, HIGH);
    digitalWrite(pin_register_output_enable, LOW);
    current_patch = patch;
    current_effects = effects;
    delay(10);
}
