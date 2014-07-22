# Datalogger library for Arduino
This is a simple library for reading noisy analog inputs in Arduino.

# Useful functions

## Datalog
Datalog is a class for averaging noisy input and storing highlights.

#### Datalog(int valuesCount = 32, int eeprom_address = -1)
Initialize a datalogger class, with the designated amount of storage. If `eeprom_address` is specified, the minimums and maximums will be persisted there.

#### void update(double value)
Set the latest value.

#### double getValue()
Get current (smooth) value. This will be an average of the latest values given in with `update(double)`.

#### MinMaxVal getExtremes()
Get a struct that contains the record high and low.

```
struct MinMaxVal
{
    double min;
    double max;
};
```

#### void resetExtremes()
Reset the minimum and maximum value.

#### boolean isMoving()
Returns `true`, if a fast change was noticed on last update, and therefore the value was changed to a "new normal".

### Example
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

## Button
Button is a class for attaching multiple functions to buttons. Attach only one function per time, and try to add enough time between them (like at least one second). After a button press, the callback with longest matching delay will be executed.

### attachFunction(void (*callback)(), int delay = 0))
Add a new listener to button, will be executed if button is pressed for at least the amount of milliseconds specified in `delay`.

### update(char value)
Update the value of the input and execute callbacks as necessary. Give in `HIGH` or `LOW` (usually directly from `digitalRead()`).

### Example

````
char BUTTON_PIN = 0;
Button b;

void myAction()
{
    // do something here when button pressed
}

void longAction()
{
    // do something here when button pressed for one second
}

void setup()
{
    pinMode(BUTTON_PIN, INPUT);
    b.attachFunction(myAction);
    b.attachFunction(longAction, 1000);
}

void loop()
{
    b.update(digitalRead(BUTTON_PIN));
}
```


### Settings

The file `settings.h` includes #defines you can comment away to save some space by removing unnecessary code.
