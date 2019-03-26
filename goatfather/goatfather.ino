#include "EffectSwitcher.h"
#include "ButtonReader.h"
#include "BankManager.h"
#include "OLED.h"
#include "AlterNation.h"
#include "Debug.h"

// ButtonReader parameters
#define NUMBER_OF_BUTTON 10
#define NUMBER_OF_SHIFT_REGISTER 2
#define IN_LOAD_PIN 4 // Connected to load pin (PL) of 74HC165
#define IN_CLOCK_PIN 5 // Connected clock pin (CP) of 74HC165
#define IN_CLOCK_ENABLE_PIN 6 // Connected to clock enable pin (CE) of 74HC165
#define IN_DATA_PIN 7 // Connected to data pin (Q7) of 74HC165

// BankManager parameters
#define EEPROM_START_ADDRESS 0
#define NUMBER_OF_EEPROM_BANKS 10
#define PATCHES_PER_BANK 7

// EffectSwitcher parameters
#define CLOCK_PIN 9 //Connected to clock pin (SRCK) of TPIC6B595N
#define LATCH_PIN 8 //Connected to latch pin (RCK) of TPIC6B595N
#define OUTPUT_ENABLE_PIN 10 //Connected to output enable (G_) of TPIC6B595N
#define DATA_PIN 11 //Connected to Data in (SER_IN) of TPIC6B595N

EffectSwitcher effect_switcher(
    CLOCK_PIN,
    LATCH_PIN,
    OUTPUT_ENABLE_PIN,
    DATA_PIN
);

void setup() {
    Debug::init(false);

    ButtonReader* button_reader = new ButtonReaderRegister(
        NUMBER_OF_BUTTON,
        IN_LOAD_PIN,
        IN_CLOCK_PIN,
        IN_CLOCK_ENABLE_PIN,
        IN_DATA_PIN,
        NUMBER_OF_SHIFT_REGISTER
    );
    BankManager* bank_manager = new BankManager(EEPROM_START_ADDRESS, NUMBER_OF_EEPROM_BANKS, PATCHES_PER_BANK);
    OLED* oled = new OLED();

    button_reader->init();
    bank_manager->init(AlterNation_Banks, ADDITIONAL_BANKS_NUMBER);
    oled->init();

    effect_switcher.init(
        button_reader,
        bank_manager,
        oled
    );
}

void loop() {
    effect_switcher.read_and_apply();
}
