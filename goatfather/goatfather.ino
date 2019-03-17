// #include <Arduino.h>
#include "Wire.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "EffectSwitcher.h"
#include "ButtonReader.h"
#include "BankManager.h"


// ButtonReader parameters
#define NUMBER_OF_BUTTON 10
#define NUMBER_OF_SHIFT_REGISTER 2
#define IN_LOAD_PIN 4 // Connected to load pin (PL) of 74HC165
#define IN_CLOCK_PIN 5 // Connected clock pin (CP) of 74HC165
#define IN_CLOCK_ENABLE_PIN 6 // Connected to clock enable pin (CE) of 74HC165
#define IN_DATA_PIN 7 // Connected to data pin (Q7) of 74HC165

// BankManager parameters
#define EEPROM_START_ADDRESS 0
#define NUMBER_OF_BANKS 8
#define PATCHES_PER_BANK 7

// OLED parameters
#define OLED_ADDRESS 0x3F
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_RESET 4

// EffectSwitcher parameters
#define CLOCK_PIN 9 //Connected to clock pin (SRCK) of TPIC6B595N
#define LATCH_PIN 8 //Connected to latch pin (RCK) of TPIC6B595N
#define OUTPUT_ENABLE_PIN 10 //Connected to output enable (G_) of TPIC6B595N
#define DATA_PIN 11 //Connected to Data in (SER_IN) of TPIC6B595N


ButtonReader* button_reader = new ButtonReaderRegister(
    NUMBER_OF_BUTTON,
    IN_LOAD_PIN,
    IN_CLOCK_PIN,
    IN_CLOCK_ENABLE_PIN,
    IN_DATA_PIN,
    NUMBER_OF_SHIFT_REGISTER
);
BankManager* bank_manager = new BankManager(EEPROM_START_ADDRESS, NUMBER_OF_BANKS, PATCHES_PER_BANK);
Adafruit_SSD1306* lcd = new Adafruit_SSD1306(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);

EffectSwitcher effect_switcher(
    button_reader, 
    bank_manager, 
    lcd,
    CLOCK_PIN,
    LATCH_PIN,
    OUTPUT_ENABLE_PIN,
    DATA_PIN,
    OLED_ADDRESS
);

void setup() {
    effect_switcher.init();
}

void loop() {
    effect_switcher.read_and_apply();
}
