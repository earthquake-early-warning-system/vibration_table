/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include "Arduino.h"
#include <elapsedMillis.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//const char* ssid = "********";
//const char* password = "********";

#include <Servo.h>

Servo myservo; // create servo object to control a servo
               // twelve servo objects can be created on most boards

// WiFiUDP Udp;
// unsigned int localUdpPort = 4210;
// char incomingPacket[256];
// char replyPacket[] = "Hi there! Got the message :-)";
void setup()
{
  myservo.attach(D7); // attaches the servo on GIO2 to the servo object
  Serial.begin(115200);
  Serial.printf_P("Start\n");

  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println(" connected");

  // Udp.begin(localUdpPort);
}

// void checkUdpMsg()
// {
//   int packetSize = Udp.parsePacket();
//   if (packetSize)
//   {
//     Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
//     int len = Udp.read(incomingPacket, 255);
//     if (len > 0)
//     {
//       incomingPacket[len] = '\0';
//     }
//     Serial.printf("UDP packet contents: %s\n", incomingPacket);

//     // (...)
//   }
// }

static float inc = 0;
void vibrate(float freq_hz, float amp)
{
  /* Corrected:: Now time_res independent algo is working.
        20 ms servo response limitation */
  /* - if this changes entire equation needs to account for the change*/
  float time_res = 0.02; // Any servo response time is suitable
  // Here 40ms reaction time for particular servo
  static elapsedMicros elapsedTime = 0;

  float inc_res = 3.6 * freq_hz * time_res * 100.0;
  float interval = (1000000.0f) * time_res;
  float angle, pos = 0;

  if (elapsedTime >= interval) // micros
  {

    elapsedTime = 0;

    inc += inc_res;
    inc = inc >= 360 ? 0 : inc;
    angle = pos + sin(PI / 180.0 * (inc)) * amp;
    myservo.write(angle + 90);
    //Serial.printf_P("angle: %f\n",angle);
  }
}

int const time_ms = 500;
bool started = false;
void loop()
{
  static elapsedMillis elapsedTime = 0;
  static float inc_l = 1;
  static float amp_l = 10;

  if (Serial.available() > 0)
  {

    int instr = Serial.read();

    if (instr == 'b')
    {
      started = true;
      Serial.printf_P("begun\n", inc_l);
    }

    if (instr == 'e')
    {
      started = false;
      Serial.printf_P("ended\n", inc_l);
    }

    if (started)
    {

      if (instr == '+')
      {
        inc_l += 0.5;
        Serial.printf_P("freq: %f\n", inc_l);
      }

      if (instr == '-')
      {
        inc_l -= 0.5;
        Serial.printf_P("freq: %f\n", inc_l);
      }

      if (inc_l <= 0)
      {
        inc_l = 1;
        Serial.printf_P("freq clipped: %f\n", inc_l);
      }

      if (inc_l >= 50)
      {
        inc_l = 50;
        Serial.printf_P("freq clipped: %f\n", inc_l);
      }

      if (instr == 'a')
      {
        amp_l += 0.5;
        Serial.printf_P("amp: %f\n", amp_l);
      }

      if (instr == 'z')
      {
        amp_l -= 0.5;
        Serial.printf_P("amp: %f\n", amp_l);
      }

      if (amp_l <= 0)
      {
        amp_l = 1;
        Serial.printf_P("amp clipped: %f\n", amp_l);
      }

      if (amp_l >= 50)
      {
        amp_l = 50;
        Serial.printf_P("amp clipped: %f\n", amp_l);
      }
    }
  }

  // if (elapsedTime >= 5000)
  // {
  //   elapsedTime = 0;
  //   inc += 1;
  // }
  if (started)
  {
    vibrate(inc_l, amp_l);
  }
}
