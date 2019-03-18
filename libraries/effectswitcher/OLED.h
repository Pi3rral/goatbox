#ifndef OLED_H
#define OLED_H

#include "Arduino.h"

class OLED {
public:
    OLED();
    void init();
    void clearDisplay();
    void print(String message, int row, int col);
    // void printBankNumber(int bank_number);
    // void printEditMode();
};

#endif // OLED_H
