#include "EffectSwitcher.h"
#include "ButtonReader.h"
#include "BankManager.h"
// #include "OLED.h"
// #include "AlterNation.h"
#include "Debug.h"

// ButtonReader parameters
#define NUMBER_OF_BUTTON 10
#define NUMBER_OF_SHIFT_REGISTER 2
#define IN_LOAD_PIN 4 // Connected to load pin (PL) of 74HC165
#define IN_CLOCK_PIN 5 // Connected clock pin (CP) of 74HC165
#define IN_CLOCK_ENABLE_PIN 6 // Connected to clock enable pin (CE) of 74HC165
#define IN_DATA_PIN 7 // Connected to data pin (Q7) of 74HC165

// BankManager parameters
#define EEPROM_START_ADDRESS 0
#define NUMBER_OF_EEPROM_BANKS 10
#define PATCHES_PER_BANK 7

// EffectSwitcher parameters
#define CLOCK_PIN 9 //Connected to clock pin (SRCK) of TPIC6B595N
#define LATCH_PIN 8 //Connected to latch pin (RCK) of TPIC6B595N
#define OUTPUT_ENABLE_PIN 10 //Connected to output enable (G_) of TPIC6B595N
#define DATA_PIN 11 //Connected to Data in (SER_IN) of TPIC6B595N

EffectSwitcher effect_switcher(
    CLOCK_PIN,
    LATCH_PIN,
    OUTPUT_ENABLE_PIN,
    DATA_PIN,
    true
);

#define COMPRESSOR        1
#define PITCH_WAH         2
#define MODULATION_BEFORE 4
#define BOOST_BEFORE      8
#define DISTORTION        16
#define MODULATION_AFTER  32
#define DELAY             64
#define BOOST_AFTER       128

#define ADDITIONAL_BANKS_NUMBER 35

void setup() {
    Debug::init(false);

    byte bank_type_1[7] = {
        (byte)(COMPRESSOR),
        (byte)(MODULATION_BEFORE),
        (byte)(BOOST_BEFORE + DISTORTION),
        (byte)(BOOST_BEFORE + DISTORTION + BOOST_AFTER),
        (byte)(MODULATION_BEFORE + BOOST_BEFORE + DISTORTION),
        (byte)(PITCH_WAH + BOOST_BEFORE + DISTORTION + BOOST_AFTER),
        (byte)(BOOST_AFTER)
    };
    struct BankDefinition all_the_small_things = {
        "All The   Small     Things    Blink 182",
        bank_type_1
    };
    struct BankDefinition arey_you_gonna_be_my_girl = {
        "Are You   Gonna Be  My Girl   Jet",
        bank_type_1
    };
    struct BankDefinition basket_case = {
        "Basket    Case      Green Day",
        bank_type_1
    };
    struct BankDefinition blurry = {
        "Blurry    Puddle Of Mudd",
        bank_type_1
    };
    struct BankDefinition bullet_with_butterfly_wings = {
        "Bullet w/ Butterfly Wings     Smashing",
        bank_type_1
    };
    struct BankDefinition cochise = {
        "Cochise   Audioslave",
        bank_type_1
    };
    struct BankDefinition come_out_and_play = {
        "Come Out  And Play  The Offspring",
        bank_type_1
    };
    struct BankDefinition even_flow = {
        "Even Flow Pearl Jam",
        bank_type_1
    };
    struct BankDefinition everlong = {
        "Everlong  Foo Fighters",
        bank_type_1
    };
    struct BankDefinition flagpole_sitta = {
        "Flagpole  Sitta     Harvey    Danger",
        bank_type_1
    };
    struct BankDefinition gel = {
        "Gel       CollectiveSoul",
        bank_type_1
    };
    struct BankDefinition give_it_away = {
        "Give It   Away      RHCP",
        bank_type_1
    };
    struct BankDefinition hash_pipe = {
        "Hash Pipe Weezer",
        bank_type_1
    };
    struct BankDefinition interstate_love_song = {
        "InterstateLove Song STP",
        bank_type_1
    };
    struct BankDefinition jeremy = {
        "Jeremy    Pearl Jam",
        bank_type_1
    };
    struct BankDefinition killing_in_the_name = {
        "Killing InThe Name  RATM",
        bank_type_1
    };
    struct BankDefinition kryptonite = {
        "Kryptonite3 Doors   Down",
        bank_type_1
    };
    struct BankDefinition lump = {
        "Lump      PresidentsOf The USA",
        bank_type_1
    };
    struct BankDefinition machinehead = {
        "Machinehead Bush",
        bank_type_1
    };
    struct BankDefinition monkey_wrench = {
        "Monkey    Wrench    Foo Fighters",
        bank_type_1
    };
    struct BankDefinition my_own_worst_enemy = {
        "My Own    Worst     Enemy     Lit",
        bank_type_1
    };
    struct BankDefinition push = {
        "Push      Moist",
        bank_type_1
    };
    struct BankDefinition rape_me = {
        "Rape Me   Nirvana",
        bank_type_1
    };
    struct BankDefinition resurrection = {
        "Resurrection        Moist",
        bank_type_1
    };
    struct BankDefinition santamonica = {
        "Santamonica Everclear",
        bank_type_1
    };
    struct BankDefinition santeria = {
        "Santeria  Sublime",
        bank_type_1
    };
    struct BankDefinition sex_type_thing = {
        "Sex Type  Thing     STP",
        bank_type_1
    };
    struct BankDefinition shine = {
        "Shine     CollectiveSoul",
        bank_type_1
    };
    struct BankDefinition smells_like_teen_spirit = {
        "Smells    Like Teen Spirit    Nirvana",
        bank_type_1
    };
    struct BankDefinition song_2 = {
        "Song 2    Blur",
        bank_type_1
    };
    struct BankDefinition supermans_dead = {
        "Superman'sDead      Our Lady  Peace",
        bank_type_1
    };
    struct BankDefinition the_kids_arent_allright = {
        "the Kids  Aren't    Allright  Offspring",
        bank_type_1
    };
    struct BankDefinition the_middle = {
        "The MiddleJimmy Eat World",
        bank_type_1
    };
    struct BankDefinition whats_my_age_again = {
        "What's my age again Blink 182",
        bank_type_1
    };
    struct BankDefinition zombie = {
        "Zombie    The       Cranberries",
        bank_type_1
    };

    struct BankDefinition AlterNation_Banks[ADDITIONAL_BANKS_NUMBER] = {
        all_the_small_things,
        arey_you_gonna_be_my_girl,
        basket_case,
        blurry,
        bullet_with_butterfly_wings,
        cochise,
        come_out_and_play,
        even_flow,
        everlong,
        flagpole_sitta,
        gel,
        give_it_away,
        hash_pipe,
        interstate_love_song,
        jeremy,
        killing_in_the_name,
        kryptonite,
        lump,
        machinehead,
        monkey_wrench,
        my_own_worst_enemy,
        push,
        rape_me,
        resurrection,
        santamonica,
        santeria,
        sex_type_thing,
        shine,
        smells_like_teen_spirit,
        song_2,
        supermans_dead,
        the_kids_arent_allright,
        the_middle,
        whats_my_age_again,
        zombie
    };

    ButtonReader* button_reader = new ButtonReaderRegister(
        NUMBER_OF_BUTTON,
        IN_LOAD_PIN,
        IN_CLOCK_PIN,
        IN_CLOCK_ENABLE_PIN,
        IN_DATA_PIN,
        NUMBER_OF_SHIFT_REGISTER
    );
    // BankManager* bank_manager = new BankManager(EEPROM_START_ADDRESS, NUMBER_OF_EEPROM_BANKS, PATCHES_PER_BANK);
    // OLED* oled = new OLED();

    button_reader->init();
    // bank_manager->init(AlterNation_Banks, ADDITIONAL_BANKS_NUMBER);
    // oled->init();

    effect_switcher.init(
        button_reader
        // bank_manager,
        // oled
    );
    effect_switcher.init_bank_manager(
        EEPROM_START_ADDRESS, 
        NUMBER_OF_EEPROM_BANKS, 
        PATCHES_PER_BANK,
        AlterNation_Banks,
        ADDITIONAL_BANKS_NUMBER
    );
}

void loop() {
    effect_switcher.read_and_apply();
}
