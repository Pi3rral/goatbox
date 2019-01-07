#ifndef GOATROLLER_H
#define GOATROLLER_H

#define SERIAL_RATE 115200

#define READING_RATE 100

#define NUMBER_OF_BANKS 8
#define PATCHES_PER_BANK 7
#define NUMBER_OF_EFFECTS 8
#define NUMBER_OF_BUTTON 10
#define NUMBER_OF_SHIFT_REGISTER 2
#define LONG_PRESS_TIME 2000
#define LOAD_PREDEFINED false

// TPIC6B595N Pins
#define CLOCK_PIN 9 //Connected to clock pin (SRCK) of TPIC6B595N
#define LATCH_PIN 8 //Connected to latch pin (RCK) of TPIC6B595N
#define OUTPUT_ENABLE_PIN 10 //Connected to output enable (G_) of TPIC6B595N
#define DATA_PIN 11 //Connected to Data in (SER_IN) of TPIC6B595N

// 74HC165 Pins
#define IN_LOAD_PIN 4 // Connected to load pin (PL) of 74HC165
#define IN_CLOCK_PIN 5 // Connected clock pin (CP) of 74HC165
#define IN_CLOCK_ENABLE_PIN 6 // Connected to clock enable pin (CE) of 74HC165
#define IN_DATA_PIN 7 // Connected to data pin (Q7) of 74HC165

// Button States
#define BUTTON_STATE_REST 0
#define BUTTON_STATE_PRESSED 1
#define BUTTON_STATE_STILL_PRESSED 2
#define BUTTON_STATE_LONG_PRESSED 3
#define BUTTON_STATE_STILL_LONG_PRESSED 4
#define BUTTON_STATE_RELEASED 5

#define BASIC_MODE 1
#define BANK_MODE 2
#define EDIT_MODE 3

#define BASIC_MODE_BANK 0

#define PREV_BANK_BUTTON 8
#define NEXT_BANK_BUTTON 9

#define BOOST_BUTTON 7
#define BOOST_PATCH_POSITION 7 

#endif // GOATROLLER_H