/*
  NodeMCU_Firebase.h - Library for reading values and sending to Firebase. Also getting commands from Firebase.
  Created by Željko Eremić, May 10, 2020.
  Released into the public domain.
*/

#ifndef NodeMCU_Firebase_h
#define NodeMCU_Firebase_h

#include "Arduino.h"

class NodeMCU_Firebase
{
  public:
    NodeMCU_Firebase(int relayPin, int trigPin, int echoPin);   
    void Loop();
    void SetupFirebase();
  private:  
    #define IDLE_STATE                0 
    #define MEASURE_STATE             3
    #define WRITING_STATE             4
    #define TIME_TO_WAIT              2000

    uint8_t m_NextNodeMCU_FirebaseState;
    uint8_t m_State;
    unsigned long m_timeOfMeasure;
    unsigned long m_timeCurrent; 
    float m_currentHumidity;
    float m_currentPressure;
    float m_currentTemperature;
    bool IsItTimeToMeasure();
    String m_time;    
    int m_dayOfWeek;  

    unsigned long currentTime;
    unsigned long targetTime;
    unsigned long timeSpan;  
};

#endif
