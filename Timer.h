#ifndef Timer_h
#define Timer_h

#include "Arduino.h"

class Timer
{
    public:
        Timer(void (*callback)(), int delay, boolean repeat = false);
        void update();

    private:
        const int _delay;
        void (*_callback)();
        long _start;
        boolean _repeat;
};

#endif // Timer_h
