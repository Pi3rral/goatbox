#ifndef ROLLEREFFECTSWITCHER_H
#define ROLLEREFFECTSWITCHER_H


#include "EffectSwitcher.h"


#define ROLLER_PREV_BANK_BUTTON 2
#define ROLLER_NEXT_BANK_BUTTON 3
#define ROLLER_NO_BANK_BUTTON 0

#define EDIT_MODE_INTERVAL 6000


class RollerEffectSwitcher : public EffectSwitcher {

protected:
    virtual void compute_registers(byte patch, byte effects);
    unsigned long edit_mode_started = 0;

public:
    RollerEffectSwitcher(
        ButtonReader* _button_reader, 
        BankManager* _bank_manager,
        byte _pin_register_clock,
        byte _pin_register_latch,
        byte _pin_register_output_enable,
        byte _pin_register_data);
    void read_and_apply();
};


#endif // ROLLEREFFECTSWITCHER_H
