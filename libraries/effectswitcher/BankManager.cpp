#include "BankManager.h"
#include <EEPROM.h>
#include "Debug.h"


BankManager::BankManager() {
    current_bank = 0;
    current_patch = 0;
    number_additional_banks = 0;
}

BankManager::~BankManager() { }

void BankManager::init(
    int _start_eeprom_address, 
    int _number_eeprom_banks, 
    int _patches_per_bank,
    struct BankDefinition* _additional_banks, 
    int _add_banks_size
) {
    start_eeprom_address = _start_eeprom_address;
    number_eeprom_banks = _number_eeprom_banks;
    patches_per_bank = _patches_per_bank;
    additional_banks = _additional_banks;
    number_additional_banks = _add_banks_size;
    Debug::print(String("Additionnal banks: ") + number_additional_banks);
}

byte BankManager::next() {
    ++current_bank;
    if (current_bank >= number_eeprom_banks + number_additional_banks) {
        current_bank = 0;
    }
    current_patch = 0;
    return get_selected_effects();
}

byte BankManager::previous() {
    --current_bank;
    if (current_bank < 0) {
        current_bank = number_eeprom_banks + number_additional_banks - 1;
    }
    current_patch = 0;
    return get_selected_effects();
}

byte BankManager::get_selected_effects(byte patch_number) {
    current_patch = patch_number;
    if (current_bank >= number_eeprom_banks) {
        return additional_banks[current_bank-number_eeprom_banks].patches[current_patch];
    } else {
        return load_effects_from_eeprom();
    }
}

byte BankManager::load_effects_from_eeprom() {
    Debug::print("Read from eeprom address: " + String(get_eeprom_address()));
    return EEPROM.read(get_eeprom_address());
}

byte BankManager::save_effect_selection(byte _selected_effects) {
    Debug::print("Write to eeprom address: " + String(get_eeprom_address()));
    EEPROM.write(get_eeprom_address(), _selected_effects);
    return load_effects_from_eeprom();
}

int BankManager::get_eeprom_address() {
    return start_eeprom_address + 
    (current_bank * patches_per_bank) + 
    current_patch;
}

String BankManager::get_current_bank_name() const {
    if (current_bank >= number_eeprom_banks) {
        return additional_banks[current_bank-number_eeprom_banks].name;
    } 
    return "";
}
