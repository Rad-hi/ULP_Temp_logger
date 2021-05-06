#include"Data_Logger.h"

void write_temp(float temp, bool hour_){
  // The file is created if doesn't exist, we're appending at the end of it
  File file = LittleFS.open(FILE_PATH, "a");
  yield();

  // Failed to open file
  if(!file) return;

  // The '#' char indicates an end of hour. We'll need that in the read_day()
  // function for the daily-report's data parsing
  if(hour_)file.print(String(temp)+",#");
  
  // The ',' char indicates an end of Temp value
  else file.print(String(temp)+",");
  file.close();
}

void read_day(char* buff){
  DynamicJsonDocument DATA(JSON_BUFFER_SIZE);   // Data is stored on the heap!
  //StaticJsonDocument<JSON_BUFFER_SIZE> DATA;  // Data is stored on the stack!
  
  File file = LittleFS.open(FILE_PATH, "r");
  yield();

  // Failed to open file
  if(!file){
    ; // Dunno what's the best thing to do yet
  }
  
  float max_ = 0.0001, min_ = 1000, mean_ = 0;  
  byte hour_ = 0;
  byte readings = 0;
  String val = "";
  while(file.available()){
    char c = file.read(); 
    switch(c - '0'){
      case -2: // Read a '.'
      case 0 ... 9:{ // Read a val --> still parsing a number
        val += c; 
        break;
      }
      case -4:{ // Read a ',' --> end of Temp value
        float f = strtof(val.c_str(), NULL);
        if(f >= max_) max_ = f;
        if(f <= min_) min_ = f;
        mean_ += f;
        readings ++;
        val = "";
        break;
      }
      case -13:{ // Read a '#' --> end of hour
        mean_ /= readings;

        // Fill data to be sent
        String tag = "H"+String(hour_);  
              
        DATA[tag]["mx"] = max_;
        DATA[tag]["mi"] = min_;
        DATA[tag]["mn"] = mean_;
        
        // Reset all values
        max_ = 0.0001;
        min_ = 1000;
        mean_ = 0;
        readings = 0;
        hour_ ++;
        break;
      }
      default: break;
    }
  }
  file.close();
  
  //
  char buff_[JSON_BUFFER_SIZE]; // Create data container
  serializeJson(DATA, buff_);   // Fill the local container with collected data
  strcpy(buff, buff_);

  // Remove the file after we're done with it
  remove_file(FILE_PATH); 

}

void remove_file(const char* path){
  LittleFS.remove(path);
}
