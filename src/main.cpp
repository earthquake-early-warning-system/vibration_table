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

Servo myservo; // create servo object to control a servo
// twelve servo objects can be created on most boards

void setup()
{
  myservo.attach(D7); // attaches the servo on GIO2 to the servo object
}

#define MAX_SIZE_SIN (11)
float inc_sin[MAX_SIZE_SIN] =
    {
        1.5192246987792,
        1.23116594048622,
        0.973193125842964,
        0.745384835867802,
        0.547810463172671,
        0.380530190825445,
        0.24359497401758,
        0.137046524542617,
        0.060917298090424,
        0.015230484360873,
        0};

void vibrate(float freq_hz, float amp)
{
  static elapsedMillis elapsedTime = 0;
  static bool alternate = true;
  const float pos = 90.0f;
  static float interval;
  static float inc = 0.0;
  const float res = 0.0001;
  const float inc_res = 0.1;
  float angle = 0.0f;
  //static float index = 0;

  interval = (1000.0f / freq_hz) * res;

  if (elapsedTime >= interval)
  {
    elapsedTime = 0;
    inc += inc_res; //index MAX_SIZE_SIN'
    inc = inc >= 360 ? 0 : inc;
    angle = pos + sin(radians(inc)) * amp; 
    myservo.write(angle);
  }
} 

int const time_ms = 500;
void loop()
{
  static elapsedMillis elapsedTime = 0;
  static int inc = 1 ;

  if(elapsedTime >= 5000)
  {
    elapsedTime = 0;
    inc += 1;
  }
  vibrate(inc, 20);

  // for (float inc = 0; inc < 360; inc += 1)
  // { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   myservo.write(90 + sin(radians(inc))*10); // tell servo to go to position in variable 'pos'
  //   delay(1);               // waits 15ms for the servo to reach the position
  // }

  // for (float inc = 360; inc >= 0; inc -= 1)
  // { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   myservo.write(90 + sin(radians(90 + inc))*10); // tell servo to go to position in variable 'pos'
  //   delay(10);               // waits 15ms for the servo to reach the position
  // }
  //delay(1000);
  // for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
  //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15ms for the servo to reach the position
  // }
}
