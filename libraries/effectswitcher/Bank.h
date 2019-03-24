#ifndef GOATBOX_BANK_H
#define GOATBOX_BANK_H

#include "Patch.h"


class Bank {

private:
    bool can_edit;
    int eeprom_address;
    int bank_number;
    int total_patches;
    int selected_patch;
    String bank_name;
    Patch** patches;

    void load();

public:
    Bank(String _bank_name, int _total_patches, Patch** _patches);
    Bank(int _bank_number, int _eeprom_address, int _total_patches);
    ~Bank();

    Patch * select_patch(int _patch_number);
    Patch * get_selected_patch() const;
    int get_selected_patch_number() const { return selected_patch; };
    String get_bank_name() const { return bank_name; };
};

#endif //GOATBOX_BANK_H
