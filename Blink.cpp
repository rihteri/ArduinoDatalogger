#include "Blink.h"

Blink::Blink(char pin, int time_lo, int time_hi)
    : _time_lo(time_lo), _time_hi(time_hi), _state(LOW), _pin(pin)
{
    _lastChg = millis();
}

void Blink::update()
{
    if (_state == HIGH)
    {
        if (millis() > _lastChg + _time_hi)
        {
            _state = LOW;
            writeState();
        }
    }
    else if (_state == LOW)
    {
        if (millis() > _lastChg + _time_lo)
        {
            _state = HIGH;
            writeState();
        }
    }
}

void Blink::writeState()
{
    digitalWrite(_pin, _state);
    _lastChg = millis();
}
