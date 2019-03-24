#include "Bank.h"
#include "Patch.h"


Bank::Bank(String _bank_name, int _total_patches, Patch** _patches) {
    bank_name = _bank_name;
    total_patches = _total_patches;
    patches = _patches;
    selected_patch = 0;
    can_edit = false;
}

Bank::Bank(int _bank_number, int _eeprom_address, int _total_patches) {
    bank_number = _bank_number;
    eeprom_address = _eeprom_address;
    total_patches = _total_patches;
    patches = new Patch*[total_patches];
    selected_patch = 0;
    bank_name = "";
    can_edit = true;
    load();
}

Bank::~Bank() {
    for (int i = 0; i < total_patches; ++i) {
        delete patches[i];
    }
    delete patches;
}

void Bank::load() {
    Serial.println(String("Loading Bank ") + bank_number);
    int selected_effects = 0;
    for (int i = 0; i < total_patches; ++i) {
        selected_effects = 0;
        patches[i] = new Patch(i, eeprom_address + i, selected_effects, true);
    }
}

Patch * Bank::select_patch(int _patch_number) {
    selected_patch = _patch_number;
    return get_selected_patch();
}

Patch * Bank::get_selected_patch() const {
    return patches[selected_patch];
}
