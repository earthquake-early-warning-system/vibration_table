/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include "Arduino.h"
#include <elapsedMillis.h>

void _setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void _loop()
{
  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, HIGH);
  // wait for a second
  delay(1000);
  // turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, LOW);
   // wait for a second
  delay(1000);
}

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards


void setup() {
  myservo.attach(D7);  // attaches the servo on GIO2 to the servo object
}

void vibrate(float freq_hz, float amp)
{
  static elapsedMillis elapsedTime;
  static bool alternate=false;
  const float pos = 90.0f;
  static float angle = pos;
  static float interval = 1000.0f/freq_hz;
  
  //if(alternate 
  // angle += alternate == true ? 0.1 : -0.1;

  // if(angle >= pos+amp)
  // {
  //   alternate = false;
  // }

  // if (elapsedTime >= interval)
  // {
  //   elapsedTime = 0;
    


    if(alternate)
    {
      //alternate = false;
      myservo.write(pos + amp);
    }
    else
    {
      //alternate = true;
      myservo.write(pos - amp);
    }
    //delay();
    
    alternate = alternate == true ? false : true;
  }
   
}


int const time_ms = 500;
void loop() {
   

  vibrate(1, 10);
  // for (pos =0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(80);                       // waits 15ms for the servo to reach the position
  // }
  //delay(1000);
  // for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
  //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15ms for the servo to reach the position
  // }
}
