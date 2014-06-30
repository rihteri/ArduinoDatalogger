#include <EEPROM.h>

#include "Datalog.h"

Datalog::Datalog(int valuesCount, int eeprom_addr) :
    _moving(false),
    _avgSize(valuesCount),
    _aggrInited(false),
    _rom(eeprom_addr)
{
    _values = new SmartArray(_avgSize);
    _outliers = new SmartArray(getOutlierSize());

    if (eeprom_addr != -1)
    {
        _aggr.minTime = 0;
        _aggr.maxTime = 0;
        _aggr.min = _rom.readDouble();
        _aggr.max = _rom.readDouble();

        if (_aggr.min > _aggr.max
           || isnan(_aggr.min)
           || isnan(_aggr.max))
        {
            _aggrInited = false;
        }
        else
        {
            _aggrInited = true;
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

void Datalog::update(double val)
{
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

Aggregate Datalog::getAggregates()
{
    if (!_aggrInited)
    {
        double current = getValue();
        Aggregate ret;
        ret.min = current;
        ret.max = current;
        ret.avg = current;
        ret.sample_count = 0;

        long timeNow = millis();
        ret.minTime = timeNow;
        ret.maxTime = timeNow;

        return ret;
    }
    else
    {
        return _aggr;
    }
}

void Datalog::updateAggregates(double value)
{
    if (!_aggrInited)
    {
        _aggr.max = value;
        _aggr.min = value;
        _aggr.sample_count = 1;
        _aggr.avg = value;

        long timeNow = millis();
        _aggr.minTime = timeNow;
        _aggr.maxTime = timeNow;

        _aggrInited = true;

        if (_rom.isValid())
        {
            _rom.write(
                (void*)&_aggr,
                sizeof(_aggr));
        }
    }
    else
    {
        if (value > _aggr.max)
        {
            _aggr.max = value;
            _aggr.maxTime = millis();

            if (_rom.isValid())
            {
                _rom.seek(sizeof(value));
                _rom.write(value, EXTREME_PRECISION);
            }
        }
        else if (value < _aggr.min)
        {
            _aggr.min = value;
            _aggr.minTime = millis();

            if (_rom.isValid())
            {
                _rom.seek(0);
                _rom.write(value, EXTREME_PRECISION);
            }
        }

        if (_values->isLooping())
        {
            _aggr.avg
                = (_aggr.avg * _aggr.sample_count + value)
                    / ++_aggr.sample_count;
        }
    }
}

void Datalog::resetAggregates()
{
    _aggrInited = false;
}

boolean Datalog::isMoving()
{
    return _moving;
}

boolean Datalog::isValid()
{
    return _values->isFull() && !_moving;
}

double Datalog::getValue()
{
    return _values->getAvg();
}


