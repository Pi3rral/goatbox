#ifndef ALTERNATION_H
#define ALTERNATION_H

#include "BankManager.h"


#define COMPRESSOR        1
#define PITCH_WAH         2
#define MODULATION_BEFORE 4
#define BOOST_BEFORE      8
#define DISTORTION        16
#define MODULATION_AFTER  32
#define DELAY             64
#define BOOSTER_AFTER     128

struct BankDefinition blink_182_whats_my_age_again = {
    "What's my age again-Blink 182",
    {
        (byte)(BOOST_BEFORE + DISTORTION),
        (byte)(MODULATION_BEFORE)
    }
};

struct BankDefinition AlterNation_Banks[1] = {
    blink_182_whats_my_age_again
};


#endif //ALTERNATION_H