#include "Patch.h"
#include <EEPROM.h>


Patch::Patch(int _patch_number, int _eeprom_address, int _selected_effects, int _load_from_eeprom) {
    patch_number = _patch_number;
    eeprom_address = _eeprom_address;
    selected_effects = _selected_effects;
    Serial.println(String("Loading Patch ") + patch_number);
    // Serial.println(String("  -> Effects: ") + selected_effects);
    if (_load_from_eeprom) {
        load();
    }
}

Patch::~Patch() {

}

void Patch::load() {

    Serial.println(String("  -> Loading from EEPROM address: ") + eeprom_address);
    selected_effects = EEPROM.read(eeprom_address);
    Serial.println(String("  -> Seletected effects: ") + selected_effects);
}

void Patch::save(byte _selected_effects) {
    selected_effects = _selected_effects;
    Serial.println(String("Save effects ") + selected_effects + String(" to address ") + eeprom_address);
    EEPROM.write(eeprom_address, selected_effects);
}

byte Patch::get_selected_effects() const {
    return selected_effects;
}
