#include <EEPROM.h>

#include "Datalog.h"

Datalog::Datalog(char pin, int valuesCount, int eeprom_addr) :
    _pin(pin),
    _moving(false),
    _avgSize(valuesCount),
    _extremesInited(false),
    _rom(eeprom_addr)
{
    _values = new SmartArray(_avgSize);
    _outliers = new SmartArray(getOutlierSize());

    if (eeprom_addr != -1)
    {
        _sessionExtremes.min = _rom.readDouble();
        _sessionExtremes.max = _rom.readDouble();

        if (_sessionExtremes.min > _sessionExtremes.max
           || isnan(_sessionExtremes.min)
           || isnan(_sessionExtremes.max))
        {
            _extremesInited = false;
        }
        else
        {
            _extremesInited = true;
        }
    }
}

Datalog::~Datalog()
{
    delete _values;
    _values = NULL;

    delete _outliers;
    _outliers = NULL;
}

int Datalog::getOutlierSize()
{
    int outlierSize = _avgSize/OUTLIER_RATIO;
    if (outlierSize < 2)
    {
        outlierSize = 2;
    }

    return outlierSize;
}

void Datalog::update(double scale)
{
    int val = analogRead(_pin)*scale;

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
                _outliers = new SmartArray(getOutlierSize());

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
    Serial.print("min: ");
    Serial.println(_sessionExtremes.min);
    Serial.print("max: ");
    Serial.println(_sessionExtremes.max);

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

            _rom.seek(sizeof(value));
            _rom.write(value);
            Serial.println("WRITE MAX");
        }
        else if (value < _sessionExtremes.min)
        {
            _sessionExtremes.min = value;

            _rom.seek(0);
            _rom.write(value);

            Serial.println("WRITE MIN");
        }
    }
}

void Datalog::resetExtremes()
{
    _extremesInited = false;
    _rom.seek(0);

    for (int i = 0;
         i < (sizeof(_sessionExtremes.min)
              + sizeof(_sessionExtremes.max));
         i++)
    {
        _rom.write((byte)255);
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


