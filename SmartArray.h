// Smart array, for storing multiple values in a
// variable size array

#ifndef SmartArray_h
#define SmartArray_h

class SmartArray
{
    public:
        SmartArray(int size);
        SmartArray(int size, SmartArray* initial_data);

        ~SmartArray();

        void pushValue(double value);

        double getAvg();

        double getStdDev();

        boolean isFull();

        /// Get the number of valid values in the array
        int getValueCount();

        /// true, if this is the value that completes a full cirle
        boolean isLooping();

        void clear();

    private:
        /// Get average of an array
        static double getAvg(double values[], int count);

        int getValue(int i);

        double* _values;
        int _currentIndex;
        bool _isFull;
        const int _size;
};

#endif //SmartArray_h
