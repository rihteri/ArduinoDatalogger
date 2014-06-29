// A class for reading and writing from EEPROM

#ifndef EEPROMStream_h
#define EEPROMStream_h

class EEPROMStream
{
    public:
        EEPROMStream(int address);

        void seek(int position);

        void write(int value);
        void write(char value);
        void write(double value);
        void write(void* data, int size);

        int readInt();
        char readByte();
        double readDouble();
        void read(void* value, int size);

    private:
        const int _addr;
        int _pos;
};



#endif //EEPROMStream_h
