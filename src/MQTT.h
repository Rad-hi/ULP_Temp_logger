#ifndef __MQTT_H__
  #define __MQTT_H__
  
  #include<ESP8266WiFi.h>
  #include <PubSubClient.h>  // https://github.com/knolleary/pubsubclient/
  
  #define BROKER            "io.adafruit.com"
  #define MQTT_PORT         1883
  #define MQTT_USER         "" // User name in adafruit.io
  #define MQTT_PASSWORD     "" // MQTT Key in adafruit.io
  #define TOPIC             "" // User_Name/feeds/Topic_Name_Of_Choice
  #define MQTT_ID           "" // Create fixed ID for your ESP8266 board
                               // If connected to the broker once with an ID, never change it!
  
  void MQTT_setup();
  void MQTT_connect();
  void MQTT_subscribe();
  void MQTT_callback(char*, byte*, unsigned int);
  void send_data(const char*);
  bool mqtt_state();
#endif
