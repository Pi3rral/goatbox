#include "RollerEffectSwitcher.h"
#include "ButtonReader.h"


RollerEffectSwitcher::RollerEffectSwitcher(
    ButtonReader* _button_reader, 
    BankManager* _bank_manager,
    byte _pin_register_clock,
    byte _pin_register_latch,
    byte _pin_register_output_enable,
    byte _pin_register_data,
    byte _edit_led_pin
): EffectSwitcher(_button_reader, _bank_manager, nullptr,
    _pin_register_clock, _pin_register_latch, _pin_register_output_enable, _pin_register_data) {
    edit_mode_started = 0;
    edit_led_pin = _edit_led_pin;
}

void RollerEffectSwitcher::init() {
    pinMode(edit_led_pin, OUTPUT);
    digitalWrite(edit_led_pin, HIGH);
    EffectSwitcher::init();
    digitalWrite(edit_led_pin, LOW);
}

void RollerEffectSwitcher::read_and_apply() {
    bool action_done = false;
    button_reader->read();
    if (mode == effects_mode::edit && millis() - edit_mode_started > EDIT_MODE_INTERVAL) {
        save_patch();
        digitalWrite(edit_led_pin, LOW);
    }
    if (button_reader->is_multiple_button_pressed() && mode != effects_mode::edit) {
        if (is_next_bank_pressed()) {
            bank_manager->next();
            Serial.println(String("Next Bank: ") + bank_manager->get_current_bank_number());
            set_bank_mode();
            unselect_all();
            action_done = true;
        } else if (is_previous_bank_pressed()) {
            bank_manager->previous();
            Serial.println(String("Previous Bank: ") + bank_manager->get_current_bank_number());
            set_bank_mode();
            unselect_all();
            action_done = true;
        } else if (is_no_bank_pressed()) {
            Serial.println(String("No Bank"));
            bank_manager->set_current_bank(BASIC_MODE_BANK);
            set_bank_mode();
            unselect_all();
            action_done = true;
        }
    }
    int button_actioned = button_reader->get_actioned_button();
    if (button_actioned >= 0) {
        int action = button_reader->get_action_for_button(button_actioned);
        if (action == button_state::pressed) {
            switch(mode) {
                case effects_mode::basic: {
                    Serial.println("Read Basic Mode");
                    read_basic_mode(button_actioned);
                    break;            
                }
                case effects_mode::bank: {
                    Serial.println("Read Bank Mode");
                    if (bitRead(current_patch, button_actioned)) {
                        unselect_all();
                    } else {
                        select_bank_patch(button_actioned);
                    }
                    break;
                }
                case effects_mode::edit: {
                    Serial.println("Read Edit Mode");
                    read_edit_mode(button_actioned);
                    break;
                }
            }
        } else if (action == button_state::long_pressed && mode == effects_mode::bank) {
            Serial.println("Now in EDIT MODE");
            display(String("Edit Patch ") + bank_manager->get_current_bank()->get_selected_patch_number() + 1, 2);
            select_bank_patch(button_actioned);
            mode = effects_mode::edit;
            digitalWrite(edit_led_pin, HIGH);
            edit_mode_started = millis();
            action_done = true;
//        } else if (action == button_state::double_pressed && mode != effects_mode::edit) {
//            switch(button_actioned) {
//                case ROLLER_PREV_BANK_BUTTON: {
//                    bank_manager->previous();
//                    Serial.println(String("Previous Bank: ") + bank_manager->get_current_bank_number());
//                    set_bank_mode();
//                    unselect_all();
//                    break;
//                }
//                case ROLLER_NEXT_BANK_BUTTON: {
//                    bank_manager->next();
//                    Serial.println(String("Next Bank: ") + bank_manager->get_current_bank_number());
//                    set_bank_mode();
//                    unselect_all();
//                    break;
//                }
//                case ROLLER_NO_BANK_BUTTON: {
//                    Serial.println(String("No Bank"));
//                    bank_manager->set_current_bank(BASIC_MODE_BANK);
//                    set_bank_mode();
//                    unselect_all();
//                    break;
//                }
//            }
//            action_done = true;
        }
    }
    if (action_done) {
        delay(READING_WAIT_AFTER_ACTION);
    }
    delay(READING_RATE);
}

void RollerEffectSwitcher::compute_registers(byte patch, byte effects) {
    // set bank leds display for TPIC6B595N
    int current_bank = bank_manager->get_current_bank_number();
    register_2 = 0;
    for (int i = 0 ; i < current_bank; ++i) {
        bitWrite(register_2, i, HIGH);
    }

    // Display LED for selected patch and effects
    register_1 = 0;
    for (int i = 0; i < bank_manager->get_patches_per_bank(); ++i) {
        if (patch > 0 && bitRead(patch, i)) {
            bitWrite(register_1, i + bank_manager->get_patches_per_bank(), HIGH);
        }
        if (bitRead(effects, i)) {
            bitWrite(register_1, i, HIGH);
        }
    }
}

bool RollerEffectSwitcher::is_next_bank_pressed() {
    if (button_reader->get_action_for_button(2) != button_state::rest
        && button_reader->get_action_for_button(3) != button_state::rest) {
        return true;
    }
    return false;
}

bool RollerEffectSwitcher::is_previous_bank_pressed() {
    if (button_reader->get_action_for_button(0) != button_state::rest
        && button_reader->get_action_for_button(1) != button_state::rest) {
        return true;
    }
    return false;
}

bool RollerEffectSwitcher::is_no_bank_pressed() {
    if (button_reader->get_action_for_button(1) != button_state::rest
        && button_reader->get_action_for_button(2) != button_state::rest) {
        return true;
    }
    return false;
}
