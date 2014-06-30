#ifndef Aggregate_h
#define Aggregate_h

#include "Arduino.h"

struct Aggregate
{
    long minTime;
    double min;
    long maxTime;
    double max;

    double avg;
    unsigned int sample_count;
};

#endif // Aggregate_h
