#include "Arduino.h"
#include "Datalog.h"

Datalog::Datalog(int pin, int avgSize)
    : _pin(pin),
      _curVal(-1),
      _valSize(avgSize),
      _inited(false)
{
    _values = new int[avgSize];
}

Datalog::~Datalog()
{
    delete _values;
    _values = NULL;
}

void Datalog::update()
{
    _curVal++;
    
    if (_curVal == _valSize) 
    {
        _curVal = 0;
        _inited = true;
    }
    
    _values[_curVal] = analogRead(_pin);
}

double Datalog::getValue()
{
    double sum = 0;
    int upperLimit = _valSize;
    
    if (!_inited) upperLimit = _curVal + 1;
    
    for (int i = 0; i < upperLimit; i++)
    {
        sum += _values[i];
    }
    
    return sum / upperLimit;
}
