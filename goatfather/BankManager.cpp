#include "BankManager.h"
#include "Bank.h"
#include "Constants.h"


BankManager::BankManager(int _start_eeprom_address, int _total_banks, int _patches_per_bank) {
    current_bank = 0;
    start_eeprom_address = _start_eeprom_address;
    total_banks = _total_banks;
    patches_per_bank = _patches_per_bank;
}

BankManager::~BankManager() {
    for (int i = 0; i < total_banks; ++i) {
        delete banks[i];
    }
    delete banks;
}

void BankManager::init(bool _load_predefined) {
    banks = new Bank*[total_banks];
    for (int i = 0; i < total_banks; ++i) {
        banks[i] = new Bank(i, start_eeprom_address + (i * patches_per_bank), patches_per_bank, _load_predefined);
    }
}

Bank * BankManager::next() {
    ++current_bank;
    if (current_bank >= total_banks) {
        current_bank = 0;
    }
    return get_current_bank();
}

Bank * BankManager::previous() {
    --current_bank;
    if (current_bank < 0) {
        current_bank = total_banks - 1;
    }
    return get_current_bank();
}

Bank * BankManager::get_current_bank() const {
    return banks[current_bank];
}

void BankManager::set_current_bank(int _bank_number) {
    current_bank = _bank_number;
}

int BankManager::get_current_bank_number() const {
    return current_bank;
}
