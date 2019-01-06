#include "Patch.h"
#include <EEPROM.h>


Patch::Patch(int _patch_number, int _eeprom_address, int _selected_effects, int _load_from_eeprom) {
    patch_number = _patch_number;
    eeprom_address = _eeprom_address;
    selected_effects = _selected_effects;
    Serial.println(String("Loading Patch ") + patch_number);
    Serial.println(String("  -> Effects: ") + selected_effects);
    if (_load_from_eeprom) {
        Serial.println(String("  -> Loading from EEPROM"));
        load();
    }
}

Patch::~Patch() {

}

void Patch::load() {
    selected_effects = EEPROM.read(eeprom_address);
}

void Patch::save(int _selected_effects) {
    selected_effects = _selected_effects;
    Serial.println(String("Save effects ") + selected_effects + String(" to address ") + eeprom_address);
    EEPROM.write(eeprom_address, selected_effects);
}

int Patch::get_selected_effects() const {
    return selected_effects;
}
