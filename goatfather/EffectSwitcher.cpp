#include "EffectSwitcher.h"
#include "Constants.h"


EffectSwitcher::EffectSwitcher() {
    bank_manager = new BankManager(0, NUMBER_OF_BANKS, PATCHES_PER_BANK);
    button_reader = new ButtonReader();
    lcd = new LiquidCrystal_I2C(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);
    mode = BASIC_MODE;
    current_patch = 0;
    current_effects = 0;
}

void EffectSwitcher::init() {
    lcd->init();
    lcd->backlight();
    lcd->setCursor(0,0);
    lcd->print("I AM THE GOATFATHER");
    lcd->setCursor(0,1);
    lcd->print("Starting");

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

    // Enable output
    digitalWrite(OUTPUT_ENABLE_PIN, LOW);
    // Disable inputs
    digitalWrite(IN_LOAD_PIN, HIGH);
    digitalWrite(IN_CLOCK_ENABLE_PIN, HIGH);

    Serial.begin(SERIAL_RATE);
    
    // initialize bank manager
    bank_manager->init(LOAD_PREDEFINED);

    Serial.println("GoatFather initialized");

    lcd->clear();
    lcd->setCursor(0,1);
    lcd->print("Effect Mode");

    unselect_all();
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
        if (action == BUTTON_STATE_PRESSED) {
            if (button_actionned == PREV_BANK_BUTTON) {
                if (mode == EDIT_MODE) {
                    save_patch();
                } else {
                    bank_manager->previous();
                    Serial.println(String("Previous Bank: ") + bank_manager->get_current_bank_number());
                    if (bank_manager->get_current_bank_number() == BASIC_MODE_BANK) {
                        mode = BASIC_MODE;
                        lcd->clear();
                        lcd->setCursor(0,1);
                        lcd->print("Effect Mode");
                    } else {
                        mode = BANK_MODE;
                        lcd->clear();
                        lcd->setCursor(0,1);
                        lcd->print(String("Bank ") + bank_manager->get_current_bank_number());
                    }
                    unselect_all();
                }
            } else if (button_actionned == NEXT_BANK_BUTTON) {
                if (mode == EDIT_MODE) {
                    cancel_edit();
                } else {
                    bank_manager->next();
                    Serial.println(String("Next Bank: ") + bank_manager->get_current_bank_number());
                    if (bank_manager->get_current_bank_number() == BASIC_MODE_BANK) {
                        mode = BASIC_MODE;
                        lcd->clear();
                        lcd->setCursor(0,1);
                        lcd->print("Effect Mode");
                    } else {
                        mode = BANK_MODE;
                        lcd->clear();
                        lcd->setCursor(0,1);
                        lcd->print(String("Bank ") + bank_manager->get_current_bank_number());
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
                lcd->setCursor(0,2);
                lcd->print(String("Edit Patch ") + bank_manager->get_current_bank()->get_selected_patch_number() + 1);
                mode = EDIT_MODE;
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
    mode = BANK_MODE;
    select_bank_patch(bank_manager->get_current_bank()->get_selected_patch_number());
    lcd->clear();
    lcd->setCursor(0,1);
    lcd->print(String("Bank ") + bank_manager->get_current_bank_number());
}

void EffectSwitcher::select_patch_and_effect(byte patch, byte effects) {
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
