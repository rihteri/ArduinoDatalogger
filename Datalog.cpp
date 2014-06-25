#include "Arduino.h"
#include "Datalog.h"

Datalog::Datalog(int pin, int avgSize)
    : _pin(pin),
      _curVal(-1),
      _valSize(avgSize),
      _inited(false),
      _positiveOutlierCount(0),
      _negativeOutlierCount(0),
      _moving(false)
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
    int val = analogRead(_pin);

    double avg = getValue();

    double stdDev = getStdDev();
    
    double deviation = val - avg;

    if (abs(deviation) < stdDev * 1.5
        || !_inited
        || _positiveOutlierCount > _valSize/4
        || _negativeOutlierCount > _valSize/4
        // detect if the value is moving to consistent direction
        || (_moving = (
            (deviation > 0
                && _positiveOutlierCount > _negativeOutlierCount * 2
                && _positiveOutlierCount > _valSize/8)
            || (deviation < 0
                && _negativeOutlierCount > _positiveOutlierCount * 2
                && _negativeOutlierCount > _valSize/8))))
    {
        // mark down outlier count for movement detection
        int outliers = 0;
        if (_negativeOutlierCount > _positiveOutlierCount)
        {
            outliers = _negativeOutlierCount - _positiveOutlierCount;
        }
        else
        {
            outliers = _positiveOutlierCount - _negativeOutlierCount;
        }

        // update value
        setValue(val);

        if (_moving && _inited)
        {
            // set previous values to avg to enable faster response
            int* newNormal = new int[outliers];
            for (int i = 0; i < outliers; i++)
            {
                int index = _curVal - i;

                if (index < 0) index = _valSize - i - _curVal;

                newNormal[i] = _values[index];
            }
            int newNormalAvg = getAvg(newNormal, outliers);
            delete newNormal;

            for (int i = 0; i < outliers; i++)
            {
                int index = _curVal - i - outliers;

                if (index < 0) index = _valSize - i - _curVal - outliers;

                _values[index] = newNormalAvg;
            }
        }
    }
    else
    {
        // value discarded
        if (deviation > 0)
        {
            _positiveOutlierCount++;
        }
        else
        {
            _negativeOutlierCount++;
        }
    }
}

boolean Datalog::isMoving()
{
    return _moving;
}

void Datalog::setValue(int value)
{
    _curVal++;

    if (_curVal == _valSize) 
    {
        _curVal = 0;
        _inited = true;
    }

    _values[_curVal] = value;
    
    _positiveOutlierCount = 0;
    _negativeOutlierCount = 0;
}

double Datalog::getValue()
{
    return getAvg(_values, getValueCount());
}

int Datalog::getValueCount()
{
    int upperLimit = _valSize;
    
    if (!_inited) upperLimit = _curVal + 1;
    
    return upperLimit;
}

double Datalog::getAvg(double values[], int count)
{
    double sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += values[i];
    }
    
    if (count == 0) return 0;

    return sum / count;
}

double Datalog::getAvg(int values[], int count)
{
    double sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += values[i];
    }

    if (count == 0) return 0;

    return sum / count;
}

double Datalog::getStdDev()
{
    int upperLimit = getValueCount();
    double avg = getAvg(_values, upperLimit);

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
