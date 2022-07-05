/*
  NodeMCU_Firebase.h - Library for reading values and sending to Firebase. Also getting commands from Firebase.
  Created by Željko Eremić, May 10, 2020.
  Released into the public domain.
*/

#include "Arduino.h"
#include "NodeMCU_Firebase.h"
#include "BMP180.h"
#include "Dht22.h"
#include <ESP8266WiFi.h>                                            // esp8266 library
#include <FirebaseArduino.h>                                        // firebase library

#include <NTPClient.h>  // Network Time Protocol (NTP)
#include <WiFiUdp.h>    //User Datagram Protocol (UDP) on port 123

#define FIREBASE_HOST "vtszr-9e108.firebaseio.com"                  // the project name address from firebase id
#define FIREBASE_AUTH "YOURSECRETKEY"                  // the secret key generated from firebase Service accounts>Database secrets

#define WIFI_SSID     "YOURSSID"                                     // put your home or public wifi name 
#define WIFI_PASSWORD "YOURPASSWORD"                                 // put your password of wifi ssid

const long utcOffsetInSeconds = 7200;  //UTC offset for your timezone in milliseconds
int mc = 0; 

BMP180 bmp180;          //Object of BMP180 sensor
Dht22  dht22;           //Object of Dht22 sensor

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", utcOffsetInSeconds);
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
   
NodeMCU_Firebase::NodeMCU_Firebase(int relayPin, int trigPin, int echoPin){ 
  m_NextNodeMCU_FirebaseState = IDLE_STATE; 
  m_State = IDLE_STATE;
  //timeSpan = 10 * 60 * 1000; // mm * sec * msec
  timeSpan = 1 * 15 * 1000; // mm * sec * msec
  currentTime = millis();
  targetTime = currentTime + timeSpan;
};

void NodeMCU_Firebase::Loop()
{
  m_State = m_NextNodeMCU_FirebaseState;
  switch (m_State)
  {
    case IDLE_STATE:
      delay(1);
      currentTime = millis();
      if(targetTime <= currentTime){
        targetTime = currentTime + timeSpan;
        Serial.print("Time: ");
        Serial.println(currentTime); // prints time since program started
        m_NextNodeMCU_FirebaseState = MEASURE_STATE;;
        delay(1);
      }
      break;
      
    case MEASURE_STATE:   
      m_currentTemperature = dht22.ReadDht22temperature();
      m_currentHumidity    = dht22.ReadDht22humidity();
      m_currentPressure    = bmp180.GetAirPressure();
      m_NextNodeMCU_FirebaseState = WRITING_STATE;
      delay(1000);
      break;

    case WRITING_STATE:  
      timeClient.update();
      m_time = timeClient.getFormattedTime();
      m_dayOfWeek = timeClient.getDay();
      String smc = String(mc); 

      Serial.print("Writing preparation for humidity: ");
      Serial.print(m_currentHumidity);
      Firebase.setFloat("/meteoarchive5_current/relhumidity", m_currentHumidity);
      Firebase.setFloat("/meteoarchive5_current/temperature", m_currentTemperature);
      Firebase.setInt("/meteoarchive5_current/atmpressure", m_currentPressure);   
      Firebase.setInt("/meteoarchive5_current/dayofweek", m_dayOfWeek);      
      Firebase.setString("/meteoarchive5_current/time", m_time);  
      
      Firebase.setString("/meteoarchive5/" + smc + "/time", m_time); 
      Firebase.setFloat("/meteoarchive5/" + smc + "/relhumidity", m_currentHumidity);
      Firebase.setFloat("/meteoarchive5/" + smc + "/temperature", m_currentTemperature);
      Firebase.setInt("/meteoarchive5/" + smc + "/atmpressure", m_currentPressure);   
      Firebase.setInt("/meteoarchive5/" + smc + "/dayofweek", m_dayOfWeek);                   
      
      m_NextNodeMCU_FirebaseState = IDLE_STATE;
      mc++;
      delay(1000);
      break;
  }
}

void NodeMCU_Firebase::SetupFirebase()
{
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);           //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                 //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase
}
