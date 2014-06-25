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
        Datalog(int pin, int avgSize = 32);
    
        ~Datalog();
    
        /// Read the latest value
        void update();
    
        /// Get a smoothed out value
        double getValue();

        /// Returns the standard deviation of the last measurements
        double getStdDev();

        boolean isMoving();
    private:
        /// Get the number of valid values in the array
        int getValueCount();

        /// Get average of an array
        double getAvg(double values[], int count);

        /// Get average of an array
        double getAvg(int values[], int count);

        /// Write a value to the array
        void setValue(int value);

        /// The latest measurements for averaging
        int* _values;

        /// Index of last value;
        int _curVal;

        /// Number of last values to store
        int _valSize;

        /// Pin to read
        int _pin;

        /// true after at least _valSize values have been read
        boolean _inited;

        int _positiveOutlierCount;
        int _negativeOutlierCount;

        /// true, if values are currently changing fast
        boolean _moving;
};



#endif Datalog_h
