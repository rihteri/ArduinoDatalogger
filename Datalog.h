// Datalog
//
// A library for reading noisy analog input
// and remembering highlights of the past.

#ifndef Datalog_h
#define Datalog_h

#include "Arduino.h"

class Datalog
{
    public:
        /// Construct a new datalogger for one data type, optionally defining
        /// the desired amount of history value storage
        Datalog(int pin, int avgSize = 5);
    
        ~Datalog();
    
        /// Read the latest value
        void update();
    
        /// Get a smoothed out value
        double getValue();
    private:
        int* _values;
        int _curVal;
        int _valSize;
        int _pin;
        boolean _inited;
};


#endif Datalog_h