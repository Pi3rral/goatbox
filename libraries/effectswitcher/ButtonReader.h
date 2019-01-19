#ifndef BUTTONREADER_H
#define BUTTONREADER_H

#define DEFAULT_LONG_PRESS_TIME 3000
#define DOUBLE_PRESSED_MAX_INTERVAL 500


// Button States
enum button_state {
    rest = 0,
    pressed = 1,
    still_pressed = 2,
    long_pressed = 3,
    still_long_pressed = 4,
    released = 5,
    double_pressed = 6
};

class ButtonReader {
    
protected:
    int long_press_time;
    byte number_of_buttons;

    byte* buttons_state;
    unsigned long* buttons_action_time;

    void update_button_state(byte button_number, byte bit_state);

public:
    ButtonReader(byte _number_of_buttons, int _long_press_time = DEFAULT_LONG_PRESS_TIME);
    virtual void init() = 0;
    virtual void read() = 0;
    int get_actionned_button();
    byte get_action_for_button(int _button);
};


class ButtonReaderRegister: public ButtonReader {

protected:
    byte pin_register_load;
    byte pin_register_clock;
    byte pin_register_clock_enable;
    byte pin_register_data_pin;
    byte number_of_registers;

public:
    ButtonReaderRegister(byte _number_of_buttons, 
                         byte _pin_register_load,
                         byte _pin_register_clock,
                         byte _pin_register_clock_enable,
                         byte _pin_register_data_pin, 
                         byte _number_of_registers,
                         int _long_press_time = DEFAULT_LONG_PRESS_TIME);
    virtual void init();
    virtual void read();
};


class ButtonReaderPins: public ButtonReader {

protected:
    byte* buttons_pin;

public:
    ButtonReaderPins(byte _number_of_buttons, 
                     byte* _buttons_pin,
                     int _long_press_time = DEFAULT_LONG_PRESS_TIME);
    virtual void init();
    virtual void read();

};

#endif //BUTTONREADER_H
