/*
  IMPORTANT NOTES VISIT :
  - https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
  - https://docs.platformio.org/en/latest/faq/ino-to-cpp.html
  - https://github.com/setiyadi-ben/Project-Tugas-Akhir

  CREATED BY SETIYADI_BEN ALIAS BENNY HARTANTO SETIYADI FROM POLITEKNIK NEGERI SEMARANG AS A PURPOSE FOR
  COMPLETING THESIS AKA "TUGAS AKHIR" FOR GET BASc IN TELECOMMUNICATION ENGINEERING.

  IN ADDITION, THESE SCRIPTS LIKE SENSOR READINGS, TELEGRAM BOT, TIMING, .etc ARE MODIFIED FROM ORIGINAL SOURCE
  FROM GITHUB AND MENTIONED BELOW

  BIG THANKS TO :
  ( ALLAH, Ari Sriyanto N., S.T., M.T., M.Sc as DOSEN PEMBIMBING I & SINDUNG H.W.S., B.S.E.E, M.Eng.Sc. as II
  + Tahan Prahara., ST., M.Kom., Ngadiyono as a Section Head of Maggot BSF Semarang	(https://tlkm.id/RF0sFJ),
  CHATGPT, STACKOVERFLOW, YOUTUBE, [ ANIME, CIGARETTE & STEAM GAMES for releasing my stress ])
*/

// Lib for communicating with ESP32 PlatformIO Framework
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// RTC library
/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-date-time-ntp-client-server-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
#include <NTPClient.h>
#include "time.h"
#include <TimeLib.h>
const long gmtOffset_sec = 25200; // GMT+7 timezone
const int daylightOffset_sec = 3600;
const char *ntpServer = "time.google.com";
const int ntpPort = 123;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset_sec, daylightOffset_sec);
// void beginUDP() {
//   ntpUDP.begin(ntpPort);
// }

// NTPClient timeClient(ntpUDP, ntpServer, gmtOffset_sec, daylightOffset_sec);

// Wifi network station credentials
#define WIFI_SSID "Ariaqi 3"
#define WIFI_PASSWORD "lisa2218"
// #define WIFI_SSID "Redmi 7"
// #define WIFI_PASSWORD "11111111"

// Telegram BOT Token (Get from Botfather)
/*
  More details at : https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
  The MIT License

  Copyright (c) 2016-2020 Brian Lough brian.d.lough@gmail.com

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#define BOT_TOKEN "5935516261:AAE6L6-DRUcqIiR7MUOmNQwtCm_LToXObn0"
// Using CHAT_ID to lock user that can access your bot outside the Group,
// get the id on @RawDataBot
#define CHAT_ID "-1001825459630"
const unsigned long BOT_MTBS = 1000; // mean time between scan messages
unsigned long bot_lasttime;          // last time messages' scan has been done
WiFiClientSecure secured_client;
// Cert using to perform SSL connection to api.telegram.org update if it expires
const char rootCA[] = R"=EOF=(
-----BEGIN CERTIFICATE-----
MIIGkDCCBXigAwIBAgIISiBgk2h4nDYwDQYJKoZIhvcNAQELBQAwgbQxCzAJBgNV
BAYTAlVTMRAwDgYDVQQIEwdBcml6b25hMRMwEQYDVQQHEwpTY290dHNkYWxlMRow
GAYDVQQKExFHb0RhZGR5LmNvbSwgSW5jLjEtMCsGA1UECxMkaHR0cDovL2NlcnRz
LmdvZGFkZHkuY29tL3JlcG9zaXRvcnkvMTMwMQYDVQQDEypHbyBEYWRkeSBTZWN1
cmUgQ2VydGlmaWNhdGUgQXV0aG9yaXR5IC0gRzIwHhcNMjIwODEwMTU1NjI4WhcN
MjMwOTExMTU1NjI4WjAZMRcwFQYDVQQDDA4qLnRlbGVncmFtLm9yZzCCASIwDQYJ
KoZIhvcNAQEBBQADggEPADCCAQoCggEBAKpIH/5PVT0RqF5iuaLJdPSSPQ/Zv4bb
3vRlAAN99rtR9/diw4JV2QsnVmReHY111XoEjUeKhjuB+ETlh/VGO/NXNPcPn/J2
LCtyMvOM1SDkwAJ2RGzN6Tj1Un9QHdtZ5yQqTk34p/dp4TnjkbcFGdZxNt2fXS68
FmE0j5KqmO4ygBcwymwzJr10750H9o9E/d2Q5q4/9fHEYSyTLJEbiuSb/3QI3Wq8
rjABm/OfINdik3Uo1tcKO3ROEgNEWdP581pBHvqDgCjLfjt6OFDDyrttUHVhIATm
1EsHIhqG5VUbSbLJlsP3PQAnngqbILMw6DyvfLm2sc9vyQXDyZfYBe0CAwEAAaOC
Az4wggM6MAwGA1UdEwEB/wQCMAAwHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUF
BwMCMA4GA1UdDwEB/wQEAwIFoDA4BgNVHR8EMTAvMC2gK6AphidodHRwOi8vY3Js
LmdvZGFkZHkuY29tL2dkaWcyczEtNDM2Ni5jcmwwXQYDVR0gBFYwVDBIBgtghkgB
hv1tAQcXATA5MDcGCCsGAQUFBwIBFitodHRwOi8vY2VydGlmaWNhdGVzLmdvZGFk
ZHkuY29tL3JlcG9zaXRvcnkvMAgGBmeBDAECATB2BggrBgEFBQcBAQRqMGgwJAYI
KwYBBQUHMAGGGGh0dHA6Ly9vY3NwLmdvZGFkZHkuY29tLzBABggrBgEFBQcwAoY0
aHR0cDovL2NlcnRpZmljYXRlcy5nb2RhZGR5LmNvbS9yZXBvc2l0b3J5L2dkaWcy
LmNydDAfBgNVHSMEGDAWgBRAwr0njsw0gzCiM9f7bLPwtCyAzjAnBgNVHREEIDAe
gg4qLnRlbGVncmFtLm9yZ4IMdGVsZWdyYW0ub3JnMB0GA1UdDgQWBBRqvbbCZElv
0pbnIM/DHudY2bUlfjCCAX8GCisGAQQB1nkCBAIEggFvBIIBawFpAHYA6D7Q2j71
BjUy51covIlryQPTy9ERa+zraeF3fW0GvW4AAAGCiHjKNwAABAMARzBFAiEAm5J/
LyrHaw+COLjPjSay0jgKCQRJ6zjYJhbvJt0aDcACIFka+9S/RP0xJPxT/TJ90qDv
2jNyWtJt/+Cz75j2ImUGAHYANc8ZG7+xbFe/D61MbULLu7YnICZR6j/hKu+oA8M7
1kwAAAGCiHjL0QAABAMARzBFAiA2KJz6YIAsKEoezBC8c1ENpEAPrurLk8xJlFjt
Vms8IwIhAMCI1cxKpfqzlqzA4Wu4Du16pqP4U1Y4n5utSkwAe2lGAHcAejKMVNi3
LbYg6jjgUh7phBZwMhOFTTvSK8E6V6NS61IAAAGCiHjMhgAABAMASDBGAiEA+TOM
1gB0ORXEr1UA04LGc7jv5iTR9tIBnTytaPgj/gICIQCgQnG0/e3TaAlw+/lFbgkG
PLf8Wd2KiM+afOWTFjHJxDANBgkqhkiG9w0BAQsFAAOCAQEAfobLSLYPbUyFJzyM
FH30eAiqI/TwNZ2p5b/IDiznAIBW8iv4VdWJTh/m61qR/RSThHEcybxg5oBg+I45
2EdI3/Tc0hKakcOkOw1Vy2ulT6TpM+oMEq0acIpi7LKe8J59D8EXmmvPHGoHUu/5
+4skGktbS6xvFtSdnG5+tzQt3CO+KaCQ5o2arYOfBn0cZfzGUWkJjYqeUn/bkcnd
ftP9KDjnUps+wOXs01g5cUcMv9+fx3bWw62R3+E97yHOt9cfcVVTfEOC9ji75q/2
su6HY2jFAmRGHdZDPcsMYW6pl24vYxzYzrvAD3GH9on6ah9JTSLW2cHRzDPfUIWA
06O1IQ==
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIE0DCCA7igAwIBAgIBBzANBgkqhkiG9w0BAQsFADCBgzELMAkGA1UEBhMCVVMx
EDAOBgNVBAgTB0FyaXpvbmExEzARBgNVBAcTClNjb3R0c2RhbGUxGjAYBgNVBAoT
EUdvRGFkZHkuY29tLCBJbmMuMTEwLwYDVQQDEyhHbyBEYWRkeSBSb290IENlcnRp
ZmljYXRlIEF1dGhvcml0eSAtIEcyMB4XDTExMDUwMzA3MDAwMFoXDTMxMDUwMzA3
MDAwMFowgbQxCzAJBgNVBAYTAlVTMRAwDgYDVQQIEwdBcml6b25hMRMwEQYDVQQH
EwpTY290dHNkYWxlMRowGAYDVQQKExFHb0RhZGR5LmNvbSwgSW5jLjEtMCsGA1UE
CxMkaHR0cDovL2NlcnRzLmdvZGFkZHkuY29tL3JlcG9zaXRvcnkvMTMwMQYDVQQD
EypHbyBEYWRkeSBTZWN1cmUgQ2VydGlmaWNhdGUgQXV0aG9yaXR5IC0gRzIwggEi
MA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC54MsQ1K92vdSTYuswZLiBCGzD
BNliF44v/z5lz4/OYuY8UhzaFkVLVat4a2ODYpDOD2lsmcgaFItMzEUz6ojcnqOv
K/6AYZ15V8TPLvQ/MDxdR/yaFrzDN5ZBUY4RS1T4KL7QjL7wMDge87Am+GZHY23e
cSZHjzhHU9FGHbTj3ADqRay9vHHZqm8A29vNMDp5T19MR/gd71vCxJ1gO7GyQ5HY
pDNO6rPWJ0+tJYqlxvTV0KaudAVkV4i1RFXULSo6Pvi4vekyCgKUZMQWOlDxSq7n
eTOvDCAHf+jfBDnCaQJsY1L6d8EbyHSHyLmTGFBUNUtpTrw700kuH9zB0lL7AgMB
AAGjggEaMIIBFjAPBgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBBjAdBgNV
HQ4EFgQUQMK9J47MNIMwojPX+2yz8LQsgM4wHwYDVR0jBBgwFoAUOpqFBxBnKLbv
9r0FQW4gwZTaD94wNAYIKwYBBQUHAQEEKDAmMCQGCCsGAQUFBzABhhhodHRwOi8v
b2NzcC5nb2RhZGR5LmNvbS8wNQYDVR0fBC4wLDAqoCigJoYkaHR0cDovL2NybC5n
b2RhZGR5LmNvbS9nZHJvb3QtZzIuY3JsMEYGA1UdIAQ/MD0wOwYEVR0gADAzMDEG
CCsGAQUFBwIBFiVodHRwczovL2NlcnRzLmdvZGFkZHkuY29tL3JlcG9zaXRvcnkv
MA0GCSqGSIb3DQEBCwUAA4IBAQAIfmyTEMg4uJapkEv/oV9PBO9sPpyIBslQj6Zz
91cxG7685C/b+LrTW+C05+Z5Yg4MotdqY3MxtfWoSKQ7CC2iXZDXtHwlTxFWMMS2
RJ17LJ3lXubvDGGqv+QqG+6EnriDfcFDzkSnE3ANkR/0yBOtg2DZ2HKocyQetawi
DsoXiWJYRBuriSUBAA/NxBti21G00w9RKpv0vHP8ds42pM3Z2Czqrpv1KrKQ0U11
GIo/ikGQI31bS/6kA1ibRrLDYGCD+H1QQc7CoZDDu+8CL9IVVO5EFdkKrqeKM+2x
LXY2JtwE65/3YR8V3Idv7kaWKK2hJn0KCacuBKONvPi8BDAB
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIDxTCCAq2gAwIBAgIBADANBgkqhkiG9w0BAQsFADCBgzELMAkGA1UEBhMCVVMx
EDAOBgNVBAgTB0FyaXpvbmExEzARBgNVBAcTClNjb3R0c2RhbGUxGjAYBgNVBAoT
EUdvRGFkZHkuY29tLCBJbmMuMTEwLwYDVQQDEyhHbyBEYWRkeSBSb290IENlcnRp
ZmljYXRlIEF1dGhvcml0eSAtIEcyMB4XDTA5MDkwMTAwMDAwMFoXDTM3MTIzMTIz
NTk1OVowgYMxCzAJBgNVBAYTAlVTMRAwDgYDVQQIEwdBcml6b25hMRMwEQYDVQQH
EwpTY290dHNkYWxlMRowGAYDVQQKExFHb0RhZGR5LmNvbSwgSW5jLjExMC8GA1UE
AxMoR28gRGFkZHkgUm9vdCBDZXJ0aWZpY2F0ZSBBdXRob3JpdHkgLSBHMjCCASIw
DQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAL9xYgjx+lk09xvJGKP3gElY6SKD
E6bFIEMBO4Tx5oVJnyfq9oQbTqC023CYxzIBsQU+B07u9PpPL1kwIuerGVZr4oAH
/PMWdYA5UXvl+TW2dE6pjYIT5LY/qQOD+qK+ihVqf94Lw7YZFAXK6sOoBJQ7Rnwy
DfMAZiLIjWltNowRGLfTshxgtDj6AozO091GB94KPutdfMh8+7ArU6SSYmlRJQVh
GkSBjCypQ5Yj36w6gZoOKcUcqeldHraenjAKOc7xiID7S13MMuyFYkMlNAJWJwGR
tDtwKj9useiciAF9n9T521NtYJ2/LOdYq7hfRvzOxBsDPAnrSTFcaUaz4EcCAwEA
AaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYwHQYDVR0OBBYE
FDqahQcQZyi27/a9BUFuIMGU2g/eMA0GCSqGSIb3DQEBCwUAA4IBAQCZ21151fmX
WWcDYfF+OwYxdS2hII5PZYe096acvNjpL9DbWu7PdIxztDhC2gV7+AJ1uP2lsdeu
9tfeE8tTEH6KRtGX+rcuKxGrkLAngPnon1rpN5+r5N9ss4UXnT3ZJE95kTXWXwTr
gIOrmgIttRD02JDHBHNA7XIloKmf7J6raBKZV8aPEjoJpL1E/QYVN8Gb5DKj7Tjo
2GTzLH4U/ALqn83/B2gX2yKQOC16jdFU8WnjXzPKej17CuPKf1855eJ1usV2GDPO
LPAvTK33sefOT6jEm0pUBsV/fdUID+Ic/n4XuKxe9tQWskMJDE32p2u0mYRlynqI
4uJEvlz36hz1
-----END CERTIFICATE-----
)=EOF=";
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

// Import lib sensor DHT11
/*
  Example testing sketch for various DHT humidity/temperature sensors
  Written by ladyada, public domain

  REQUIRES the following Arduino libraries:
  - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
  - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
*/
#include <DHT.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 4 // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.
// Uncomment whatever type you're using!
#define DHTTYPE DHT11 // DHT 11
// #define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
// #define DHTTYPE DHT21   // DHT 21 (AM2301)
/*
// Interfacing DHT Sensor
- Connect pin 1 (on the left) of the sensor to +5V
- NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
  to 3.3V instead of 5V!
- Connect pin 2 of the sensor to whatever your DHTPIN is
- Connect pin 4 (on the right) of the sensor to GROUND
- Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
- Note that older versions of this library took an optional third parameter to
  tweak the timings for faster processors.  This parameter is no longer needed
  as the current DHT reading algorithm adjusts itself to work on faster procs.
*/
DHT dht(DHTPIN, DHTTYPE);

// Import lib sensor BH1750 and LCD 20x4
/*
  Rui Santos
  Complete project details at https://randomnerdtutorials.com/esp32-bh1750-ambient-light-sensor/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
#include <Wire.h> // Lanes that used for communication BH1750 and LCD 20x4
#include <BH1750.h>
// Initialize BH1750 sensor.
BH1750 lightMeter(0x23); // Address 0x23 because it's sharing lanes with LCD 20x4

// Import lib for LCD 20x4
#include <LiquidCrystal_I2C.h>
// Set up the LCD display
LiquidCrystal_I2C lcd(0x27, 20, 4); // Address 0x27, 20 columns, 4 rows

// Relay setup configuration
/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-relay-module-ac-web-server/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
// relay pinout
const int relay1_waterPump = 18;
const int relay2_lampuFertilizer = 19;
// unused relay
#define relay3_solenoidValve 20

// Manual switch from GPIO pin
const int switch_pin = 5;
#define switch_waterPump 13
#define switch_lampuFertilizer 12
// #define switch_lampuFertilizer 12

// relay default state (my relay configuration is Normally Closed)
bool state1_waterPump = LOW;
bool state2_lampuFertilizer = LOW;
bool state3_solenoidValve = LOW;

unsigned long sensorStartTime;
unsigned long sensorDelay = 1250; // delay for 1250 milliseconds

unsigned long previousMillis = 0; // variable to store the last time the LCD was updated
const long interval = 1000;       // interval at which to update the LCD (1000ms = 1 second)

// test state using builtin led esp32
const int ledPin = 2;
// bool ledState = LOW;

// void printLocalTime()
// {
//   struct tm timeinfo;
//   if (!getLocalTime(&timeinfo))
//   {
//     Serial.println("Failed to obtain time");
//     return;
//   }
// }

void bot_setup()
{
  const String commands = F("["
                            /*
                            Command format is customizeable with combination of lowercase and "_". Except that, it will not shown
                            in telegram bot command "/" ex: "{\"command\":\"sensorVal\",  \"description\":\"Print nilai output sensor\"},"
                            */
                            "{\"command\":\"help\",  \"description\":\"Panduan penggunaan bot\"},"
                            // Print sensor reading from DHT11, BH1750 and actuator state
                            "{\"command\":\"print\", \"description\":\"Output status dari esp32\"},"
                            // Manual Switch On / Off actuator
                            "{\"command\":\"switch\", \"description\":\"Saklar digital aktuator scr manual\"},"
                            // Auto setup with provided timeInput data
                            "{\"command\":\"schedule\", \"description\":\"Penjadwalan otomatis aktuator\"}" // no comma on last command
                            // Setmode for actuators to work auto with timeInput or manually
                            //"{\"command\":\"setmode\", \"description\":\"Setup esp32 manual atau auto\"},"
                            // "{\"command\":\"led_on\", \"description\":\"turn led on\"},"
                            // "{\"command\":\"led_off\", \"description\":\"turn led off\"}" // no comma on last command
                            "]");
  bot.setMyCommands(commands);
  // bot.sendMessage("25235518", "Hola amigo!", "Markdown");
}

// void handleNewMessages(int numNewMessages)
// {
//   Serial.print("handleNewMessages ");
//   Serial.println(numNewMessages);

//   for (int i = 0; i < numNewMessages; i++)
//   {
//     // Chat id of the requester
//     String chat_id = String(bot.messages[i].chat_id);
//     if (chat_id != CHAT_ID)
//     {
//       bot.sendMessage(chat_id, "Unauthorized user", "");
//       return;
//     }
//     // Get all the important data from the message
//     int message_id = bot.messages[i].message_id;
//     // String chat_id = String(bot.messages[i].chat_id);
//     String text = bot.messages[i].text;
//     String from_name = bot.messages[i].from_name;
//     if (from_name == "")
//       from_name = "Guest";
//     String msg = ""; // init a message string to use

//     // Output the message_id to give you feeling on how this example works
//     Serial.print("Message id: ");
//     Serial.println(message_id);

//     // Inline buttons with callbacks when pressed will raise a callback_query message
//     if (bot.messages[i].type == "callback_query")
//     {
//       Serial.print("Call back button pressed by: ");
//       Serial.println(bot.messages[i].from_id);
//       Serial.print("Data on the button: ");
//       Serial.println(bot.messages[i].text);
//     }
//   }
// }

void setup()
{
  Serial.begin(9600);
  // Initialize dht11 lib
  dht.begin();
  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  // For Wemos / Lolin D1 Mini Pro and the Ambient Light shield use Wire.begin(D2, D1);
  // Initialize bh1750 lib
  lightMeter.begin();
  Serial.println();
  // Initialize the LCD display
  lcd.begin(20, 4);
  lcd.backlight();

  // define builtin led
  pinMode(ledPin, OUTPUT);
  // digitalWrite(ledPin, ledState);

  // Setup Relay Pin as OUTPUT
  pinMode(relay1_waterPump, OUTPUT);
  pinMode(relay2_lampuFertilizer, OUTPUT);
  pinMode(relay3_solenoidValve, OUTPUT);

  // Setup Manual GPIO Switch
  pinMode(switch_pin, INPUT_PULLUP);
  pinMode(switch_waterPump, INPUT_PULLUP);
  pinMode(switch_lampuFertilizer, INPUT_PULLUP);

  // Defining States to set on boot
  digitalWrite(switch_pin, LOW);
  digitalWrite(relay1_waterPump, state1_waterPump);
  digitalWrite(relay2_lampuFertilizer, state2_lampuFertilizer);

  // attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected.");
  Serial.print("\nIP address: ");
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  secured_client.setCACert(rootCA);
  // secured_client.getPeerCertificate();
  // secured_client.setInsecure();

  // Telegram Bot Setup from Library
  bot_setup();
  // handleNewMessages();

  // Initialize and synchronize time with NTP
  ntpUDP.begin(ntpPort);
  timeClient.begin();
  timeClient.setUpdateInterval(1000);
  while (!timeClient.update())
  {
    timeClient.forceUpdate();
  }

  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  // printLocalTime();
}

// Defining time schedule for an actuator to work
bool scheduleEnabled = false;
bool scheduleEnabled2 = false; // for demo testing only
void loop()
{
  // Get current time
  timeClient.update();
  time_t now = timeClient.getEpochTime();
  tm timeinfo = *localtime(&now); // Note that the localtime() function is used to convert the epoch time

  //  Control & Monitoring Switch using Telegram Bot
  if (millis() - bot_lasttime > BOT_MTBS)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    for (int i = 0; i < numNewMessages; i++)
    {
      // String chat_id = String(bot.messages[i].chat_id);
      // String text = bot.messages[i].text;

      // // Get all the important data from the message
      // int message_id = bot.messages[i].message_id;
      // String from_name = bot.messages[i].from_name;
      // if (from_name == "")
      //   from_name = "Guest";
      // String msg = ""; // init a message string to use

      // Chat id of the requester
      String chat_id = String(bot.messages[i].chat_id);
      if (chat_id != CHAT_ID)
      {
        bot.sendMessage(chat_id, "Unauthorized user", "");
        return;
      }
      // Get all the important data from the message
      int message_id = bot.messages[i].message_id;
      // String chat_id = String(bot.messages[i].chat_id);
      String text = bot.messages[i].text;
      String from_name = bot.messages[i].from_name;
      if (from_name == "")
        from_name = "Guest";
      String msg = ""; // init a message string to use

      // Output the message_id to give you feeling on how this example works
      Serial.print("Message id: ");
      Serial.println(message_id);

      // Inline buttons with callbacks when pressed will raise a callback_query message
      if (bot.messages[i].type == "callback_query")
      {
        Serial.print("Call back button pressed by: ");
        Serial.println(bot.messages[i].from_id);
        Serial.print("Data on the button: ");
        Serial.println(bot.messages[i].text);
      }

      if (text == "/switch@bsfcontrol_bot")
      {
        // lets create a friendly welcome message
        // msg = "Hi " + from_name + "!\n";
        msg = "Berikut merupakan saklar digital untuk pompa dan lampu.\n\n";
        msg += "Silahkan tekan tombol dibawah untuk mengendalikan kondisi perangkat.\n\n";

        String keyboardJson = "["; // start Json
        // updateInlineKeyboard for waterPump
        keyboardJson += "[{ \"text\" : \"waterpump is ";
        keyboardJson += (state1_waterPump ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/switchButton1\" }]";
        // updateInlineKeyboard for lampuFertilizer
        keyboardJson += ", [{ \"text\" : \"lampufertilizer is ";
        keyboardJson += (state2_lampuFertilizer ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/switchButton2\" }]";
        // keyboardJson += ", [{ \"text\" : \"Send message\", \"callback_data\" : \"/sendMessage\" }]";
        // keyboardJson += ", [{ \"text\" : \"Go to Google\", \"url\" : \"https://www.google.com\" }]";
        keyboardJson += "]"; // end Json

        // first time, send this message as a normal inline keyboard message:
        bot.sendMessageWithInlineKeyboard(chat_id, msg, "Markdown", keyboardJson);
      }

      if (text == "/schedule@bsfcontrol_bot")
      {
        // msg header
        msg = "Berikut merupakan saklar digital dengan penjadwalan secara otomatis.\n\n";
        msg += "Saklar beroperasi dengan parameter waktu dan intensitas cahaya yang ditentukan.\n\n";

        // Prepare the buttons
        String keyboardJson = "["; // start Json
        // updateInlineKeyboard
        keyboardJson += "[{ \"text\" : \"08 AM to 01 PM is ";
        keyboardJson += (scheduleEnabled ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/scheduleButton\" }]";
        // updateInlineKeyboard
        keyboardJson += ", [{ \"text\" : \"schedule example is ";
        keyboardJson += (scheduleEnabled2 ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/scheduleButton2\" }]";
        keyboardJson += "]"; // end Json

        // first time, send this message as a normal inline keyboard message:
        bot.sendMessageWithInlineKeyboard(chat_id, msg, "Markdown", keyboardJson);
      }

      // Reading temperature or humidity takes about 250 milliseconds!
      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
      // Read temperature as Celsius (the default)
      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();
      // Read temperature as Fahrenheit (isFahrenheit = true)
      // float f = dht.readTemperature(true);
      // Compute value of light intensity
      float lux = lightMeter.readLightLevel();
      // Define the lowState and highState variables
      String lowState = "idle";
      String highState = "running";

      // Get the state of the waterPump and lampuFertilizer variables as a string
      String waterPumpState = state1_waterPump == LOW ? lowState : highState;
      String lampuFertilizerState = state2_lampuFertilizer == LOW ? lowState : highState;

      String answer;
      for (int i = 0; i < numNewMessages; i++)
      {
        telegramMessage &msg = bot.messages[i];
        Serial.println("Received " + msg.text);
        // add @ to mention the bot in command message
        // if (msg.text == "/setup@bsfcontrol_bot")
        //   answer = "===========================";
        if (msg.text == "/help@bsfcontrol_bot")
          answer = "Berikut fungsi dari setiap _perintah_ yang tersedia:"
                   "\n"
                   "- _/help_ Menu bantuan perintah telegram bot."
                   "\n"
                   "- _/start_ Menginisialisasi perangkat."
                   "\n"
                   "- _/print_ Mencetak output status aktuator dan pembacaan sensor dari ESP32."
                   "\n"
                   "- _/switch_ Saklar digital untuk pompa dan lampu."
                   "\n"
                   "- _/schedule_ Saklar digital dengan waktu penjadwalan otomatis."
                   "\n";
        // if (msg.text == "/switch@bsfcontrol_bot")
        //   answer = "----------------------------";
        if (msg.text == "/start@bsfcontrol_bot")
          answer = "Selamat Datang *" + msg.from_name + "*, bot online dan siap digunakan.";
        if (msg.text == "/print@bsfcontrol_bot")
        {

          // Use the state variable in the answer variable
          answer = "Berikut merupakan data monitoring dari hasil perhitungan :"
                   "\n"
                   "- Temp : " +
                   String(temperature) + "°C\n" +
                   "- Humi : " + String(humidity) + "%\n" +
                   "- Lux : " + String(lux) + "lx\n" +
                   "- waterPump : " + waterPumpState + "\n" +
                   "- lampuFertilizer : " + lampuFertilizerState + "\n";
        }
        // example
        // if (msg.text == "/led_on@bsfcontrol_bot")
        // {
        //   answer = "LED state set to ON";
        //   ledState = HIGH;
        //   digitalWrite(ledPin, ledState);
        // }
        // if (msg.text == "/led_off@bsfcontrol_bot")
        // {
        //   answer = "LED state set to OFF";
        //   ledState = LOW;
        //   digitalWrite(ledPin, ledState);
        // }

        // else
        //   answer = "Say what?";

        bot.sendMessage(msg.chat_id, answer, "Markdown");
      }

      // CALLBACK MANUAL SWITCH BUTTON
      if (text == "/switchButton1")
      {
        // Toggle the ledState and update the LED itself
        // ledState = !ledState;
        // digitalWrite(ledPin, ledState);

        state1_waterPump = !state1_waterPump;
        digitalWrite(relay1_waterPump, state1_waterPump);

        // state2_lampuFertilizer = !state2_lampuFertilizer;
        // digitalWrite(relay2_lampuFertilizer, state2_lampuFertilizer);

        // Now we can UPDATE the message, lets prepare it for sending:
        msg = "Halo " + from_name + ", perintah berhasil dijalankan.\n";
        msg += "Jika gagal, silahkan tekan tombol reset pada panel box.\n\n";
        // msg += "Try it again, see the button has updated as well:\n\n";

        // Prepare the buttons
        String keyboardJson = "["; // start Json
        // updateInlineKeyboard for waterPump
        keyboardJson += "[{ \"text\" : \"waterpump is ";
        keyboardJson += (state1_waterPump ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/switchButton1\" }]";
        // updateInlineKeyboard for lampuFertilizer
        keyboardJson += ", [{ \"text\" : \"lampufertilizer is ";
        keyboardJson += (state2_lampuFertilizer ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/switchButton2\" }]";
        // keyboardJson += ", [{ \"text\" : \"Send message\", \"callback_data\" : \"/sendMessage\" }]";
        // keyboardJson += ", [{ \"text\" : \"Go to Google\", \"url\" : \"https://www.google.com\" }]";
        keyboardJson += "]"; // end Json

        // Now send this message including the current message_id as the 5th input to UPDATE that message
        bot.sendMessageWithInlineKeyboard(chat_id, msg, "Markdown", keyboardJson, message_id);
      }

      if (text == "/switchButton2")
      {
        // Toggle the ledState and update the LED itself
        // ledState = !ledState;
        // digitalWrite(ledPin, ledState);
        // state1_waterPump = !state1_waterPump;
        // digitalWrite(relay1_waterPump, state1_waterPump);

        state2_lampuFertilizer = !state2_lampuFertilizer;
        digitalWrite(relay2_lampuFertilizer, state2_lampuFertilizer);

        // Now we can UPDATE the message, lets prepare it for sending:
        msg = "Halo " + from_name + ", perintah berhasil dijalankan.\n";
        msg += "Jika gagal, silahkan tekan tombol reset pada panel box.\n\n";
        // msg += "Try it again, see the button has updated as well:\n\n";

        // Prepare the buttons
        String keyboardJson = "["; // start Json
        // updateInlineKeyboard for waterPump
        keyboardJson += "[{ \"text\" : \"waterpump is ";
        keyboardJson += (state1_waterPump ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/switchButton1\" }]";
        // updateInlineKeyboard for lampuFertilizer
        keyboardJson += ", [{ \"text\" : \"lampufertilizer is ";
        keyboardJson += (state2_lampuFertilizer ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/switchButton2\" }]";
        // keyboardJson += ", [{ \"text\" : \"Send message\", \"callback_data\" : \"/sendMessage\" }]";
        // keyboardJson += ", [{ \"text\" : \"Go to Google\", \"url\" : \"https://www.google.com\" }]";
        keyboardJson += "]"; // end Json

        // Now send this message including the current message_id as the 5th input to UPDATE that message
        bot.sendMessageWithInlineKeyboard(chat_id, msg, "Markdown", keyboardJson, message_id);
      }

      // Check if the message is a custom command to change the state
      // if (text == "/scheduleButton2")
      // {
      //   scheduleEnabled2 = true;
      //   bot.sendMessage(chat_id, "Schedule 2 activated.");
      // }
      // else
      // {
      //   bot.sendMessage(chat_id, "Schedule 2 deactivated.");
      // }
      // Add other commands here as needed

      // CALLBACK SCHEDULED BUTTON
      if (text == "/scheduleButton")
      {
        // Togle TRUE & FALSE statements
        scheduleEnabled = !scheduleEnabled;

        // Now we can UPDATE the message, lets prepare it for sending:
        msg = "Halo " + from_name + ", perintah berhasil dijalankan.\n";
        msg += "Jika gagal, silahkan tekan tombol reset pada panel box.\n\n";
        // msg += "Try it again, see the button has updated as well:\n\n";

        // Prepare the buttons
        String keyboardJson = "["; // start Json
        // updateInlineKeyboard
        keyboardJson += "[{ \"text\" : \"08 AM to 01 PM is ";
        keyboardJson += (scheduleEnabled ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/scheduleButton\" }]";
        // updateInlineKeyboard
        keyboardJson += ", [{ \"text\" : \"schedule example is ";
        keyboardJson += (scheduleEnabled2 ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/scheduleButton2\" }]";
        keyboardJson += "]"; // end Json

        // Now send this message including the current message_id as the 5th input to UPDATE that message
        bot.sendMessageWithInlineKeyboard(chat_id, msg, "Markdown", keyboardJson, message_id);
      }

      if (text == "/scheduleButton2")
      {
        // Togle TRUE & FALSE statements
        scheduleEnabled2 = !scheduleEnabled2;
        Serial.println(scheduleEnabled2);

        // Now we can UPDATE the message, lets prepare it for sending:
        msg = "Halo " + from_name + ", perintah berhasil dijalankan.\n";
        msg += "Jika gagal, silahkan tekan tombol reset pada panel box.\n\n";
        // msg += "Try it again, see the button has updated as well:\n\n";

        // Prepare the buttons
        String keyboardJson = "["; // start Json
        // updateInlineKeyboard
        keyboardJson += "[{ \"text\" : \"08 AM to 01 PM is ";
        keyboardJson += (scheduleEnabled ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/scheduleButton\" }]";
        // updateInlineKeyboard
        keyboardJson += ", [{ \"text\" : \"schedule example is ";
        keyboardJson += (scheduleEnabled2 ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/scheduleButton2\" }]";
        keyboardJson += "]"; // end Json

        // Now send this message including the current message_id as the 5th input to UPDATE that message
        bot.sendMessageWithInlineKeyboard(chat_id, msg, "Markdown", keyboardJson, message_id);
      }
    }

    while (numNewMessages)
    {
      Serial.println("got response");
      // handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }

  unsigned long waterPumpStart = 0;
  unsigned long waterPumpDuration = 15000;
  unsigned long waterPumpInterval = 45000; // Interval between water pump state changes in milliseconds
  unsigned long lastWaterPumpChangeTime = 0;
  bool lastScheduleEnabled2 = false;
  bool waterPumpOn = false; // flag to keep track of water pump state
  bool flagBtn = false;
  unsigned long waterPumpOnTime = 0;  // counter for the duration of water pump being on
  unsigned long waterPumpOffTime = 0; // counter for the duration of water pump being off

  if (scheduleEnabled2 != lastScheduleEnabled2)
  {
    lastScheduleEnabled2 = scheduleEnabled2;

    if (digitalRead(switch_pin) == HIGH)
    {
      // Set flag to true if switch_pin is HIGH
      flagBtn = true;
    }
    else
    {
      flagBtn = false;
    }

    if (flagBtn && scheduleEnabled2 && hour(now) >= 8 && hour(now) < 24)
    {
      // Debugging for an error
      Serial.println(flagBtn);
      Serial.println(scheduleEnabled2);
      Serial.println(hour(now));
      Serial.println(switch_pin);
      // Serial.println(timeinfo.tm_hour);
      // BH1750 reading affected by lux value to toggle lampuFertilizer
      float lux = lightMeter.readLightLevel();
      if (lux < 50)
      {
        digitalWrite(relay2_lampuFertilizer, HIGH);
        state2_lampuFertilizer = true;
        Serial.println("lampuFertilizer turned on (Scheduler).");
      }
      else
      {
        digitalWrite(relay2_lampuFertilizer, LOW);
        state2_lampuFertilizer = false;
        Serial.println("lampuFertilizer turned off (Scheduler).");
      }

      // Check if it's time to turn on the water pump
      if (millis() - lastWaterPumpChangeTime >= waterPumpInterval)
      {
        if (waterPumpOn && millis() - waterPumpStart >= waterPumpDuration)
        {
          // Turn off water pump if it has been on for more than 15 seconds
          digitalWrite(relay1_waterPump, LOW);
          state1_waterPump = false;
          Serial.println("waterPump turned off (Scheduler).");
          lastWaterPumpChangeTime = millis(); // Update the lastWaterPumpChangeTime variable
          waterPumpOn = false;
          waterPumpOffTime = 0; // reset the waterPumpOffTime counter
        }
        else if (!waterPumpOn)
        {
          // Turn on water pump if it has been off for more than 15 seconds
          digitalWrite(relay1_waterPump, HIGH);
          state1_waterPump = true;
          Serial.println("waterPump turned on (Scheduler).");
          lastWaterPumpChangeTime = millis(); // Update the lastWaterPumpChangeTime variable
          waterPumpOn = true;
          waterPumpStart = millis();
          waterPumpOnTime = 0; // reset the waterPumpOnTime counter
        }
      }

      // Check if the water pump has been running for the duration
      if (waterPumpOn && waterPumpOnTime < waterPumpDuration)
      {
        waterPumpOnTime = millis() - waterPumpStart;
      }
      else if (waterPumpOn && waterPumpOnTime >= waterPumpDuration)
      {
        digitalWrite(relay1_waterPump, LOW);
        state1_waterPump = false;
        Serial.println("waterPump turned off (Scheduler).");
        lastWaterPumpChangeTime = millis(); // Update the lastWaterPumpChangeTime variable
        waterPumpOn = false;
        waterPumpOffTime = 0; // reset the waterPumpOffTime counter
      }

      // Check if the water pump has been off for the duration
      if (millis() - lastWaterPumpChangeTime >= waterPumpInterval && !waterPumpOn)
      {
        // It's time to turn on the water pump again
        waterPumpStart = millis();
        digitalWrite(relay1_waterPump, HIGH);
        state1_waterPump = true;
        Serial.println("waterPump turned on (Scheduler).");
        Serial.println(waterPumpOn);
        // Serial.println(lastWaterPumpChangeTime);
        // Serial.println(waterPumpInterval);
        waterPumpOn = true;
      }
    }
  }

  // Control Manual GPIO Switch
  bool flag = false;
  bool prevSwitchState = digitalRead(switch_pin);
  if (prevSwitchState == LOW)
  {
    flag = true;
    if (digitalRead(switch_waterPump) == LOW)
    {
      digitalWrite(relay1_waterPump, HIGH);
      state1_waterPump = true;
      Serial.println("waterPump turned on (Manual).");
    }
    else
    {
      digitalWrite(relay1_waterPump, LOW);
      state1_waterPump = false;
      Serial.println("waterPump turned off (Manual).");
    }
    // LampuFertilizer
    if (digitalRead(switch_lampuFertilizer) == LOW)
    {
      digitalWrite(relay2_lampuFertilizer, HIGH);
      state2_lampuFertilizer = true;
      Serial.println("lampuFertilizer turned on (Manual).");
    }
    else
    {
      digitalWrite(relay2_lampuFertilizer, LOW);
      state2_lampuFertilizer = false;
      Serial.println("lampuFertilizer turned off (Manual).");
    }
  }
  else if (digitalRead(switch_pin) != prevSwitchState && flag == false)
  {
    flag = true;
    // stop current code here
  }

  if (digitalRead(switch_pin) == HIGH)
  {
    flag = false;
  }

  prevSwitchState = digitalRead(switch_pin);

  // // get current time lcd not used
  // struct tm timeinfo2;
  // if (!getLocalTime(&timeinfo2))
  // {
  //   Serial.println("Failed to obtain time");
  //   return;
  // }
  // char formattedDate2[7];
  // strftime(formattedDate2, sizeof(formattedDate2), "%d %b", &timeinfo2);
  // // Serial.println(formattedDate);
  // char formattedTime2[6];
  // strftime(formattedTime2, sizeof(formattedTime2), "%H:%M", &timeinfo2);
  // // Serial.println(formattedTime);

  // Formatted date and time to display on LCD
  char formattedDate[7];
  char formattedTime[6];
  // sprintf(formattedDate, "%02d %03d", day(now), month(now));
  strftime(formattedDate, sizeof(formattedDate), "%d %b", &timeinfo);
  sprintf(formattedTime, "%02d:%02d", hour(now), minute(now));

  // Define the lowState and highState variables
  String lowState = "off";
  String highState = "on";

  // Get the state of the waterPump and lampuFertilizer variables as a string
  String waterPumpState = state1_waterPump == LOW ? lowState : highState;
  String lampuFertilizerState = state2_lampuFertilizer == LOW ? lowState : highState;

  sensorStartTime = millis();
  // check time difference and delay if necessary
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  // float f = dht.readTemperature(true);
  // Read light intensity
  float lux = lightMeter.readLightLevel();

  // Print the sensor data on the LCD display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C ");
  lcd.print(formattedDate);

  lcd.setCursor(0, 1);
  lcd.print("Humi: ");
  lcd.print(humidity);
  lcd.print(" % ");
  lcd.print(formattedTime);

  lcd.setCursor(0, 2);
  lcd.print("Lux : ");
  lcd.print(lux);
  lcd.print(" lx");

  lcd.setCursor(0, 3);
  lcd.print("Pump: ");
  lcd.print(waterPumpState);
  lcd.print(" Lamp: ");
  lcd.print(lampuFertilizerState);

  unsigned long sensorEndTime = millis();
  if (sensorEndTime - sensorStartTime < sensorDelay)
  {
    delay(sensorDelay - (sensorEndTime - sensorStartTime));
  }
  if (isnan(humidity) || isnan(temperature) /* || isnan(f) */)
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
}
