/*
 * Main file for the Temperature logging IoT node.
 * ---
 * This file requires a collection of header files to run properly.
 * Header files: {WIFI.h(WIFI.cpp), 
 *                MQTT.h(MQTT.cpp),
 *                Data_Logger.h(Data_Logger.cpp)}
 * ---
 * Written by: Radhi SGHAIER, https://github.com/Rad-hi 
 */

#include"Data_Logger.h"
#include"WIFI.h"

// Turn ON/OFF debug printing (debugging section won't be included in compiled code when VERBOSE is false) 
#define VERBOSE                 true

// If this is true, two random numbers in the appropriate range will simulate the Temp & Smoke inputs
#define SIMULATE_RANDOM         true

// 115.2K baud serial connection to computer
#define SERIAL_MON_BAUD_RATE    115200

//
#define SLEEP_TIME              60000000UL  // 1 minute   (uSeconds) (60e6 works too)
#define SLEEP_COUNTER           60

// RTC Memory locations (It's divided into chunks of 4 bytes)
#define PATTERN_RTC_LOC         0
#define WAKE_COUNTER_RTC_LOC    4
#define HOUR_COUNTER_RTC_LOC    8

// The pattern to be written at the beginning of RTC memory to insure its data validity
// (could be any 4 bytes pattenrn)
#define RTC_VAL_PATTERN         0x66669420

// 
#define SEC_IN_DAY              86400
#define SEC_IN_HOUR             3600

// Functions' prototypes
float read_temperature(float&);
void communicate_();
void check_rtc_mem_validity();
void check_for_daily_report();
void go_to_sleep();

// Global variables
byte end_hour = 0; // This flag is set once on each end of hour
float temp_value = 0;

// These are required to be uint32_t by the RTC memory even though we don't need a 32 bits interger
uint32_t wake_counter; // Counts how many seconds have passed since the last daily report
uint32_t hour_counter; // Keeps track of which hour of the day we're in {0 .. 23}

void setup(){
  // Turn off Wi-Fi
  off_unnecessary(); 

  check_rtc_mem_validity();
  
  // Mount the file-system
  LittleFS.begin();
  
  #if VERBOSE
    Serial.begin(SERIAL_MON_BAUD_RATE);
  #endif

  // Sim
  #if SIMULATE_RANDOM
    randomSeed(analogRead(0)); 
  #endif
}

void loop() {
      read_temperature(temp_value);
      
      // Viz
      #if VERBOSE
        Serial.printf("Temperature value: %f\n", temp_value);
      #endif
      
      check_for_daily_report();
      // Write temperature value to the LOG
      write_temp(temp_value, end_hour);
      go_to_sleep();
      
}

void go_to_sleep(){
  // Unmount the file-system
  LittleFS.end();
  
  // Off WiFi
  off_unnecessary();
  
  #if VERBOSE
    Serial.println("Going to sleep...");
  #endif
    
  // WAKE_RF_DISABLED to keep the WiFi radio disabled when we wake up
  ESP.deepSleepInstant(SLEEP_TIME, WAKE_RF_DISABLED);
}

void check_for_daily_report(){
  // Read how many seconds we've been asleep for, since the last 24h-report
  ESP.rtcUserMemoryRead(WAKE_COUNTER_RTC_LOC, &wake_counter, sizeof(wake_counter));
  // Read which hour of the day we're in (in seconds)
  ESP.rtcUserMemoryRead(HOUR_COUNTER_RTC_LOC, &hour_counter, sizeof(hour_counter));

  // Viz
  #if VERBOSE
    Serial.printf("Wake counter: %d\n", wake_counter);
    Serial.printf("Hour n°%d of the day\n", hour_counter);
  #endif

  // If we've finished an hour, we need this flag (end_hour) for when we write the temp to the LOG
  // (checking with a tolerance of 1 minute)
  uint32_t hours = (hour_counter+1)*SEC_IN_HOUR;
  if((wake_counter - hours >= 0) && (wake_counter - hours <= 60)){
        // hour_counter goes from 0 to 23
        if(++hour_counter == 24)hour_counter = 0;
        end_hour = 1;
      }

  // It's been a day already
  if(wake_counter >= SEC_IN_DAY){
    communicate_();
    wake_counter = 0;
  }
  else wake_counter += SLEEP_COUNTER;
  // Update the values in the RTC memory
  ESP.rtcUserMemoryWrite(HOUR_COUNTER_RTC_LOC, &hour_counter, sizeof(hour_counter));
  ESP.rtcUserMemoryWrite(WAKE_COUNTER_RTC_LOC, &wake_counter, sizeof(wake_counter));
}

// At boot, the RTC memory will contain random values, so we need a way to check
// If the data we're reading is valid on not (edited by us, or random), so to do that 
// we read the 4 first bytes, and these need to be exactly RTC_VAL_PATTERN == 0x66669420 
// (any specific pattern)
void check_rtc_mem_validity(){
  uint32_t pattern = 0, reset_var = 0;
  ESP.rtcUserMemoryRead(PATTERN_RTC_LOC, &pattern, sizeof(pattern));

  // Viz
  #if VERBOSE
    Serial.printf("\nRTC Pattern: %d, Ours: %d", pattern, RTC_VAL_PATTERN);
  #endif
  
  if(pattern != RTC_VAL_PATTERN){ // Pattern is non-valid

    // Viz
    #if VERBOSE
      Serial.println("Resetting RTC patterns");
    #endif
    
    // Update the pattern to indicate that we indeed modified it
    pattern = RTC_VAL_PATTERN; 
    ESP.rtcUserMemoryWrite(PATTERN_RTC_LOC, &pattern, sizeof(pattern));

    // Reset all RTC memory savings that we care about their value to 0
    ESP.rtcUserMemoryWrite(WAKE_COUNTER_RTC_LOC, &reset_var, sizeof(reset_var)); 
    ESP.rtcUserMemoryWrite(HOUR_COUNTER_RTC_LOC, &reset_var, sizeof(reset_var));   
  }
}

float read_temperature(float& output){
  
  #if SIMULATE_RANDOM // Simulate sensor's readings with a random number generator
  
    output = random(0, 100);
  
  #else // Read the sensor values from the two sensors
    /*
     * INSERT CODE HERE
     */
  #endif
  
}


void communicate_(){

  char* buffer = (char*)malloc(sizeof(char)*JSON_BUFFER_SIZE); // Allocate memory for the buffer
  
  read_day(buffer); // Parse the formatted day's data

  MQTT_setup();

  // Viz
  #if VERBOSE 
    Serial.println("Connecting to WiFi...");
  #endif
  
  wake_wifi_up();

  // Viz
  #if VERBOSE
    Serial.println("Connecting to MQTT broker...");
  #endif
  
  MQTT_connect();
  send_data(buffer);
  
  // Viz
  #if VERBOSE
    Serial.print("Sent: ");
    Serial.println(buffer);
  #endif

  // Garbage collection
  free(buffer); 
}
