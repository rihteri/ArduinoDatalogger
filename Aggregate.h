#ifndef Aggregate_h
#define Aggregate_h

#include "Arduino.h"

struct Aggregate
{
    unsigned long minTime;
    double min;
    unsigned long maxTime;
    double max;

    double avg;
    unsigned int sample_count;
};

#endif // Aggregate_h
