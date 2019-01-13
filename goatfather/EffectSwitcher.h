#ifndef EFFECTSWITCHER_H
#define EFFECTSWITCHER_H

#include "LiquidCrystal_I2C.h"
#include "BankManager.h"
#include "ButtonReader.h"


enum effects_mode {
    basic = 1,
    bank = 2,
    edit = 3,
};


class EffectSwitcher {

protected:
    BankManager* bank_manager = 0;
    ButtonReader* button_reader = 0;
    LiquidCrystal_I2C* lcd = 0;

    byte pin_register_clock;
    byte pin_register_latch;
    byte pin_register_output_enable;
    byte pin_register_data;

    byte mode = effects_mode::basic;
    byte current_patch = 0;
    byte current_effects = 0;

    void unselect_all();
    virtual void read_basic_mode(int button_actionned);
    virtual void read_edit_mode(int button_actionned);
    void toggle_boost();
    void save_patch();
    void cancel_edit();
    virtual void select_patch_and_effect(byte patch, byte effects);
    void select_bank_patch(byte patch_number);
    void init_lcd();
    void init_register();
    void display(String message, int row = 0, int col = 0, bool clear = false);
    void clear_display();

public:
    EffectSwitcher(
        ButtonReader* _button_reader, 
        BankManager* _bank_manager, 
        LiquidCrystal_I2C* _lcd,
        byte _pin_register_clock,
        byte _pin_register_latch,
        byte _pin_register_output_enable,
        byte _pin_register_data);
    void init();
    void read_and_apply();

};

#endif // EFFECTSWITCHER_H
