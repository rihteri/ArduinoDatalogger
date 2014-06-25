# Datalogger library for Arduino
This is a simple library for reading noisy analog inputs in Arduino.

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
  
  if (tmp.isMoving())
  {
    // quick change detection has been activated
    digitalWrite(13, HIGH);
  }
  else
  {
    digitalWrite(13, LOW);
  }
  
  // calculate temperature from reading
  double voltage = (val / 1024.0) * 5.0;
  double temp = ( voltage - 0.5 ) * 100;

  Serial.print(temp);
  Serial.println(" deg C");
  
  delay(100);
}
```