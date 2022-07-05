/*
  BMP180.h - Library for reading from BMP180 sensor
  Created by Željko Eremić, May 28, 2020.
  Released into the public domain.

  BMP180     NODEMCU
  -------------
  SCL        D1
  SDA        D2
  VCC        3.3V
  GND        0V
  
*/

#ifndef BMP180_h
#define BMP180_h

class BMP180
{
  public:
    BMP180();
    double GetAirPressure();
  private:
    double m_airPressureValue;
};

#endif
