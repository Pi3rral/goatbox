#ifndef BUTTONREADER_H
#define BUTTONREADER_H

#include "Constants.h"


class ButtonReader {
    
private:
    int buttons_state[NUMBER_OF_BUTTON];
    int buttons_action_time[NUMBER_OF_BUTTON];

public:
    ButtonReader();
    void read();
    int get_actionned_button();
    int get_action_for_button(int _button);
    
};

#endif //BUTTONREADER_H
