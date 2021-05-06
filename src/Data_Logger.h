#ifndef __DATA_LOG_H__
  #define __DATA_LOG_H__

  #include <ArduinoJson.h>
  #include <LittleFS.h>                 // https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html#directory-object-dir

  // File path in the Flash of the ESP8266
  #define FILE_PATH               "/Temp_Log.txt"
  
  // Message buffer
  #define JSON_BUFFER_SIZE        2048  // Calculate the correct size using:
                                        // https://arduinojson.org/v5/assistant/
                                               
  void write_temp(float, bool);
  void read_day(char*);
  void remove_file(const char*);

#endif
