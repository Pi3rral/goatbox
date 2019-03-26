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
#define BOOST_AFTER     128

struct BankDefinition blink_182_whats_my_age_again = {
    "What's my age again Blink 182",
    {
        (byte)(BOOST_BEFORE + DISTORTION),
        (byte)(MODULATION_BEFORE),
        (byte)(BOOST_BEFORE)
    }
};
struct BankDefinition blink_182_all_the_small_things = {
    "All The   Small     Things    Blink 182",
    {
        (byte)(BOOST_BEFORE + DISTORTION),
        (byte)(MODULATION_BEFORE),
        (byte)(BOOST_BEFORE)
    }
};
struct BankDefinition smashing_pumpkins_bullet = {
    "Bullet w/ Butterfly Wings     Smashing",
    {
        (byte)(BOOST_BEFORE + DISTORTION),
        (byte)(MODULATION_BEFORE),
        (byte)(BOOST_BEFORE + DISTORTION + MODULATION_BEFORE),
        (byte)(BOOST_BEFORE + DISTORTION + MODULATION_BEFORE + BOOST_AFTER),
    }
};


#define ADDITIONAL_BANKS_NUMBER 3

struct BankDefinition AlterNation_Banks[ADDITIONAL_BANKS_NUMBER] = {
    blink_182_whats_my_age_again,
    blink_182_all_the_small_things,
    smashing_pumpkins_bullet
};


#endif //ALTERNATION_H