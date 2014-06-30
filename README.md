# Datalogger library for Arduino
This is a simple library for reading noisy analog inputs in Arduino.

# Useful functions
#### Datalog(int valuesCount = 32, int eeprom_address = -1)
Initialize a datalogger class, with the designated amount of storage. If an eeprom address is specified, the minimums and maximums will be persisted there.

#### void Datalog.update(double value)
Set the latest value.

#### double Datalog.getValue()
Get current (smooth) value. This will be an average of the latest values given in with update(double).

#### MinMaxVal Datalog.getExtremes()
Get a struct that contains the record high and low.

```
struct MinMaxVal
{
    double min;
    double max;
};
```

#### void Datalog.resetExtremes()
Reset the minimum and maximum value.

#### boolean isMoving()
Returns true, if a fast change was noticed on last update, and therefore the value was changed to a "new normal".

# Example
For now, it looks like this:

```
#include <Datalog.h>

// init with pin number and number of samples to
// store for averaging
Datalog temperature(A2, 32);

void setup() {
  Serial.begin(9600);
}

void loop() {
  // calculate temperature from reading
  double voltage = (val / 1024.0) * 5.0;
  double temp = ( voltage - 0.5 ) * 100;
  
  // run this often, it will keep your average better
  temperature.update(temp);

  // get the smoothed value
  double val = temperature.getValue(); 
  
  Serial.print(temp);
  Serial.println(" deg C");
  
  delay(100);
}
```