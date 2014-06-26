# Datalogger library for Arduino
This is a simple library for reading noisy analog inputs in Arduino.

# Useful functions
#### Datalog(int pin, int valuesCount = 32)
Initialize a datalogger class on selected pin, with the designated amount of storage.

#### void Datalog.update()
Read value from pin.

#### double Datalog.getValue()
Get current (smooth) value.

#### MinMaxVal Datalog.getExtremes()
Get a struct that contains the record high and low.

```
struct MinMaxVal
{
    double min;
    double max;
};
```

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
  // run this often, it will keep your average better
  temperature.update();
  
  // note that we get double from analog inputs!
  double val = temperature.getValue(); 
  
  // calculate temperature from reading
  double voltage = (val / 1024.0) * 5.0;
  double temp = ( voltage - 0.5 ) * 100;

  Serial.print(temp);
  Serial.println(" deg C");
  
  delay(100);
}
```