#include "SerialShell.h"

SerialShell::SerialShell(Stream* serial) :
    _serial(serial),
    _functionCount(0),
    _functions(NULL),
    _data(NULL),
    _len(0)
{

}

SerialShell::~SerialShell()
{
    if (_data != NULL) delete _data;

    delete _functions;
}

void SerialShell::registerHandler(
    char* command,
    void (*callback)(Print*, char*))
{
    _functionCount++;

    HandlerFunction* func_old = _functions;
    _functions = new HandlerFunction[_functionCount];

    for (int i = 0; i < _functionCount - 1; i++)
    {
        _functions[i] = func_old[i];
    }

    _functions[_functionCount - 1].command = command;
    _functions[_functionCount - 1].callback = callback;
}

void SerialShell::readFromStream()
{
    char len = 0;
    if ((len = _serial->available()) > 0)
    {
        if (_len > 0) _len--;
        char* oldData = _data;
        _data = new char[_len + len + 1];

        if (oldData != NULL)
        {
            for (int i = 0; i < _len; i++)
            {
                _data[i] = oldData[i];
            }

            delete oldData;
        }

        len = _serial->readBytes(_data + _len, len);
        _len = _len + len + 1;
        _data[_len - 1] = NULL;
    }
}

void SerialShell::update()
{
    readFromStream();

    if (_data[_len - 2] == '\n')
    {
        // full command received
        for (int i = 0; i < _functionCount; i++)
        {
            for (int c = 0; c < _len; c++)
            {
                if (_functions[i].command[c] == NULL
                   && (_data[c] == ' ' || _data[c] == '\n'))
                {
                    // match
                    char* newData = NULL;
                    int newDataLen = 0;
                    int nlPos = 0;
                    for (int cc = c + 1; cc < _len; cc++)
                    {
                        if (nlPos > 0)
                        {
                            newData[cc - nlPos - 1] = _data[cc];
                        }
                        else if (_data[cc] == '\n')
                        {
                            if (cc < _len - 2)
                            {
                                // multiple commands
                                nlPos = cc;
                                newDataLen = _len - cc - 1;
                                newData = new char[newDataLen + 1];
                            }

                            _data[cc] = NULL;
                        }
                    }

                    if (_data[c + 1] == NULL)
                    {
                        _functions[i].callback(_serial, NULL);
                    }
                    else
                    {
                        _functions[i].callback(_serial, _data + c + 1);
                    }

                    _len = 0;
                    delete _data;
                    _data = NULL;

                    if (newData != NULL)
                    {
                        _data = newData;
                        _len = newDataLen;
                    }
                }
                else if (_data[c] == _functions[i].command[c])
                {
                    // ok, continue
                }
                else
                {
                    // no match, go to next command
                    break;
                }
            }
        }

        flushTrash();
    }
}

void SerialShell::flushTrash()
{
    if (_data[_len - 2] == '\n')
    {
        char *oldData = _data;
        int nlPos = 0;

        for (int i = 0; i < _len; i++)
        {
            if (nlPos > 0)
            {
                _data[i - nlPos - 1] = oldData[i];
            }
            else if (i < _len -2 && _data[i] == '\n')
            {
                oldData = _data;
                _data = new char[_len - i];
                nlPos = i;
            }
        }

        delete oldData;
        _len = _len - nlPos - 1;

        if (nlPos == 0)
        {
            _data = NULL;
            _len = 0;
        }
    }
}
