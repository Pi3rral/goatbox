#include "BankManager.h"
#include <EEPROM.h>


BankManager::BankManager(int _start_eeprom_address, 
                         int _number_eeprom_banks, 
                         int _patches_per_bank) {
    current_bank = 0;
    current_patch = 0;
    number_additional_banks = 0;
    start_eeprom_address = _start_eeprom_address;
    number_eeprom_banks = _number_eeprom_banks;
    patches_per_bank = _patches_per_bank;
}

BankManager::~BankManager() { }

void BankManager::init(BankDefinition* _additional_banks) {
    additional_banks = _additional_banks;
    number_additional_banks = sizeof(additional_banks);
}

byte BankManager::next() {
    ++current_bank;
    if (current_bank >= number_eeprom_banks) {
        current_bank = 0;
    }
    current_patch = 0;
    return get_selected_effects();
}

byte BankManager::previous() {
    --current_bank;
    if (current_bank < 0) {
        current_bank = number_eeprom_banks - 1;
    }
    current_patch = 0;
    return get_selected_effects();
}

byte BankManager::get_selected_effects(byte patch_number) {
    current_patch = patch_number;
    return load_effects_from_eeprom();
}

byte BankManager::load_effects_from_eeprom() {
    Serial.println("Read from eeprom address: " + String(get_eeprom_address()));
    return EEPROM.read(get_eeprom_address());
}

byte BankManager::save_effect_selection(byte _selected_effects) {
    Serial.println("Write to eeprom address: " + String(get_eeprom_address()));
    EEPROM.write(get_eeprom_address(), _selected_effects);
    return load_effects_from_eeprom();
}

int BankManager::get_eeprom_address() {
    return start_eeprom_address + 
    (current_bank * patches_per_bank) + 
    current_patch;
}
