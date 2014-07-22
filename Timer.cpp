#include "Timer.h"

Timer::Timer(void (*callback)(), int delay, boolean repeat)
    : _delay(delay), _callback(callback), _repeat(repeat)
{
    _start = millis();
}

void Timer::update()
{
    if (millis() > _start + _delay
       && _start != -1)
    {
        if (!_repeat)
        {
            _start = -1;
        }
        else
        {
            _start = millis();
        }

        _callback();

        // do nothing after this line in case we got deleted
    }
}
