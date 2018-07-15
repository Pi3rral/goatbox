#ifndef GOATROLLER_H
#define GOATROLLER_H


#define NUMBER_OF_BANKS 7
#define PATCHES_PER_BANK 4
#define NUMBER_OF_EFFECTS 4
#define LONG_PRESS_TIME 2000
#define LOAD_PREDEFINED true

// TPIC6B595N Pins
#define CLOCK_PIN 2 //Connected to clock pin (SRCK) of TPIC6B595N
#define LATCH_PIN 3 //Connected to latch pin (RCK) of TPIC6B595N
#define OUTPUT_ENABLE_PIN 4 //Connected to output enable (G_) of TPIC6B595N
#define DATA_PIN 5 //Connected to Data in (SER_IN) of TPIC6B595N

// EDIT LED Pin
#define EDIT_LED_PIN 6

#define NUMBER_OF_BUTTON 4

// Buttons Pins
#define BUTTON0_PIN 7
#define BUTTON1_PIN 8
#define BUTTON2_PIN 9
#define BUTTON3_PIN 10

#define BASIC_MODE_BANK 0

#define CHANGE_BANK_BUTTON 0
#define RESET_BANK_BUTTON 1
#define PREV_BANK_BUTTON 2
#define NEXT_BANK_BUTTON 3

const int predefined_patches[NUMBER_OF_BANKS][PATCHES_PER_BANK] = {
    {1, 2, 4, 8},    // never used
    {2, 10, 1, 4},   // Disto / Disto+Booster / Phaser / Modulation
    {2, 10, 11, 4},  // Disto / Disto+Booster / Disto+Phaser+Booster / Modulation
    {2, 4, 6, 13},   // Disto / Modulation / Disto+Modulation / Disto+Modulation+Booster
    {2, 3, 11, 4},   // Disto / Disto+Phaser / Disto+Phaser+Booster / Modulation
    {2, 11, 14, 4},  // Disto / Disto+Phaser+Booster / Disto+Modulation+Booster / Modulation
    {1, 2, 4, 8}     // Phaser / Disto / Modulation / Booster
};


#endif // GOATROLLER_H