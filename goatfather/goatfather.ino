// #include <Arduino.h>
#include "EffectSwitcher.h"

EffectSwitcher effect_switcher;

void setup() {
    effect_switcher.init();
}

void loop() {
    effect_switcher.read_and_apply();
}
