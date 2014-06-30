// Datalog
//
// A library for reading noisy analog input
// and remembering highlights of the past.

#ifndef Datalog_h
#define Datalog_h

#include "Arduino.h"

#include "SmartArray.h"
#include "EEPROMStream.h"

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
        Datalog(int valuesCount = 32, int eeprom_addr = -1);
    
        ~Datalog();
    
        /// set the latest value
        void update(double value);

        /// returns true if fast-change detection was activated at the
        /// last update().
        boolean isMoving();

        double getValue();

        MinMaxVal getExtremes();

        void resetExtremes();

    private:
        void updateAggregates(double currentValue);
        int getOutlierSize();

        /// The minimum and maximum values of this session
        MinMaxVal _sessionExtremes;
        bool _extremesInited;

        SmartArray* _values;
        SmartArray* _outliers;

        const int _avgSize;
        EEPROMStream _rom;

        /// true, if values are currently changing fast
        boolean _moving;

        static const int OUTLIER_RATIO = 4;
        static const double ALLOWED_DEVIATION = 1.5;
        static const int EXTREME_PRECISION = 1;
};



#endif //Datalog_h
