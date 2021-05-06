#ifndef __WIFI_H__
  #define __WIFI_H__
  
  #include"MQTT.h" 

  // WiFi data
  #define WLAN_SSID               ""
  #define WLAN_PASSWD             ""

  // 
  #define WIFI_TIMEOUT            20000UL // 20 seconds timeout to connect to WiFi
  
  void off_unnecessary();
  void wake_wifi_up();
#endif
