#ifndef GOATBOX_BANKMANAGER_H
#define GOATBOX_BANKMANAGER_H

#include "Bank.h"
#include "Constants.h"


class BankManager {

private:
    int total_banks;
    int patches_per_bank;
    int current_bank;
    int start_eeprom_address;
    Bank ** banks;

public:
    BankManager(int _start_eeprom_address = 0,
                int _total_banks = NUMBER_OF_BANKS, 
                int _patches_per_bank = PATCHES_PER_BANK);
    ~BankManager();

    void init(bool _load_predefined = LOAD_PREDEFINED);

    Bank * next();
    Bank * previous();

    Bank * get_current_bank() const;
    void set_current_bank(int _bank_number);

    int get_current_bank_number() const;
};

#endif //GOATBOX_BANKMANAGER_H
