#ifndef ROLLEREFFECTSWITCHER_H
#define ROLLEREFFECTSWITCHER_H


#include "EffectSwitcher.h"


#define ROLLER_PREV_BANK_BUTTON 3
#define ROLLER_NEXT_BANK_BUTTON 4
#define ROLLER_NO_BANK_BUTTON 1


class RollerEffectSwitcher : public EffectSwitcher {

protected:
    virtual void compute_registers(byte patch, byte effects);

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
