// Datalog
//
// A library for reading noisy analog input
// and remembering highlights of the past.

#ifndef Datalog_h
#define Datalog_h

#include "Arduino.h"

#include "SmartArray.h"

struct MinMaxVal
{
    double min;
    double max;
};

class Datalog
{
    public:
        /// Construct a new datalogger for one data type, optionally defining
        /// the desired amount of history value storage
        Datalog(int pin, int valuesCount = 32);
    
        ~Datalog();
    
        /// Read the latest value
        void update();

        /// returns true if fast-change detection was activated at the
        /// last update().
        boolean isMoving();

        double getValue();

        MinMaxVal getExtremes();

    private:
        void updateAggregates(double currentValue);

        /// The minimum and maximum values of this session
        MinMaxVal _sessionExtremes;
        bool _extremesInited;

        /// Pin to read
        int _pin;

        SmartArray* _values;
        SmartArray* _outliers;

        int _outlierSize;
        int _avgSize;

        /// true, if values are currently changing fast
        boolean _moving;

        static const int OUTLIER_RATIO = 4;
        static const double ALLOWED_DEVIATION = 1.5;
};



#endif //Datalog_h
