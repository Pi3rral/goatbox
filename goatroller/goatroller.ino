#include "RollerEffectSwitcher.h"
#include "ButtonReader.h"
#include "BankManager.h"


// ButtonReader parameters
#define NUMBER_OF_BUTTON 4
#define NUMBER_OF_SHIFT_REGISTER 2

#define BUTTON0_PIN 7
#define BUTTON1_PIN 8
#define BUTTON2_PIN 9
#define BUTTON3_PIN 10

// BankManager parameters
#define EEPROM_START_ADDRESS 0
#define NUMBER_OF_BANKS 7
#define PATCHES_PER_BANK 4

// EffectSwitcher parameters
#define CLOCK_PIN 9 //Connected to clock pin (SRCK) of TPIC6B595N
#define LATCH_PIN 8 //Connected to latch pin (RCK) of TPIC6B595N
#define OUTPUT_ENABLE_PIN 10 //Connected to output enable (G_) of TPIC6B595N
#define DATA_PIN 11 //Connected to Data in (SER_IN) of TPIC6B595N

#define EDIT_LED_PIN 6


// #define IN_LOAD_PIN 4 // Connected to load pin (PL) of 74HC165
// #define IN_CLOCK_PIN 5 // Connected clock pin (CP) of 74HC165
// #define IN_CLOCK_ENABLE_PIN 6 // Connected to clock enable pin (CE) of 74HC165
// #define IN_DATA_PIN 7 // Connected to data pin (Q7) of 74HC165
// ButtonReader* button_reader = new ButtonReaderRegister(
//     NUMBER_OF_BUTTON,
//     IN_LOAD_PIN,
//     IN_CLOCK_PIN,
//     IN_CLOCK_ENABLE_PIN,
//     IN_DATA_PIN,
//     NUMBER_OF_SHIFT_REGISTER
// );

byte * buttons = new byte[BUTTON0_PIN, BUTTON1_PIN, BUTTON2_PIN, BUTTON3_PIN];
ButtonReader* button_reader = new ButtonReaderPins(
    NUMBER_OF_BUTTON,
    buttons
);
BankManager* bank_manager = new BankManager(EEPROM_START_ADDRESS, NUMBER_OF_BANKS, PATCHES_PER_BANK);

RollerEffectSwitcher effect_switcher(
    button_reader, 
    bank_manager, 
    CLOCK_PIN,
    LATCH_PIN,
    OUTPUT_ENABLE_PIN,
    DATA_PIN,
    EDIT_LED_PIN
);

void setup() {
    effect_switcher.init();
}

void loop() {
    effect_switcher.read_and_apply();
}


// #include <Arduino.h>
// #include "GoatRoller.h"
// #include "BankManager.h"

// #define BASIC_MODE 1
// #define CHANGE_BANK_MODE 2
// #define BANK_MODE 3
// #define EDIT_MODE 4

// #define BUTTON_STATE_REST 0
// #define BUTTON_STATE_PRESSED 1
// #define BUTTON_STATE_STILL_PRESSED 2
// #define BUTTON_STATE_LONG_PRESSED 3
// #define BUTTON_STATE_RELEASED 4

// struct {
//     int button = 0;
//     bool pressed = false;
//     bool released = false;
//     bool still_pressed = false;
//     bool long_pressed = false;
//     bool still_long_pressed = false;
// } button_action;

// // typedef struct read_button_action ButtonPressed;

// // int to set mode;
// int mode = BASIC_MODE;
// // bool to indicate we are waiting for button release before doing any other action
// bool wait_for_release = false;

// // time for counting long press
// unsigned long time_button_action;

// int buttons[NUMBER_OF_BUTTON] = { BUTTON0_PIN, BUTTON1_PIN, BUTTON2_PIN, BUTTON3_PIN };
// int buttons_state[NUMBER_OF_BUTTON] = { BUTTON_STATE_REST, BUTTON_STATE_REST, BUTTON_STATE_REST, BUTTON_STATE_REST };
// unsigned long buttons_action_time[NUMBER_OF_BUTTON] = { 0, 0, 0, 0 };

// // bank Bank
// BankManager bank_manager(0, NUMBER_OF_BANKS, PATCHES_PER_BANK);

// // loop variables
// // ButtonPressed button_action;
// int current_bank = 0;
// int current_patch = 0;
// byte current_patch_and_effects = 0;
// byte bank_leds = 0;


// void setup() {
//     //set pins to output for registers and edit LED
//     pinMode(CLOCK_PIN, OUTPUT); 
//     pinMode(LATCH_PIN, OUTPUT);
//     pinMode(OUTPUT_ENABLE_PIN, OUTPUT);
//     pinMode(DATA_PIN, OUTPUT);  
//     pinMode(EDIT_LED_PIN, OUTPUT);

//     // set button pins to input
//     pinMode(BUTTON0_PIN, INPUT);
//     pinMode(BUTTON1_PIN, INPUT);
//     pinMode(BUTTON2_PIN, INPUT);
//     pinMode(BUTTON3_PIN, INPUT);

//     // turn on edit LED when initializing
//     digitalWrite(EDIT_LED_PIN, HIGH);

//     Serial.begin(57600);
    
//     // initialize bank manager
//     bank_manager.init(LOAD_PREDEFINED);

//     // Enable output
//     digitalWrite(OUTPUT_ENABLE_PIN, LOW);
//     // turn off edit LED
//     digitalWrite(EDIT_LED_PIN, LOW);

//     Serial.println("GoatRoller initialized");

//     unselect_all();
// }

// void loop() {
//     read_buttons_state();
//     if (wait_for_release && !button_action.long_pressed) {
//         if (button_action.released || !is_button_pressed()) {
//             Serial.println("No Button Pressed - remove wait_for_release");
//             wait_for_release = false;
//             delay(200);
//         }
//         return;
//     }
//     // Hook for activating long press mode all the time
//     if (button_action.long_pressed && button_action.button != CHANGE_BANK_BUTTON) {
//         Serial.println("Change Bank Shortcut");
//         read_change_bank_mode(true);
//         return;
//     }
//     switch(mode) {
//         case BASIC_MODE: {
//             read_basic_mode();
//             break;            
//         }
//         case BANK_MODE: {
//             read_bank_mode();
//             break;
//         }
//         case CHANGE_BANK_MODE: {
//             if (!button_action.pressed) {
//                 return;
//             }
//             read_change_bank_mode(false);
//             break;
//         }
//         case EDIT_MODE: {
//             read_edit_mode();
//             break;
//         }
//     }
// }

// void read_basic_mode() {
//     if (button_action.long_pressed && button_action.button == CHANGE_BANK_BUTTON) {
//         set_change_bank_mode_on();
//     } else if (button_action.pressed) {
//         if (bitRead(current_patch_and_effects, button_action.button)) {
//             Serial.println(String("Basic Mode: Deactivate ") + button_action.button);
//             bitWrite(current_patch_and_effects, button_action.button, LOW);
//             bitWrite(current_patch_and_effects, button_action.button + NUMBER_OF_EFFECTS, LOW);
//         } else {
//             Serial.println(String("Basic Mode: Activate ") + button_action.button);
//             bitWrite(current_patch_and_effects, button_action.button, HIGH);
//             bitWrite(current_patch_and_effects, button_action.button + NUMBER_OF_EFFECTS, HIGH);
//         }
//         select_patch_and_effect(current_patch_and_effects);
//         Serial.println("wait_for_release - read_basic_mode");
//         wait_for_release = true;
//     }
// }

// void read_change_bank_mode(bool shortcut) {
//     switch(button_action.button) {
//         case RESET_BANK_BUTTON: {
//             bank_manager.set_current_bank(BASIC_MODE_BANK);
//             current_bank = BASIC_MODE_BANK;
//         }
//         case CHANGE_BANK_BUTTON: {
//             activate_selected_bank();
//             break;
//         }
//         case PREV_BANK_BUTTON: {
//             bank_manager.previous();
//             current_bank = bank_manager.get_current_bank_number();
//             Serial.println(String("Change Bank Previous to ") + current_bank);
//             if (shortcut) {
//                 activate_selected_bank();
//             } else {
//                 Serial.println("wait_for_release - read_change_bank_mode - previous");
//                 wait_for_release = true;
//             }
//             break;
//         }
//         case NEXT_BANK_BUTTON: {
//             bank_manager.next();
//             current_bank = bank_manager.get_current_bank_number();
//             Serial.println(String("Change Bank Next to ") + current_bank);
//             if (shortcut) {
//                 activate_selected_bank();
//             } else {
//                 Serial.println("wait_for_release - read_change_bank_mode - next");
//                 wait_for_release = true;
//             }
//             break;
//         }
//     }
//     // set bank leds display for TPIC6B595N
//     bank_leds = 0;
//     for (int i = 0 ; i < current_bank; ++i) {
//         bitWrite(bank_leds, i, HIGH);
//     }
//     unselect_all();
// }

// void activate_selected_bank() {
//     Serial.println(String("Selected bank: ") + current_bank);
//     if (current_bank == BASIC_MODE_BANK) {
//         Serial.println("Set Basic Mode");
//         mode = BASIC_MODE;
//     } else {
//         Serial.println("Set Bank Mode");
//         mode = BANK_MODE;
//         current_patch = 0;
//     }
//     set_change_bank_mode_off();
// }

// void read_bank_mode() {
//     if (button_action.long_pressed && button_action.button == CHANGE_BANK_BUTTON) {
//         set_change_bank_mode_on();
//     } else if (button_action.pressed) {
//         Serial.println(String("Bank Mode Reading"));
//         if (button_action.button != current_patch) {
//             Serial.println(String("Select Patch: ") + button_action.button);
//             select_bank_patch(button_action.button);
//         } else {
//             unselect_all();
//         }
//         Serial.println("wait_for_release - read_bank_mode - previous");
//         wait_for_release = true;
//     }
// }

// void read_edit_mode() {

// }

// void set_change_bank_mode_on() {
//     Serial.println("Change Bank ON");
//     mode = CHANGE_BANK_MODE;
//     // turn on edit LED when changing bank
//     digitalWrite(EDIT_LED_PIN, HIGH);
//     Serial.println("wait_for_release - set_change_bank_mode_on");
//     wait_for_release = true;
// }

// void set_change_bank_mode_off() {
//     Serial.println("Change Bank OFF");
//     // turn off edit LED
//     digitalWrite(EDIT_LED_PIN, LOW);
//     Serial.println("wait_for_release - set_change_bank_mode_off");
//     wait_for_release = true;
// }

// void read_buttons_state() {
//     // read all button states
//     for (int i = 0; i < sizeof(buttons)/sizeof(int); ++i) {
//         if (digitalRead(buttons[i])) { // stop at first button with a pressed state
//             if ((button_action.still_pressed || button_action.still_long_pressed) 
//                 && button_action.button == i) { // if same button is still pressed
//                 button_action.pressed = false;
//                 button_action.released = false;
//                 button_action.long_pressed = false;
//                 if (millis() - time_button_action > LONG_PRESS_TIME && !button_action.still_long_pressed) { 
//                     // if pressed button is long pressed (more than 3s), switch to long pressed state
//                     // remove still_pressed state to not pass into this condition next time
//                     button_action.still_pressed = false;
//                     button_action.still_long_pressed = true;
//                     button_action.long_pressed = true;
//                     Serial.println(String("Button Long Pressed: ") + i);
//                     Serial.println("");
//                 }
//             } else if (button_action.button != i) { // if a different button is pressed, set it as pressed
//                 button_action.button = i;
//                 button_action.pressed = true;
//                 button_action.released = false;
//                 button_action.still_pressed = true;
//                 button_action.long_pressed = false;
//                 button_action.still_long_pressed = false;
//                 time_button_action = millis();
//                 Serial.println(String("Button Pressed: ") + i);
//                 Serial.println("");
//             }
//             return;
//         }
//     }
//     if (button_action.still_pressed || button_action.still_long_pressed) { // if button was in pressed state, set it to release
//         button_action.released = true;
//         button_action.pressed = false;
//         button_action.still_pressed = false;
//         button_action.long_pressed = false;
//         button_action.still_long_pressed = false;
//         Serial.println(String("Button Released: ") + button_action.button);
//         Serial.println("");
//     } else { // remove all states
//         button_action.button = -1;
//         button_action.released = false;
//         button_action.pressed = false;
//         button_action.still_pressed = false;
//         button_action.long_pressed = false;
//         button_action.still_long_pressed = false;
//         // Serial.println(String("Button Clear All Actions"));
//     }
// }

// bool is_button_pressed() {
//     return button_action.pressed || button_action.still_pressed || button_action.long_pressed;
// }

// void read_buttons_state_2() {
//     for (int i = 0; i < NUMBER_OF_BUTTON; ++i) {
//         if (digitalRead(buttons[i])) {
//             switch(buttons_state[i]) {
//                 case BUTTON_STATE_RELEASED:
//                 case BUTTON_STATE_REST: {
//                     buttons_state[i] = BUTTON_STATE_PRESSED;
//                     buttons_action_time[i] = millis();
//                     break;
//                 }
//                 case BUTTON_STATE_LONG_PRESSED:
//                 case BUTTON_STATE_PRESSED: {
//                     buttons_state[i] = BUTTON_STATE_STILL_PRESSED;
//                     break;
//                 }
//                 case BUTTON_STATE_STILL_PRESSED: {
//                     if (millis() - buttons_action_time[i] > LONG_PRESS_TIME) {
//                         buttons_state[i] = BUTTON_STATE_LONG_PRESSED;
//                     }
//                     break;
//                 }
//             }
//         } else {
//             switch(buttons_state[i]) {
//                 case BUTTON_STATE_LONG_PRESSED:
//                 case BUTTON_STATE_PRESSED: 
//                 case BUTTON_STATE_STILL_PRESSED: {
//                     buttons_state[i] = BUTTON_STATE_RELEASED;
//                     break;
//                 }
//                 default: {
//                     buttons_state[i] = BUTTON_STATE_REST;
//                     break;
//                 }
//             }
//         }
//     }
// }

// void select_patch_and_effect(byte patch_and_effects) {
//     // turn off the output so the pins don't light up
//     // while you're shifting bits:
//     digitalWrite(LATCH_PIN, LOW);
//     digitalWrite(OUTPUT_ENABLE_PIN, HIGH);
    
//     // shift the bits for selected bank
//     shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, bank_leds);
    
//     // shift the bits out
//     shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, patch_and_effects);

//     // turn on the output so the LEDs can light up:
//     digitalWrite(LATCH_PIN, HIGH);
//     digitalWrite(OUTPUT_ENABLE_PIN, LOW);
//     current_patch_and_effects = patch_and_effects;
//     delay(10);
// }

// void unselect_all() {
//     Serial.println(String("Unselect All"));
//     current_patch = -1;
//     select_patch_and_effect(0);
// }

// void select_bank_patch(int patch_number) {
//     current_patch = patch_number;
//     int selected_effects = bank_manager.get_current_bank()->select_patch(current_patch)->get_selected_effects();
//     Serial.println(String("Select effects: ") + int(selected_effects));

//     byte patch_and_effects = 0;

//     // Display LED for selected patch
//     bitWrite(patch_and_effects, current_patch + NUMBER_OF_EFFECTS, HIGH);
//     Serial.println(String("bitWrite: ") + int(current_patch + NUMBER_OF_EFFECTS));
//     for (int i = 0; i < PATCHES_PER_BANK; ++i) {
//         if (bitRead(selected_effects, i)) {
//             Serial.println(String("bitWrite: ") + int(i));
//             bitWrite(patch_and_effects, i, HIGH);
//         }
//     }

//     select_patch_and_effect(patch_and_effects);
// }
