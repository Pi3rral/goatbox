#include "Bank.h"
#include <EEPROM.h>


Bank::Bank(String _bank_name, byte* _patches) {
    bank_name = _bank_name;
    total_patches = sizeof(_patches);
    patches = _patches;
    selected_patch = 0;
    can_edit = false;
}

Bank::Bank(int _bank_number, int _eeprom_address, int _total_patches) {
    bank_number = _bank_number;
    eeprom_address = _eeprom_address;
    total_patches = _total_patches;
    patches = new byte[total_patches];
    selected_patch = 0;
    bank_name = "";
    can_edit = true;
    load_from_eeprom();
}

Bank::~Bank() {
    delete patches;
}

void Bank::load_from_eeprom() {
    Serial.println(String("Loading Bank ") + bank_number);
    int selected_effects = 0;
    for (int i = 0; i < total_patches; ++i) {
        selected_effects = 0;
        patches[i] = EEPROM.read(eeprom_address + i);
    }
}

void Bank::save_patch(byte _selected_effects) {
    if (can_edit) {
        EEPROM.write(eeprom_address + get_selected_patch_number(), _selected_effects);
        patches[get_selected_patch_number()] = _selected_effects;
    }
}

byte Bank::select_patch(int _patch_number) {
    selected_patch = _patch_number;
    return get_selected_patch();
}

byte Bank::get_selected_patch() const {
    return patches[selected_patch];
}
