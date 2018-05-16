/**
 * Master Controller Code
 * Get the input from 8 button, display associated LED.
 * Send the number to the slave (with the relays).
 * Display activated output's LED on the controller.
 * Display infos on the LCD display
 */

/* 
 * How many shift register chips are daisy-chained.
 */
#define IN_NUMBER_OF_SHIFT_CHIPS   1
/* 
 * Width of data (how many ext lines).
 */
#define IN_DATA_WIDTH   8

/* Width of pulse to trigger the shift register to read and latch.
*/
#define IN_PULSE_WIDTH_USEC   5

/* Optional delay between shift register reads.
*/
#define IN_POLL_DELAY_MSEC   1

/* You will need to change the "int" to "long" If the
 * NUMBER_OF_SHIFT_CHIPS is higher than 2.
*/
#define BYTES_VAL_T unsigned int

// 74HC165 INPUTS
int inPLoadPin        = 4;  // Connects to Parallel load pin the 165
int inClockEnablePin  = 5;  // Connects to Clock Enable pin the 165
int inDataPin         = 6; // Connects to the Q7 pin the 165
int inClockPin        = 7; // Connects to the Clock pin the 165

BYTES_VAL_T pinValues;
BYTES_VAL_T oldPinValues;


// 74HC595 OUTPUTS
int latchPin = 1; // pin connected to ST_CP of 74HC595
int clockPin = 2; // pin connected to SH_CP of 74HC595
int dataPin  = 3; // pin connected to DS of 74HC595


/* This function is essentially a "shift-in" routine reading the
 * serial Data from the shift register chips and representing
 * the state of those pins in an unsigned integer (or long).
*/
BYTES_VAL_T shiftIn() {
    long bitVal;
    BYTES_VAL_T bytesVal = 0;

    // Trigger a parallel Load to latch the state of the data lines,
    digitalWrite(inClockEnablePin, HIGH);
    digitalWrite(inPLoadPin, LOW);
    delayMicroseconds(IN_PULSE_WIDTH_USEC);
    digitalWrite(inPLoadPin, HIGH);
    digitalWrite(inClockEnablePin, LOW);

    // Loop to read each bit value from the serial out line of the SN74HC165N.
    for(int i = 0; i < IN_DATA_WIDTH; i++)
    {
        bitVal = digitalRead(inDataPin);

        // Set the corresponding bit in bytesVal.
        bytesVal |= (bitVal << ((IN_DATA_WIDTH-1) - i));

        // Pulse the Clock (rising edge shifts the next bit).
        digitalWrite(inClockPin, HIGH);
        delayMicroseconds(IN_PULSE_WIDTH_USEC);
        digitalWrite(inClockPin, LOW);
    }

    return(bytesVal);
}


void shiftOut(BYTES_VAL_T dataOut) {
    // This shifts 8 bits out MSB first,
    //on the rising edge of the clock,
    //clock idles low

    //clear everything out just in case to
    //prepare shift register for bit shifting
    digitalWrite(dataPin, 0);
    digitalWrite(clockPin, 0);
    digitalWrite(latchPin, 0);

    //for each bit in the byte dataOut�
    //NOTICE THAT WE ARE COUNTING DOWN in our for loop
    //This means that %00000001 or "1" will go through such
    //that it will be pin Q0 that lights.
    for (int i = 7; i >= 0; i--)  {
        digitalWrite(clockPin, 0);

        //if the value passed to dataOut and a bitmask result
        // true then... so if we are at i=6 and our value is
        // %11010100 it would the code compares it to %01000000
        // and proceeds to set pinState to 1.
        if ( dataOut & (1<<i) ) {
            digitalWrite(dataPin, 1);
        }
        else {
            digitalWrite(dataPin, 0);
        }

        //Sets the pin to HIGH or LOW depending on pinState
        digitalWrite(dataPin, pinState);
        //register shifts bits on upstroke of clock pin
        digitalWrite(clockPin, 1);
        //zero the data pin after shift to prevent bleed through
        digitalWrite(dataPin, 0);
    }

    //stop shifting
    digitalWrite(clockPin, 0);
    digitalWrite(latchPin, 1);
}

/* 
 * Dump the list of inputs along with their current status.
 */
void display_pin_values() {
    Serial.print("Pin States:\r\n");

    for(int i = 0; i < IN_DATA_WIDTH; i++)
    {
        Serial.print("- Pin-");
        Serial.print(i);
        Serial.print(": ");

        if((pinValues >> i) & 1)
            Serial.print("HIGH");
        else
            Serial.print("LOW");

        Serial.print("\r\n");
    }

    Serial.print("\r\n");
}


void apply_pin_values() {
    display_pin_values();
}


void setupShiftIn() {
    // Initialize our digital pins...
    pinMode(inPLoadPin, OUTPUT);
    pinMode(inClockEnablePin, OUTPUT);
    pinMode(inClockPin, OUTPUT);
    pinMode(inDataPin, INPUT);

    digitalWrite(inClockPin, LOW);
    digitalWrite(inPLoadPin, HIGH);

    // Read in and display the pin states at startup.
    pinValues = read_shift_regs();
    display_pin_values();
    oldPinValues = pinValues;
}

void setupShiftOut() {
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
}
 

void setup() {
    Serial.begin(9600);

    setupShiftIn();
    setupShiftOut();
}

void loop() {
    //Read the state of all zones.
    pinValues = shiftIn();

    // If there was a chage in state, apply them
    if(pinValues != oldPinValues) {
        Serial.print("*Pin value change detected*\r\n");
        apply_pin_values();
        oldPinValues = pinValues;
    }

    delay(IN_POLL_DELAY_MSEC);
}
