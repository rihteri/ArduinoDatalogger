#include "Button.h"

Button::Button() :
    _functions(NULL),
    _startMillis(-1),
    _nfunctions(0),
    _alreadyRun(false)
{ }

Button::~Button()
{
    delete _functions;
}

void Button::attachFunction(void (*callback)(), int delay)
{
    ButtonFunction* old = _functions;
    _nfunctions++;
    _functions = new ButtonFunction[_nfunctions];

    bool newFuncPlaced = false;
    for (int i = 0; i < _nfunctions; i++)
    {
        if (!newFuncPlaced)
        {
            if(i < _nfunctions - 1
               && _functions[i].delay < delay)
            {
                _functions[i] = old[i];
            }
            else
            {
                _functions[i].callback = callback;
                _functions[i].delay = delay;

                newFuncPlaced = true;
            }
        }
        else
        {
            _functions[i] = old[i-1];
        }
    }

    delete old;
}

void Button::update(char value)
{
    if (value == HIGH)
    {
        if (_startMillis < 0)
        {
            _startMillis = millis();
            _alreadyRun = false;
        }
        else if (_functions[_nfunctions-1].delay < millis() - _startMillis
                && !_alreadyRun)
        {
            // last press timeout reached
            _functions[_nfunctions-1].callback();
            _alreadyRun = true;
        }
    }
    else if (value == LOW)
    {
        if (_startMillis >= 0 && !_alreadyRun)
        {
            long millisNow = millis();

            void (*callback)() = NULL;
            for (int i = 0; i < _nfunctions; i++)
            {
                if (_functions[i].delay < millisNow - _startMillis)
                {
                    callback = _functions[i].callback;
                }
            }

            if (callback != NULL)
            {
                callback();
            }
        }

        _startMillis = -1;
    }
}
