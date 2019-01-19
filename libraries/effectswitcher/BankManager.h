#ifndef GOATBOX_BANKMANAGER_H
#define GOATBOX_BANKMANAGER_H

#include "Bank.h"

#define DEFAULT_START_EEPROM_ADDRESS 0
#define DEFAULT_NUMBER_OF_BANKS 8
#define DEFAULT_PATCHES_PER_BANK 8
#define DEFAULT_LOAD_PREDEFINED false


class BankManager {

private:
    int total_banks;
    int patches_per_bank;
    int current_bank;
    int start_eeprom_address;
    Bank** banks;

public:
    BankManager(int _start_eeprom_address = DEFAULT_START_EEPROM_ADDRESS,
                int _total_banks = DEFAULT_NUMBER_OF_BANKS, 
                int _patches_per_bank = DEFAULT_PATCHES_PER_BANK);
    ~BankManager();

    void init(bool _load_predefined = DEFAULT_LOAD_PREDEFINED);

    Bank * next();
    Bank * previous();

    Bank * get_current_bank() const;
    void set_current_bank(int _bank_number);

    int get_current_bank_number() const;

    int get_total_bank() const { return total_banks; };
    int get_patches_per_bank() const { return patches_per_bank; };
};

#endif //GOATBOX_BANKMANAGER_H
