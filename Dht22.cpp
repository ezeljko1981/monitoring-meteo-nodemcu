/*
  Dht22.cpp - Library for reading from Dht22 sensor
  Created by Željko Eremić, May 30, 2020.
  Released into the public domain.

  DHT22      NODEMCU
  -------------
  DATA       D3
  VCC        5V
  GND        0V
  
*/
#include "Arduino.h"
#include "Dht22.h"
#include "DHT.h"

#define DHTPIN D3     
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);

Dht22::Dht22(){
  dht.begin();  
}

float Dht22::ReadDht22temperature(){
  float t = dht.readTemperature();
  return t;
}

float Dht22::ReadDht22humidity(){
  float h = dht.readHumidity();
  return h;
}   
