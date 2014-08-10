#include "KeyPad.h"

PressedKey KeyPad::readKey()
{
    int val = analogRead(A0);

    if (val > 850)
    {
        return NONE;
    }
    else if (val > 600)
    {
        return SELECT;
    }
    else if (val > 400)
    {
        return LEFT;
    }
    else if (val > 200)
    {
        return DOWN;
    }
    else if (val > 50)
    {
        return UP;
    }
    else
    {
        return RIGHT;
    }
}
