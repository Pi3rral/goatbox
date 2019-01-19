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
    if (mode == effects_mode::edit && millis() - edit_mode_started > EDIT_MODE_INTERVAL) {
        save_patch();
        digitalWrite(edit_led_pin, LOW);
    }
    button_reader->read();
    int button_actionned = button_reader->get_actionned_button();
    if (button_actionned >= 0) {
        int action = button_reader->get_action_for_button(button_actionned);
        if (action == button_state::pressed) {
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
        } else if (action == button_state::long_pressed && mode == effects_mode::bank) {
            Serial.println("Now in EDIT MODE");
            display(String("Edit Patch ") + bank_manager->get_current_bank()->get_selected_patch_number() + 1, 2);
            mode = effects_mode::edit;
            digitalWrite(edit_led_pin, HIGH);
            edit_mode_started = millis();
        } else if (action == button_state::double_pressed && mode != effects_mode::edit) {
            switch(button_actionned) {
                case ROLLER_PREV_BANK_BUTTON: {
                    bank_manager->previous();
                    Serial.println(String("Previous Bank: ") + bank_manager->get_current_bank_number());
                    set_bank_mode();
                    unselect_all();
                    break;
                }
                case ROLLER_NEXT_BANK_BUTTON: {
                    bank_manager->next();
                    Serial.println(String("Next Bank: ") + bank_manager->get_current_bank_number());
                    set_bank_mode();
                    unselect_all();
                    break;
                }
                case ROLLER_NO_BANK_BUTTON: {
                    Serial.println(String("No Bank"));
                    bank_manager->set_current_bank(BASIC_MODE_BANK);
                    set_bank_mode();
                    unselect_all();
                    break;
                }
            }
        }
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
