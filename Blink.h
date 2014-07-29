#ifndef Blink_h
#define Blink_h

#include "Arduino.h"

class Blink
{
    public:
        Blink(char pin, int time_lo, int time_hi);
        void update();

    private:
        void writeState();

        const int _time_lo, _time_hi;
        long _lastChg;
        char _state;
        char _pin;
};

#endif
