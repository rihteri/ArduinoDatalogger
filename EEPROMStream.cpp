#include "Arduino.h"
#include <EEPROM.h>

#include "EEPROMStream.h"

EEPROMStream::EEPROMStream(int address) :
    _addr(address),
    _pos(0) { }

void EEPROMStream::seek(int position)
{
    _pos = position;
}

boolean EEPROMStream::isValid()
{
    return _addr >= 0;
}

// write
////////

void EEPROMStream::write(int value)
{
    write((void*)&value, sizeof(value));
}

void EEPROMStream::write(char value)
{
    write((void*)&value, sizeof(value));
}

void EEPROMStream::write(double value, int precision)
{
    if (precision >= 0)
    {
        for (int i = 0; i < precision; i++)
        {
            value = value * 10;
        }

        value = round(value);

        for (int i = 0; i < precision; i++)
        {
            value = value / 10;
        }
    }

    write((void*)&value, sizeof(value));
}

void EEPROMStream::write(void* data, int size)
{
    if (_addr < 0) return;

    byte* data_byte = (byte*)data;

    for (int i = 0; i < size; i++)
    {
        byte curVal = EEPROM.read(_addr + _pos);

        if (curVal != *data_byte)
        {
            EEPROM.write(_addr + _pos, *data_byte);
        }

        _pos++; data_byte++;
    }
}


// read
///////

int EEPROMStream::readInt()
{
    int ret = 0;

    read((void*)&ret, sizeof(ret));

    return ret;
}

char EEPROMStream::readByte()
{
    char ret = 0;

    read((void*)&ret, sizeof(ret));

    return ret;
}

double EEPROMStream::readDouble()
{
    double ret = 0.0;

    read((void*)&ret, sizeof(ret));

    return ret;
}

void EEPROMStream::read(void* value, int size)
{
    byte* data = (byte*)value;
    for (int i = 0; i < size; i++)
    {
        *data++ = EEPROM.read(_addr + _pos++);
    }
}
