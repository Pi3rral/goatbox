#ifndef ALTERNATION_H
#define ALTERNATION_H

#include "Bank.h"
#include "Patch.h"

#define COMPRESSOR        1
#define PITCH_WAH         2
#define MODULATION_BEFORE 4
#define BOOST_BEFORE      8
#define DISTORTION        16
#define MODULATION_AFTER  32
#define DELAY             64
#define BOOSTER_AFTER     128

Patch* blink_182_whats_my_age_again_patches[2] = {
    new Patch(
        0,
        0,
        BOOST_BEFORE +
        DISTORTION,
        false,
        false
    ),
    new Patch(
        0,
        0,
        MODULATION_BEFORE,
        false,
        false
    ),
};
Bank* blink_182_whats_my_age_again = new Bank(
    "What's my age again-Blink 182",
    2,
    blink_182_whats_my_age_again_patches
);


Bank* AlterNation_Banks[1] = {
    blink_182_whats_my_age_again
};


#endif //ALTERNATION_H