#include "EffectSwitcher.h"
#include "ButtonReader.h"
#include "Debug.h"

EffectSwitcher::EffectSwitcher(
    byte _pin_register_clock,
    byte _pin_register_latch,
    byte _pin_register_output_enable,
    byte _pin_register_data,
    bool _with_oled) {
    pin_register_clock = _pin_register_clock;
    pin_register_latch = _pin_register_latch;
    pin_register_output_enable = _pin_register_output_enable;
    pin_register_data = _pin_register_data;
    with_oled = with_oled;
    mode = effects_mode::basic;
    register_1 = 0;
    register_2 = 0;
    current_patch = 0;
    current_effects = 0;
}

void EffectSwitcher::init_oled() {
    if (with_oled) {
        oled.init();
        oled.clearDisplay();
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

void EffectSwitcher::init(ButtonReader* _button_reader) {
    button_reader = _button_reader;
    display("Starting...", true);
    init_register();
    init_oled();
    unselect_all();
    displayBankNumber();
    Debug::print("GoatFather initialized");
}

void EffectSwitcher::init_bank_manager(
    int _start_eeprom_address,
    int _number_eeprom_banks, 
    int _patches_per_bank,
    struct BankDefinition* _additional_banks,
    int _add_banks_size
) {
    bank_manager.init(
        _start_eeprom_address,
        _number_eeprom_banks, 
        _patches_per_bank,
        _additional_banks,
        _add_banks_size
    );
}

void EffectSwitcher::display(String message, bool clear) {
    if (with_oled) {
        if (clear) {
            oled.clearDisplay();
        }
        oled.print(message.c_str());
    }
}

void EffectSwitcher::displayBankNumber() {
    if (with_oled) {
        oled.clearDisplay();
        if (bank_manager.get_current_bank_number() == BASIC_MODE_BANK) {
            oled.print("No Bank");
            Debug::print("No Bank");
        } else {
            if (!bank_manager.get_current_bank_name().equals("")) {
                oled.print(bank_manager.get_current_bank_name().c_str());
                Debug::print("Bank Name: " + String(bank_manager.get_current_bank_name()));
            } else {
                oled.printBankNumber(bank_manager.get_current_bank_number());
                Debug::print("Bank Number: " + String(bank_manager.get_current_bank_number()));
            }
        }
    }
}

void EffectSwitcher::displayEditMode() {
    if (with_oled) {
        oled.printEditMode();
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
                    bank_manager.previous();
                    // Debug::print(String("Previous Bank: ") + bank_manager.get_current_bank_number());
                    set_bank_mode();
                    unselect_all();
                }
            } else if (button_actionned == NEXT_BANK_BUTTON) {
                if (mode == effects_mode::edit) {
                    cancel_edit();
                } else {
                    bank_manager.next();
                    // Debug::print(String("Next Bank: ") + bank_manager.get_current_bank_number());
                    set_bank_mode();
                    unselect_all();
                }
            } else if (button_actionned == BOOST_BUTTON && mode != effects_mode::edit) {
                toggle_boost();
            } else {
                switch(mode) {
                    case effects_mode::basic: {
                        Debug::print("Read Basic Mode");
                        read_basic_mode(button_actionned);
                        break;            
                    }
                    case effects_mode::bank: {
                        Debug::print("Read Bank Mode");
                        if (bitRead(current_patch, button_actionned)) {
                            unselect_all();
                        } else {
                            select_bank_patch(button_actionned);
                        }
                        break;
                    }
                    case effects_mode::edit: {
                        Debug::print("Read Edit Mode");
                        read_edit_mode(button_actionned);
                        break;
                    }
                }
            }
        } else if (action == button_state::long_pressed && mode == effects_mode::bank) {
            if (button_actionned != NEXT_BANK_BUTTON && button_actionned != PREV_BANK_BUTTON) {
                Debug::print("Now in EDIT MODE");
                displayEditMode();
                mode = effects_mode::edit;
            }
        }
    }
    delay(READING_RATE);
}

void EffectSwitcher::set_bank_mode() {
    if (bank_manager.get_current_bank_number() == BASIC_MODE_BANK) {
        mode = effects_mode::basic;
        Debug::print("Effect Mode");
    } else {
        mode = effects_mode::bank;
        Debug::print(String("Bank Mode - Bank ") + bank_manager.get_current_bank_number());
    }
    displayBankNumber();
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
    display("Saving...", true);
    Debug::print("Save Effects");
    bank_manager.save_effect_selection(current_effects);
    delay(500);
    cancel_edit();
}

void EffectSwitcher::cancel_edit() {
    Debug::print("Cancel Edit");
    mode = effects_mode::bank;
    select_bank_patch(bank_manager.get_selected_effects());
    displayBankNumber();
}

void EffectSwitcher::unselect_all() {
    Debug::print(String("Unselect All"));
    select_patch_and_effect(0, 0);
}

void EffectSwitcher::select_bank_patch(byte patch_number) {
    unselect_all();
    byte effects = bank_manager.get_selected_effects(patch_number);
    byte patch_activated = 0;
    bitWrite(patch_activated, patch_number, HIGH);
    Debug::print(String("Select effects: ") + effects);
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
