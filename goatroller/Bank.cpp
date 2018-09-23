#include "Bank.h"
#include "Patch.h"
#include "GoatRoller.h"


Bank::Bank(int _bank_number, int _eeprom_address, int _total_patches, bool _load_predefined) {
    bank_number = _bank_number;
    eeprom_address = _eeprom_address;
    total_patches = _total_patches;
    patches = new Patch*[total_patches];
    selected_patch = 0;
    load(_load_predefined);
}

Bank::~Bank() {
    for (int i = 0; i < total_patches; ++i) {
        delete patches[i];
    }
    delete patches;
}

void Bank::load(bool _load_predefined) {
    Serial.println(String("Loading Bank ") + bank_number);
    int selected_effects = 0;
    for (int i = 0; i < total_patches; ++i) {
        selected_effects = 0;
        if (_load_predefined) {
            selected_effects = predefined_patches[bank_number][i];
        }
        patches[i] = new Patch(i, eeprom_address + i, selected_effects, !_load_predefined);
    }
}

Patch * Bank::select_patch(int _patch_number) {
    selected_patch = _patch_number;
    return get_selected_patch();
}

Patch * Bank::get_selected_patch() const {
    return patches[selected_patch];
}
