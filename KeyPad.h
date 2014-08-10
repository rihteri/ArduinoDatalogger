#ifndef KeyPad_h
#define KeyPad_h

#include "Arduino.h"

enum PressedKey
{
  NONE = 0,
  SELECT = 1,
  LEFT = 2,
  RIGHT = 4,
  UP = 8,
  DOWN = 16
};

/// A class for handling the buttons on an
/// LCD keypad shield
class KeyPad
{
    public:
        static PressedKey readKey();
};

#endif // KeyPad_h
