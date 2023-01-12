/*
IMPORTANT NOTES https://docs.platformio.org/en/latest/faq/ino-to-cpp.html

CREATED BY SETIYADI_BEN ALIAS BENNY HARTANTO SETIYADI FROM POLITEKNIK NEGERI SEMARANG AS A PURPOSE FOR
COMPLETING THESIS AKA "TUGAS AKHIR" FOR GET BASc IN TELECOMMUNICATION ENGINEERING.

IN ADDITION, THESE SCRIPTS LIKE SENSOR READINGS, TELEGRAM BOT, TIMING, .etc ARE MODIFIED FROM ORIGINAL SOURCE
FROM GITHUB AND MENTIONED BELOW
*/

// Lib for communicating with ESP32 PlatformIO Framework
#include <Arduino.h>
/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-date-time-ntp-client-server-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <WiFi.h>
#include "time.h"

const char* ssid = "Ariaqi 3";
const char* password = "lisa2218";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 25200;
const int   daylightOffset_sec = 3600;


void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");

  // LCD 20X4 CHAR OUTPUT
  char formattedDate[7];
  strftime(formattedDate, sizeof(formattedDate), "%d %b", &timeinfo);
  Serial.println(formattedDate);

  char formattedTime[6];
  strftime(formattedTime, sizeof(formattedTime), "%H:%M", &timeinfo);
  Serial.println(formattedTime);

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println();
}

void setup(){
  Serial.begin(9600);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop(){
  delay(1000);
  printLocalTime();
}