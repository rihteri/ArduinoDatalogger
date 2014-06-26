/*
    Smart array, for storing multiple values in a
    variable size array
*/

#ifndef SmartArray_h
#define SmartArray_h

#include "Arduino.h"

class SmartArray
{
    public:
        SmartArray(int size);
        SmartArray(int size, SmartArray* initial_data);

        ~SmartArray();

        void pushValue(int value);

        double getAvg();

        double getStdDev();

        boolean isFull();

        /// Get the number of valid values in the array
        int getValueCount();

    private:
        /// Get average of an array
        double getAvg(double values[], int count);

        int getValue(int i);

        int* _values;
        int _currentIndex;
        bool _isFull;
        int _size;
};

#endif //SmartArray_h
