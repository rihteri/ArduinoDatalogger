#include "Arduino.h"
#include "Datalog.h"

Datalog::Datalog(int pin, int avgSize) :
    _pin(pin),
    _moving(false),
    _avgSize(avgSize),
    _extremesInited(false)
{
    _outlierSize = avgSize/OUTLIER_RATIO;
    if (_outlierSize < 2)
    {
        _outlierSize = 2;
    }

    _values = new SmartArray(avgSize);
    _outliers = new SmartArray(_outlierSize);
}

Datalog::~Datalog()
{
    delete _values;
    _values = NULL;

    delete _outliers;
    _outliers = NULL;
}

void Datalog::update()
{
    int val = analogRead(_pin);

    double stdDev = _values->getStdDev();
    double avg = _values->getAvg();
    double deviation = abs(val - avg);

    if (_values->isFull())
    {
        updateAggregates(avg);
    }

    if (deviation > stdDev * ALLOWED_DEVIATION
        && _values->isFull())
    {
        // questionable value
        _outliers->pushValue(val);

        if (_outliers->isFull())
        {
            if (_outliers->getStdDev() < stdDev)
            {
                // new normal
                delete _values;
                _values = new SmartArray(_avgSize, _outliers);

                delete _outliers;
                _outliers = new SmartArray(_outlierSize);

                _moving = true;
            }
            else
            {
                // keep us from reaching 0 stdDev
                _values->pushValue(_outliers->getAvg());
            }
        }
    }
    else
    {
        // update value
        _values->pushValue(val);
        _moving = false;
    }
}

MinMaxVal Datalog::getExtremes()
{
    if (!_extremesInited)
    {
        double current = getValue();
        MinMaxVal ret;
        ret.min = current;
        ret.max = current;
        return ret;
    }
    else
    {
        return _sessionExtremes;
    }
}

void Datalog::updateAggregates(double value)
{
    if (!_extremesInited)
    {
        _sessionExtremes.max = value;
        _sessionExtremes.min = value;
        _extremesInited = true;
    }
    else
    {
        if (value > _sessionExtremes.max)
        {
            _sessionExtremes.max = value;
        }
        else if (value < _sessionExtremes.min)
        {
            _sessionExtremes.min = value;
        }
    }
}

boolean Datalog::isMoving()
{
    return _moving;
}

double Datalog::getValue()
{
    return _values->getAvg();
}


