#ifndef EFFECTSWITCHER_H
#define EFFECTSWITCHER_H

#include "LiquidCrystal_I2C.h"
#include "BankManager.h"
#include "ButtonReader.h"

#define SERIAL_RATE 115200
#define READING_RATE 100
#define READING_WAIT_AFTER_ACTION 500
#define READING_WAIT_AFTER_2_PRESSED 1000
#define BASIC_MODE_BANK 0

#define PREV_BANK_BUTTON 8
#define NEXT_BANK_BUTTON 9

#define BOOST_BUTTON 7
#define BOOST_PATCH_POSITION 7

enum effects_mode
{
    basic = 1,
    bank = 2,
    edit = 3,
};

class EffectSwitcher
{

protected:
    BankManager *bank_manager = 0;
    ButtonReader *button_reader = 0;
    LiquidCrystal_I2C *lcd = 0;

    byte pin_register_clock;
    byte pin_register_latch;
    byte pin_register_output_enable;
    byte pin_register_data;

    byte mode = effects_mode::basic;
    byte current_patch = 0;
    byte current_effects = 0;

    byte register_1 = 0;
    byte register_2 = 0;

    void unselect_all();
    virtual void read_basic_mode(int button_actioned);
    virtual void read_edit_mode(int button_actioned);
    virtual void set_bank_mode();
    void toggle_boost();
    void save_patch();
    void cancel_edit();
    virtual void compute_registers(byte patch, byte effects);
    void select_bank_patch(byte patch_number);
    void init_lcd();
    void init_register();
    void display(String message, int row = 0, int col = 0, bool clear = false);
    void clear_display();

public:
    EffectSwitcher(
        ButtonReader *_button_reader,
        BankManager *_bank_manager,
        LiquidCrystal_I2C *_lcd,
        byte _pin_register_clock,
        byte _pin_register_latch,
        byte _pin_register_output_enable,
        byte _pin_register_data);
    virtual void init();
    virtual void read_and_apply();
    virtual void select_patch_and_effect(byte patch, byte effects);
    virtual void toggle_loop(byte loop_number);
    virtual void set_loop_state(byte loop_number, int loop_state);
};

#endif // EFFECTSWITCHER_H
