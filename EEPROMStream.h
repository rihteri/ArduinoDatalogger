// A class for reading and writing from EEPROM

#ifndef EEPROMStream_h
#define EEPROMStream_h

class EEPROMStream
{
    public:
        EEPROMStream(int address);

        void seek(int position);

        void write(int value);
        void write(unsigned long value);
        void write(char value);
        void write(double value, int precision = -1);
        void write(void* data, int size);

        int readInt();
        char readByte();
        double readDouble();
        unsigned long readULong();
        void read(void* value, int size);

        /// true, if the address was valid
        boolean isValid();

    private:
        const int _addr;
        int _pos;
};



#endif //EEPROMStream_h
