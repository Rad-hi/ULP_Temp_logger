#include"WIFI.h"

#ifndef __MQTT_H__
  #include<ESP8266WiFi.h>
#endif  

void off_unnecessary(){
  WiFi.disconnect(true);  
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(2);
}

void wake_wifi_up(){
  // Wake wifi module up
  WiFi.forceSleepWake();
  delay(2);
  
  // Disable the WiFi persistence. ESP8266 will not load and save WiFi settings in the flash memory.
  WiFi.persistent(false);

  // Make a static IP address to decrease DHCP IP request time
  IPAddress staticIP(104,21,47,183); // IP of adafruit.io
  IPAddress dns(107,21,47,183);      // DNS address of adafruit.io
  IPAddress gateway(192,168,1,9);    // Optional, so left them randomly
  IPAddress subnet(255,255,255,0);   // Default
  WiFi.config(staticIP, dns, gateway, subnet);

  // Turn on WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WLAN_SSID, WLAN_PASSWD);
  
  unsigned long wifi_start = millis();

  // Wait for WiFi connection to be established
  while(WiFi.status() != WL_CONNECTED && millis() - wifi_start < WIFI_TIMEOUT){
    yield(); // Lets the background tasks finish before passing to next task
  }
  
  if(WiFi.status() != WL_CONNECTED){
    ESP.restart();
  }
}
