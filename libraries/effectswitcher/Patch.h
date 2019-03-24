#ifndef GOATBOX_PATCH_H
#define GOATBOX_PATCH_H

#include <Arduino.h>

class Patch {

private:
    bool can_edit;
    int patch_number;
    int eeprom_address;
    byte selected_effects;

public:
    Patch(int _patch_number, int _eeprom_address = 0, int _selected_effects = 0, int _load_from_eeprom = true, bool _can_edit = true);
    ~Patch();

    void load();
    void save(byte _selected_effects);

    byte get_selected_effects() const;
};

#endif //GOATBOX_PATCH_H
