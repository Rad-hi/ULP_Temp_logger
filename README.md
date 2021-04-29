# Temperature logging IoT node

---

## Overview: 
This is an ultra low power (ULP) temperature logging IoT node based on the ESP8266 MCU, and it sends data in JSON format using MQTT.

This node achieves ULP levels thanks to these tricks:

- Use of a bare-bones 8266 since the use of development boards introduces non-necessary (for deployment) energy consumptions, hence prohibiting the ESP from reaching the 20uA sleep currents claimed by Espressif.

- Use of deep sleep to reduce power consumption as much as possible.

- Turning WiFi ON only when needed, and configuring the ESP to wake up with it OFF.

- Using a static IP address in order to save DHCP dynamic IP allocation time.

- Adopting an edge computing approach, which allows us (with simple, yet smart data processing) to reduce WiFi ON-time to only 6-8 seconds per day, and in the process, save tremendous amounts of energy; longer lasting battery.

---

## Technologies used:

I used MQTT to send the data in JSON format. 

In order to make the data logger, I used the file system of the ESP through the LittleFS library. 

Keeping track of time and successfully sending data each 24h introduced a challenge the moment I thought about deep-sleep, luckily I discovered the RTC (Real Time Clock) memory, which isn't really real-time, but it's accurate enough for temperature logging.

---

## Data provided by the node:

The node sends a JSON file each 24h (approximate, not exactly) that contains 96 values, the **Max**, **Min**, and **Mean** of temperatures for each hour, formatted in this format:

{"H0":{"mx":"Max_value","mi":"Min_value","mn":"Mean_value"},
 
 "H1":{"mx":"Max_value","mi":"Min_value","mn":"Mean_value"},

 .

 .

 .

 "H23":{"mx":"Max_value","mi":"Min_value","mn":"Mean_value"}

}

We don't actually need every temperature in a whole day (well this is what I thought) in order to get useful data, and this is why, instead of going the route of sending a value each 30 min to decrease the ON-time, I'd increase the number of data points by waking up each 1 min to read and log the value, then provide an overview of the Temp in that day. In my opinion, the power saving is worth it.

---

## Author info:

[Radhi Sghaier on LinkedIn](https://www.linkedin.com/in/radhi-sghaier/)
