/*
  BMP180.cpp - Library for reading from BMP180 sensor
  Created by Željko Eremić, May 28, 2020.
  Released into the public domain.

  BMP180     NODEMCU
  -------------
  SCL        D1
  SDA        D2
  VCC        3.3V
  GND        0V
  
*/

#include "Arduino.h"
#include "BMP180.h"
#include <SFE_BMP180.h>
#include <Wire.h>
SFE_BMP180 pressure;

#define ALTITUDE 80.0 // Altitude in meters, please set your value in meters

BMP180::BMP180(){}
 
double BMP180::GetAirPressure()
{
  Serial.begin(115200);
  if (pressure.begin()){
    Serial.println("BMP180 init success");
  }else{
    Serial.println("BMP180 init fail\n\n");
    while(1); // Pause forever.
  }
  //loop
  char status;
  double T,P,p0,a;
  status = pressure.startTemperature();
  if (status != 0)
  {
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);
        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          p0 = pressure.sealevel(P,ALTITUDE);
          m_airPressureValue = p0;      
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");

  delay(1000);  // Pause for 10 seconds.
  
  return m_airPressureValue;
}
