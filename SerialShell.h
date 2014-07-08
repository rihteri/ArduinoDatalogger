#ifndef SerialShell_h
#define SerialShell_h

#include "Arduino.h"
#include <Print.h>

struct HandlerFunction
{
    char* command;
    void (*callback)(Print*, char*);
};

class SerialShell
{
    public:
        SerialShell(Stream* serial);
        ~SerialShell();

        void registerHandler(
            char* command,
            void (*callback)(Print*, char*));

        void update();

    private:
        void readFromStream();
        void flushTrash();

        HandlerFunction* _functions;
        char _functionCount;

        char* _data;
        char _len;

        Stream* _serial;
};

#endif
