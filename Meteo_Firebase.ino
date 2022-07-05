/*
 System for automatic measurement and storage of meteorological data
 NodeMCU_Firebase Library for :
 - Reading values from sensors for temperature, pressure and humididty and sending value to Firebase. 

 The circuit:
  BMP180     NODEMCU
  -------------
  SCL        D1
  SDA        D2
  VCC        3.3V
  GND        0V
  
  DHT22      NODEMCU
  -------------
  DATA       D3 
  VCC        5V
  GND        0V
  
 Library originally added May 10, 2020.
 by Željko Eremić

 This example code is in the public domain.

*/

#include "NodeMCU_Firebase.h"

NodeMCU_Firebase nodeMCU_Firebase(D0, D4, D3);

void setup(void)
{
  Serial.begin(9600);
  nodeMCU_Firebase.SetupFirebase();
  while(1)
  {
    nodeMCU_Firebase.Loop();  
  } 
}
void loop(void) { }
