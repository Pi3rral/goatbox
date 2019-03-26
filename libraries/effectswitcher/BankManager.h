#ifndef GOATBOX_BANKMANAGER_H
#define GOATBOX_BANKMANAGER_H
#include <Arduino.h>

#define DEFAULT_START_EEPROM_ADDRESS 0
#define DEFAULT_NUMBER_OF_BANKS 8
#define DEFAULT_PATCHES_PER_BANK 8
#define MAX_BANK_NAME 40

struct BankDefinition {
    char name[MAX_BANK_NAME];
    byte patches[DEFAULT_PATCHES_PER_BANK];
};


class BankManager {

private:
    int number_eeprom_banks = DEFAULT_NUMBER_OF_BANKS;
    int patches_per_bank = DEFAULT_PATCHES_PER_BANK;
    int start_eeprom_address = DEFAULT_START_EEPROM_ADDRESS;
    int current_bank = 0;
    int current_patch = 0;
    BankDefinition* additional_banks = nullptr;
    int number_additional_banks = 0;

    int get_eeprom_address();
    byte load_effects_from_eeprom();

public:
    BankManager(int _start_eeprom_address = DEFAULT_START_EEPROM_ADDRESS,
                int _number_eeprom_banks = DEFAULT_NUMBER_OF_BANKS, 
                int _patches_per_bank = DEFAULT_PATCHES_PER_BANK);
    ~BankManager();

    void init(struct BankDefinition* _additional_banks, int _add_banks_size = 0);

    byte next();
    byte previous();

    byte get_selected_effects(byte patch_number = 0);
    byte save_effect_selection(byte _selected_effects);

    int get_current_bank_number() const { return current_bank; };
    char * get_current_bank_name() const;
    int get_current_patch_number() const { return current_patch; };
};

#endif //GOATBOX_BANKMANAGER_H
