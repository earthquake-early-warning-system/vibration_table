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
bool started = false, run_servo = false, print_val = false, timed_eq = false;
long timed_eq_this_now = 0;
elapsedMillis timed_eq_el_time;
int timed_eq_interval = 0;

double mapf(double val, double in_min, double in_max, double out_min, double out_max)
{
  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double sine(float freq_hz, float time_sec, float apm_deg, float phase)
{
  return apm_deg * sin(2 * PI * freq_hz * time_sec + phase);
}
void vibrate(float freq_hz, float amp)
{
  static float last_angle = 0;
  static elapsedMicros elapsed_time = 0, elapsed_print_time = 0;
  static float servo_resp_time = 0; //fabs(angle-last_angle)/60.0;
  const float pos_off = 90.0f;

  double angle = sine(freq_hz, micros64() / 1000000.0, amp, 0); // rads
  double safe_angle = 0.0;
  safe_angle = mapf(angle, radians(-90), radians(90), radians(-60), radians(60));
  //angle = safe_angle;

  if (elapsed_time >= servo_resp_time * 1000000.0) // micros
  {
    elapsed_time = 0;
    servo_resp_time = fabs(degrees(angle) - degrees(last_angle)) / 600.0; ////(radians(60.0)); // 60.0 deg per 0.1 sec
                                                                          //servo_resp_time *= 10.0;

    if (true == run_servo)
    {
      myservo.write(degrees(safe_angle) + pos_off);
    }

    last_angle = angle;
  }

  if (elapsed_print_time >= 0.001 * 1000000.0) // micros
  {
    elapsed_print_time = 0;
    if (print_val == true)
    {
      Serial.printf("dt:%f, da: %f, %f, %f\n", servo_resp_time, degrees(safe_angle), degrees(angle), degrees(angle - last_angle));
    }
  }
}

// void _NOTHIN(float freq_hz, float amp)
// {
//   /* Corrected:: Now time_res independent algo is working.
//         20 ms servo response limitation */
//   /* - if this changes entire equation needs to account for the change*/
//   float time_res = 0.02; // Any servo response time is suitable
//   // Here 40ms reaction time for particular servo
//   static elapsedMicros elapsed_time = 0, elapsed_servo_act_time = 0;
//   //static float last_angle = 0;
//   float angle, pos = 90.0f;

//   float inc_res = 3.6 * freq_hz; //* time_res * 100.0;

//   if (elapsed_time >= time_res * 1000000.0) // micros
//   {
//     elapsed_time = 0;
//     inc += inc_res;
//     inc = inc >= 360 ? 0 : inc;
//     angle = pos + sin(PI / 180.0 * (inc)) * amp;
//     float time_resp = fabs(angle - last_angle) / 60.0;
//     float interval = (1000000.0f) * time_resp; //time_res;

//     if (elapsed_servo_act_time >= interval) // micros
//     {

//       elapsed_servo_act_time = 0;

//       last_angle = angle;
//       myservo.write(angle);
//       //Serial.printf_P("angle: %f\n",angle);
//     }
//   }
// }

int const time_ms = 500;
void loop()
{
  static elapsedMillis elapsedTime = 0;
  static float inc_l = 1;
  static float amp_l = 1;

  if(timed_eq==true)
  {
    if(timed_eq_el_time > timed_eq_interval)
    {
      timed_eq_el_time = 0 ; 
      timed_eq = false;
      started = false;
      run_servo = false;
      Serial.printf_P("timed_eq ended for %d ms with freq: %f and amp %f\n", timed_eq_interval, inc_l, amp_l);
    }
  }  

  if (Serial.available() > 0)
  {

    int instr = Serial.read();


    if (isDigit(instr) == true) // 0 to 9 only
    {
      timed_eq_this_now = millis();  
      timed_eq_el_time = 0; 
      timed_eq = true;   
      started = true;
      run_servo = true;
      timed_eq_interval = (instr - 0x30) * 1000.0; //ms

      Serial.printf_P("timed_eq began for %d ms with freq: %f and amp %f\n", timed_eq_interval, inc_l, amp_l);
    }

    if (instr == 'b')
    {
      started = true;
      Serial.printf_P("begun\n", inc_l);
    }

    if (instr == 'e')
    {
      started = false;
      run_servo = false;
      Serial.printf_P("ended\n", inc_l);
    }

    if (instr == 's')
    {
      run_servo = true;
      Serial.printf_P("run_servo\n", inc_l);
    }

    if (instr == 't')
    {
      run_servo = false;
      Serial.printf_P("stop_servo\n", inc_l);
    }

    if (instr == 'p')
    {
      print_val = true;
      Serial.printf_P("print\n", inc_l);
    }

    if (instr == 'o')
    {
      print_val = false;
      Serial.printf_P("stop print\n", inc_l);
    }

    if (instr == 'r')
    {
      Serial.printf_P("reset\n", inc_l);
      delay(1000);
      ESP.reset();
    }

    if (started)
    { 
      if (instr == '+')
      {
        if (inc_l < 0.5)
        {
          inc_l += 0.0625;
        }
        else
        {
          inc_l += 0.5;
        }
        Serial.printf_P("freq: %f\n", inc_l);
      }

      if (instr == '-')
      {
        if (inc_l < 0.5)
        {
          inc_l -= 0.0625;
        }
        else
        {
          inc_l -= 0.5;
        }
        Serial.printf_P("freq: %f\n", inc_l);
      }

       if (inc_l < 0)
       {
         inc_l = 0;
      //   if (inc_l < 0.5)
      //   {
      //     inc_l += 0.0625;
      //   }
      //   else
      //   {
      //     inc_l += 0.5;
      //   }
          Serial.printf_P("freq clipped: %f\n", inc_l);
        }

      if (inc_l > 50)
      {
        inc_l = 50;
        Serial.printf_P("freq clipped: %f\n", inc_l);
      }

      if (instr == 'a')
      {
        if (amp_l < 0.0625)
        {
          amp_l += 0.00625;
        }
        else
        {
          amp_l += 0.0625;
        }
        Serial.printf_P("amp: %f\n", amp_l);
      }

      if (instr == 'z')
      {
        if (amp_l < 0.0625)
        {
          amp_l -= 0.00625;
        }
        else
        {
          amp_l -= 0.0625;
        }
        Serial.printf_P("amp: %f\n", amp_l);
      }

      if (amp_l < 0)
      {
        amp_l = 0;
        Serial.printf_P("amp clipped: %f\n", amp_l);
      }

      if (amp_l > 50)
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
