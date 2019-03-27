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
#define BOOST_AFTER       128


byte bank_type_1[7] = {
    (byte)(COMPRESSOR),
    (byte)(MODULATION_BEFORE),
    (byte)(BOOST_BEFORE + DISTORTION),
    (byte)(BOOST_BEFORE + DISTORTION + BOOST_AFTER),
    (byte)(MODULATION_BEFORE + BOOST_BEFORE + DISTORTION),
    (byte)(PITCH_WAH + BOOST_BEFORE + DISTORTION + BOOST_AFTER),
    (byte)(BOOST_AFTER)
};

struct BankDefinition whats_my_age_again = {
    "What's my age again Blink 182",
    bank_type_1
};
struct BankDefinition all_the_small_things = {
    "All The   Small     Things    Blink 182",
    bank_type_1
};
struct BankDefinition bullet_with_butterfly_wings = {
    "Bullet w/ Butterfly Wings     Smashing",
    bank_type_1
};
struct BankDefinition interstate_love_song = {
    "InterstateLove Song STP",
    bank_type_1
};
struct BankDefinition monkey_wrench = {
    "Monkey    Wrench    Foo Fighters",
    bank_type_1
};
struct BankDefinition everlong = {
    "Everlong  Foo Fighters",
    bank_type_1
};
struct BankDefinition basket_case = {
    "Basket    Case      Green Day",
    bank_type_1
};
struct BankDefinition santeria = {
    "Santeria  Sublime",
    bank_type_1
};
struct BankDefinition my_own_worst_enemy = {
    "My Own    Worst     Enemy     Lit",
    bank_type_1
};
struct BankDefinition resurrection = {
    "Resurrection        Moist",
    bank_type_1
};
struct BankDefinition lump = {
    "Lump      PresidentsOf The USA",
    bank_type_1
};
struct BankDefinition the_middle = {
    "The MiddleJimmy Eat World",
    bank_type_1
};
struct BankDefinition santa_monica = {
    "santa     Monica    Everclear",
    bank_type_1
};
struct BankDefinition kryptonite = {
    "Kryptonite3 Doors   Down",
    bank_type_1
};
struct BankDefinition gel = {
    "Gel       CollectiveSoul",
    bank_type_1
};
struct BankDefinition blurry = {
    "Blurry    Puddle Of Mudd",
    bank_type_1
};
// struct BankDefinition come_out_and_play = {
//     "Come Out  And Play  The Offspring",
//     {
//         (byte)(BOOST_BEFORE + DISTORTION),
//         (byte)(BOOST_BEFORE + DISTORTION + BOOST_AFTER),
//         (byte)(COMPRESSOR + BOOST_AFTER)
//     }
// };
// struct BankDefinition zombie = {
//     "Zombie    The Cranberries",
//     {
//         (byte)(BOOST_BEFORE + DISTORTION),
//         (byte)(MODULATION_BEFORE + BOOST_BEFORE + DISTORTION + BOOST_AFTER),
//         (byte)(DELAY),
//         (byte)(MODULATION_BEFORE + BOOST_BEFORE + DISTORTION + DELAY + BOOST_AFTER)
//     }
// };



// #define ADDITIONAL_BANKS_NUMBER 18
#define ADDITIONAL_BANKS_NUMBER 16

struct BankDefinition AlterNation_Banks[ADDITIONAL_BANKS_NUMBER] = {
    whats_my_age_again,
    all_the_small_things,
    bullet_with_butterfly_wings,
    interstate_love_song,
    monkey_wrench,
    everlong,
    basket_case,
    santeria,
    my_own_worst_enemy,
    resurrection,
    lump,
    the_middle,
    santa_monica,
    // come_out_and_play,
    kryptonite,
    gel,
    blurry,
    // zombie
};


#endif //ALTERNATION_H