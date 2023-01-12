#include <Arduino.h>
#include <WiFi.h>
#include <NTPClient.h>

const char* ssid = "Ariaqi 3";
const char* password = "lisa2218";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup() {
    Serial.begin(9600);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    timeClient.begin();
    // Set offset time zone (in seconds)
    timeClient.setTimeOffset(25200);
}

void loop() {
    timeClient.update();

    int day = timeClient.getDay();
    int month = timeClient.getMonth();
    int hour = timeClient.getHours();
    int minute = timeClient.getMinutes();

    String date = String(day) + " ";
    switch (month) {
        case 1: date += "Jan"; break;
        case 2: date += "Feb"; break;
        // ...
        case 12: date += "Dec"; break;
    }

    String time = String(hour) + ":" + String(minute);
    if (minute < 10) time = String(hour) + ":0" + String(minute);

    Serial.println(date);
    Serial.println(time);
    delay(1000);
}
