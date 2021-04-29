#ifndef __MQTT_H__
  #define __MQTT_H__
  
  #include<ESP8266WiFi.h>
  #include <PubSubClient.h>  // https://github.com/knolleary/pubsubclient/
  
  #define BROKER            "io.adafruit.com"
  #define MQTT_PORT         1883
  #define MQTT_USER         ""
  #define MQTT_PASSWORD     ""
  #define TOPIC             ""
  #define MQTT_ID           ""
  
  void MQTT_setup();
  void MQTT_connect();
  void MQTT_subscribe();
  void MQTT_callback(char*, byte*, unsigned int);
  void send_data(const char*);
  bool mqtt_state();
#endif
