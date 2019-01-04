#ifndef BUTTONREADER_H
#define BUTTONREADER_H

#include "GoatFather.h"

class ButtonReader {
    private:
        int buttons_state[NUMBER_OF_BUTTON];
        int buttons_action_time[NUMBER_OF_BUTTON];

    public:
        ButtonReader();
        int * read_states();
        int * get_states() const { return buttons_state; }

}

#endif //BUTTONREADER_H