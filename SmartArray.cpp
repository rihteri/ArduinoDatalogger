#include "Arduino.h"

#include "SmartArray.h"

SmartArray::SmartArray(int size) :
    _size(size),
    _isFull(false),
    _currentIndex(-1)
{
    _values = new double[_size];
}

SmartArray::SmartArray(int size, SmartArray* initial_data) :
    _size(size),
    _isFull(false),
    _currentIndex(-1)
{
    _values = new double[_size];

    int until = initial_data->getValueCount();

    if (until >= _size)
    {
        until = _size;
        _isFull = true;
    }

    for (int i = 0; i < until; i++)
    {
        pushValue(initial_data->getValue(_currentIndex));
    }
}

SmartArray::~SmartArray()
{
    delete _values;
}

int SmartArray::getValue(int i)
{
    int index = _currentIndex - i;

    if (index < 0)
    {
        index = _size + index;
    }

    return _values[index];
}

boolean SmartArray::isFull()
{
    return _isFull;
}

void SmartArray::pushValue(double value)
{
    _currentIndex++;

    if (_currentIndex >= _size)
    {
        _currentIndex = 0;
        _isFull = true;
    }

    _values[_currentIndex] = value;
}

int SmartArray::getValueCount()
{
    int count = _size;

    if (!_isFull) count = _currentIndex + 1;

    return count;
}

double SmartArray::getAvg(double values[], int count)
{
    double sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += values[i];
    }

    if (count == 0) return 0;

    return sum / count;
}

double SmartArray::getAvg()
{
    int count = getValueCount();
    return getAvg(_values, count);
}

double SmartArray::getStdDev()
{
    int upperLimit = getValueCount();
    double avg = getAvg();

    double* devs = new double[upperLimit];

    for (int i = 0; i < upperLimit; i++)
    {
        double dev = _values[i] - avg;
        devs[i] = dev * dev;
    }

    double stdDev = sqrt(getAvg(devs, upperLimit));

    delete devs;

    return stdDev;
}
