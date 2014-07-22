// Datalog
//
// A library for reading noisy analog input
// and remembering highlights of the past.

#ifndef Datalog_h
#define Datalog_h

#include "Arduino.h"

#include "settings.h"
#include "SmartArray.h"

#ifdef DATALOG_USE_EEPROM
#include "EEPROMStream.h"
#endif

#include "Aggregate.h"

class Datalog
{
    public:
        /// Construct a new datalogger for one data type, optionally defining
        /// the desired amount of history value storage
        Datalog(int valuesCount = 32
#ifdef DATALOG_USE_EEPROM
                , int eeprom_addr = -1
#endif
               );
    
        ~Datalog();
    
        /// set the latest value
        void update(double value);

        /// returns true if fast-change detection was activated at the
        /// last update().
        boolean isMoving();

        double getValue();

        Aggregate getAggregates();

        void resetAggregates();

        boolean isValid();

    private:
        void updateAggregates(double currentValue);
        int getOutlierSize();

        /// The minimum and maximum values of this session
        Aggregate _aggr;
        bool _aggrInited;

        SmartArray* _values;
        SmartArray* _outliers;

        const int _avgSize;

#ifdef DATALOG_USE_EEPROM
        EEPROMStream _rom;
#endif

        /// true, if values are currently changing fast
        boolean _moving;

        static const int OUTLIER_RATIO = 4;
        static const double ALLOWED_DEVIATION = 1.5;
        static const int EXTREME_PRECISION = 1;
};



#endif //Datalog_h
