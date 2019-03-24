#ifndef GOATBOX_BANK_H
#define GOATBOX_BANK_H
#include "types.h"

class Bank {

private:
    bool can_edit;
    int eeprom_address;
    byte bank_number;
    byte total_patches;
    byte selected_patch;
    String bank_name;
    byte* patches;

    void load_from_eeprom();

public:
    Bank(String _bank_name, byte* _patches);
    Bank(int _bank_number, int _eeprom_address, int _total_patches);
    ~Bank();

    byte select_patch(int _patch_number);
    byte get_selected_patch() const;
    int get_selected_patch_number() const { return selected_patch; };
    String get_bank_name() const { return bank_name; };
    void save_patch(byte _selected_effects);
};

#endif //GOATBOX_BANK_H
