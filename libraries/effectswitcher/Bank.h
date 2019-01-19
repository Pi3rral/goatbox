#ifndef GOATBOX_BANK_H
#define GOATBOX_BANK_H

#include "Patch.h"


class Bank {

private:
    int eeprom_address;
    int bank_number;
    int total_patches;
    int selected_patch;
    Patch** patches;

    void load(bool _load_predefined);

public:
    Bank(int _bank_number, int _eeprom_address, int _total_patches, bool _load_predefined);
    ~Bank();

    Patch * select_patch(int _patch_number);
    Patch * get_selected_patch() const;
    int get_selected_patch_number() const { return selected_patch; };
};

#endif //GOATBOX_BANK_H
