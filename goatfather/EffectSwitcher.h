#ifndef EFFECTSWITCHER_H
#define EFFECTSWITCHER_H

// #include "Wire.h"
#include "LiquidCrystal_I2C.h" 
#include "BankManager.h"
#include "ButtonReader.h"
#include "Constants.h"


class EffectSwitcher {

private:
    BankManager * bank_manager;
    ButtonReader * button_reader;
    LiquidCrystal_I2C * lcd;

    byte mode = BASIC_MODE;
    byte current_patch = 0;
    byte current_effects = 0;

public:
    EffectSwitcher();
    void init();
    void read_and_apply();

private:
    void unselect_all();
    void read_basic_mode(int button_actionned);
    void read_edit_mode(int button_actionned);
    void toggle_boost();
    void save_patch();
    void cancel_edit();
    void select_patch_and_effect(byte patch, byte effects);
    void select_bank_patch(byte patch_number);
};

#endif // EFFECTSWITCHER_H
