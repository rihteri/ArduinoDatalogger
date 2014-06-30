#ifndef Button_h
#define Button_h

#include "Arduino.h"

struct ButtonFunction
{
    int delay;
    void (*callback)();
};

class Button
{
    public:
        Button();
        ~Button();

        void attachFunction(void (*callback)(), int delay = 0);

        /// set to HIGH or LOW often
        void update(char value);

    private:
        ButtonFunction* _functions;
        char _nfunctions;
        long _startMillis;
};

#endif // Button_h
