#include "RollerEffectSwitcher.h"
#include "ButtonReader.h"
#include "BankManager.h"
#include <SoftwareSerial.h>
#include <MIDI.h>

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
#define CLOCK_PIN 2         //Connected to clock pin (SRCK) of TPIC6B595N
#define LATCH_PIN 3         //Connected to latch pin (RCK) of TPIC6B595N
#define OUTPUT_ENABLE_PIN 4 //Connected to output enable (G_) of TPIC6B595N
#define DATA_PIN 5          //Connected to Data in (SER_IN) of TPIC6B595N

#define EDIT_LED_PIN 6

int buttons[NUMBER_OF_BUTTON] = {BUTTON0_PIN, BUTTON1_PIN, BUTTON2_PIN, BUTTON3_PIN};
ButtonReader *button_reader = new ButtonReaderPins(
    NUMBER_OF_BUTTON,
    buttons);
BankManager *bank_manager = new BankManager(EEPROM_START_ADDRESS, NUMBER_OF_BANKS, PATCHES_PER_BANK);

RollerEffectSwitcher effect_switcher(
    button_reader,
    bank_manager,
    CLOCK_PIN,
    LATCH_PIN,
    OUTPUT_ENABLE_PIN,
    DATA_PIN,
    EDIT_LED_PIN);

SoftwareSerial midiSerial(11, 12); //RX, TX

MIDI_CREATE_INSTANCE(SoftwareSerial, midiSerial, midiSoft);

void setup()
{
    effect_switcher.init();
    midiSoft.begin(); // Launch MIDI, by default listening to channel 1.
    midiSoft.setInputChannel(1);
}

void checkMIDI()
{
    if (midiSoft.read()) // Is there a MIDI message incoming ?
    {
        switch (midiSoft.getType()) // Get the type of the message we caught
        {
        case midi::ProgramChange: // If it is a Program Change,
            Serial.println(String("Midi PC Message ") + midiSoft.getData1());
            break;
        // See the online reference for other message types
        default:
            break;
        }
    }
}

void loop()
{
    checkMIDI();
    effect_switcher.read_and_apply();
}
