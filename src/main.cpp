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

/*
ESP32Ping
https://github.com/marian-craciunescu/ESP32Ping

Let the ESP32Ping ping a remote machine.
#Note that this is a port from https://github.com/dancol90/ESP8266Ping With this library an ESP32Ping can ping
a remote machine and know if it's reachable. It provide some basic measurements on ping messages (avg response time).
*/
#include <ESP32Ping.h> //Ping watcher library to check keep-alive connectivity

/*
  witnessmenow/Universal-Arduino-Telegram-Bot

  Introduction

  This library provides an interface for Telegram Bot API.

  Telegram is an instant messaging service that allows for the creation of bots. Bots can be configured to send and
  receive messages. This is useful for Arduino projects as you can receive updates from your project or issue it
  commands via your Telegram app from anywhere.

  This is a library forked from one library and inspired by another

  Each library only supported a single type of Arduino and had different features implemented. The only thing that needs
  to be different for each board is the actual sending of requests to Telegram so I thought a library that additional
  architectures or boards could be configured easily would be useful,
*/
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
/*
  Arduino Time Library
  https://registry.platformio.org/libraries/paulstoffregen/Time

  Time is a library that provides timekeeping functionality for Arduino.

  Using the Arduino Library Manager, install "Time by Michael Margolis".

  The code is derived from the Playground DateTime library but is updated to provide an API that is more
  flexible and easier to use.

  A primary goal was to enable date and time functionality that can be used with a variety of external time
  sources with minimum differences required in sketch logic.

  Example sketches illustrate how similar sketch code can be used with: a Real Time Clock, internet NTP time
  service, GPS time data, and Serial time messages from a computer for time synchronization.
*/
#include <TimeLib.h>
const long gmtOffset_sec = 25200; // GMT+7 timezone
const int daylightOffset_sec = 3600;
const char *ntpServer = "time.google.com";
const int ntpPort = 123;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset_sec, daylightOffset_sec);

// Wifi network station credentials
#define WIFI_SSID "XL satu@benny"
#define WIFI_PASSWORD "jaringanku"
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
#define BOT_TOKEN "5935516261:AAHmE1cO-aI1Uy0MTigdinUwgo8i_un4EAc"
// Using CHAT_ID to lock user that can access your bot outside the Group,
// get the id on @RawDataBot
#define CHAT_ID "-1001825459630"
const unsigned long BOT_MTBS = 1000; // mean time between scan messages
unsigned long bot_lasttime;          // last time messages' scan has been done
WiFiClientSecure secured_client;
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
bool state1_waterPump = false;
bool state2_lampuFertilizer = false;
bool state3_solenoidValve = LOW;

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
                            // Starting and getting started with the bot
                            "{\"command\":\"start\",  \"description\":\"Memulai penggunaan bot\"},"
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
}
// Moved numNewMessages to void loop()
// void handleNewMessages(int numNewMessages)
// {
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
  digitalWrite(switch_pin, HIGH);
  digitalWrite(relay1_waterPump, HIGH);
  digitalWrite(relay2_lampuFertilizer, HIGH);

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
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  // secured_client.getPeerCertificate();
  secured_client.setInsecure();

  // Telegram Bot Setup from Library
  bot_setup();

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

  // Disconnect notification
  if (WiFi.status() == WL_CONNECTED)
  {
    // int32_t rssi = WiFi.RSSI();
    // Serial.print("Signal Strength (RSSI): ");
    // Serial.print(rssi);
    // Serial.println(" dBm");
  }
  else
  {
    Serial.println("WiFi disconnected");
  }
  if (second(now) <= 59)
  {
    bool ret = Ping.ping("api.telegram.org");
    float avg_time_ms = Ping.averageTime();
    Serial.print("reply from api.telegram.org ");
    Serial.print(avg_time_ms);
    Serial.println(" ms");
    // WiFi signal strength
    int32_t rssi = WiFi.RSSI();
    Serial.print("Signal Strength (RSSI): ");
    Serial.print(rssi);
    Serial.println(" dBm");
  }

  //  Control & Monitoring Switch using Telegram Bot
  if (millis() - bot_lasttime > BOT_MTBS)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    for (int i = 0; i < numNewMessages; i++)
    {
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

      // UNAPPLIED
      // Check if the minutes value has changed since the last time we checked
      // static int lastMinute = -1; // Initialize to an impossible value
      // int lastMessageId = 0;
      // // Schedule the message to be deleted after 1 minutes
      // if (minute(now) != lastMinute && lastMessageId > 0)
      // {
      //   // Update the lastMinute variable
      //   lastMinute = minute(now);
      //   bot.deleteMessage(CHAT_ID, lastMessageId);
      //   Serial.println("Message was deleted");
      // }

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
        keyboardJson += "[{ \"text\" : \"Water pump is ";
        keyboardJson += (state1_waterPump ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/onWaterpump\" }]";
        // updateInlineKeyboard for lampuFertilizer
        keyboardJson += ", [{ \"text\" : \"Lampu fertilizer is ";
        keyboardJson += (state2_lampuFertilizer ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/onLampuFertilizer\" }]";
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
        keyboardJson += "[{ \"text\" : \"08 AM to 02 PM is ";
        keyboardJson += (scheduleEnabled ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/scheduleButton\" }]";
        // updateInlineKeyboard
        keyboardJson += ", [{ \"text\" : \"Schedule demo is ";
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
      String lowState = "on";
      String highState = "off";

      // Get the state of the waterPump and lampuFertilizer variables as a string
      String waterPumpState = state1_waterPump == true ? lowState : highState;
      String lampuFertilizerState = state2_lampuFertilizer == true ? lowState : highState;

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
                   "- _Water pump_ : " + waterPumpState + "\n" +
                   "- Lampu _fertilizer_ : " + lampuFertilizerState + "\n";
        }

        bot.sendMessage(msg.chat_id, answer, "Markdown");
      }

      // CALLBACK MANUAL SWITCH BUTTON TELEGRAM
      if (digitalRead(switch_pin) == HIGH && text == "/onWaterpump")
      {
        digitalWrite(relay1_waterPump, LOW);
        state1_waterPump = true;
        Serial.println("waterPump turned on (Telegram Switch).");

        // Now we can UPDATE the message, lets prepare it for sending:
        msg = "Halo " + from_name + ", perintah berhasil dijalankan.\n";
        msg += "Jika gagal, silahkan tekan tombol reset pada panel box.\n\n";
        // msg += "Try it again, see the button has updated as well:\n\n";

        // Prepare the buttons
        String keyboardJson = "["; // start Json
        // updateInlineKeyboard for waterPump
        keyboardJson += "[{ \"text\" : \"Water pump is ";
        keyboardJson += (state1_waterPump ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/offWaterpump\" }]";
        // updateInlineKeyboard for lampuFertilizer
        keyboardJson += ", [{ \"text\" : \"Lampu fertilizer is ";
        keyboardJson += (state2_lampuFertilizer ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/offLampuFertilizer\" }]";
        keyboardJson += "]"; // end Json

        // Now send this message including the current message_id as the 5th input to UPDATE that message
        bot.sendMessageWithInlineKeyboard(chat_id, msg, "Markdown", keyboardJson, message_id);
      }

      if (digitalRead(switch_pin) == HIGH && text == "/offWaterpump")
      {
        digitalWrite(relay1_waterPump, HIGH);
        state1_waterPump = false;
        Serial.println("waterPump turned off (Telegram Switch).");

        // Now we can UPDATE the message, lets prepare it for sending:
        msg = "Halo " + from_name + ", perintah berhasil dijalankan.\n";
        msg += "Jika gagal, silahkan tekan tombol reset pada panel box.\n\n";
        // msg += "Try it again, see the button has updated as well:\n\n";

        // Prepare the buttons
        String keyboardJson = "["; // start Json
        // updateInlineKeyboard for waterPump
        keyboardJson += "[{ \"text\" : \"Water pump is ";
        keyboardJson += (state1_waterPump ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/onWaterpump\" }]";
        // updateInlineKeyboard for lampuFertilizer
        keyboardJson += ", [{ \"text\" : \"Lampu fertilizer is ";
        keyboardJson += (state2_lampuFertilizer ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/onLampuFertilizer\" }]";
        keyboardJson += "]"; // end Json

        // Now send this message including the current message_id as the 5th input to UPDATE that message
        bot.sendMessageWithInlineKeyboard(chat_id, msg, "Markdown", keyboardJson, message_id);
      }

      if (digitalRead(switch_pin) == HIGH && text == "/onLampuFertilizer")
      {
        digitalWrite(relay2_lampuFertilizer, LOW);
        state2_lampuFertilizer = true;
        Serial.println("lampuFertilizer turned on (Telegram Switch).");

        // Now we can UPDATE the message, lets prepare it for sending:
        msg = "Halo " + from_name + ", perintah berhasil dijalankan.\n";
        msg += "Jika gagal, silahkan tekan tombol reset pada panel box.\n\n";
        // msg += "Try it again, see the button has updated as well:\n\n";

        // Prepare the buttons
        String keyboardJson = "["; // start Json
        // updateInlineKeyboard for waterPump
        keyboardJson += "[{ \"text\" : \"Water pump is ";
        keyboardJson += (state1_waterPump ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/offWaterpump\" }]";
        // updateInlineKeyboard for lampuFertilizer
        keyboardJson += ", [{ \"text\" : \"Lampu fertilizer is ";
        keyboardJson += (state2_lampuFertilizer ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/offLampuFertilizer\" }]";
        keyboardJson += "]"; // end Json

        // Now send this message including the current message_id as the 5th input to UPDATE that message
        bot.sendMessageWithInlineKeyboard(chat_id, msg, "Markdown", keyboardJson, message_id);
      }

      if (digitalRead(switch_pin) == HIGH && text == "/offLampuFertilizer")
      {
        digitalWrite(relay2_lampuFertilizer, HIGH);
        state2_lampuFertilizer = false;
        Serial.println("LampuFertilizer turned off (Telegram Switch).");

        // Now we can UPDATE the message, lets prepare it for sending:
        msg = "Halo " + from_name + ", perintah berhasil dijalankan.\n";
        msg += "Jika gagal, silahkan tekan tombol reset pada panel box.\n\n";
        // msg += "Try it again, see the button has updated as well:\n\n";

        // Prepare the buttons
        String keyboardJson = "["; // start Json
        // updateInlineKeyboard for waterPump
        keyboardJson += "[{ \"text\" : \"Water pump is ";
        keyboardJson += (state1_waterPump ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/onWaterpump\" }]";
        // updateInlineKeyboard for lampuFertilizer
        keyboardJson += ", [{ \"text\" : \"Lampu fertilizer is ";
        keyboardJson += (state2_lampuFertilizer ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/onLampuFertilizer\" }]";
        keyboardJson += "]"; // end Json

        // Now send this message including the current message_id as the 5th input to UPDATE that message
        bot.sendMessageWithInlineKeyboard(chat_id, msg, "Markdown", keyboardJson, message_id);
      }

      // CALLBACK SCHEDULED BUTTON
      if (text == "/scheduleButton")
      {
        // Toggle TRUE & FALSE statements
        scheduleEnabled = !scheduleEnabled;
        // Debug statements
        if (scheduleEnabled == true)
        {
          Serial.print(scheduleEnabled);
          Serial.println(" - Scheduler activated.");
        }
        else
        {
          Serial.print(scheduleEnabled);
          Serial.println(" - Scheduler deactivated.");
        }

        // Now we can UPDATE the message, lets prepare it for sending:
        msg = "Halo " + from_name + ", perintah berhasil dijalankan.\n";
        msg += "Jika gagal, silahkan tekan tombol reset pada panel box.\n\n";
        // msg += "Try it again, see the button has updated as well:\n\n";

        // Prepare the buttons
        String keyboardJson = "["; // start Json
        // updateInlineKeyboard
        keyboardJson += "[{ \"text\" : \"08 AM to 02 PM is ";
        keyboardJson += (scheduleEnabled ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/scheduleButton\" }]";
        // updateInlineKeyboard
        keyboardJson += ", [{ \"text\" : \"Schedule demo is ";
        keyboardJson += (scheduleEnabled2 ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/scheduleButton2\" }]";
        keyboardJson += "]"; // end Json

        // Now send this message including the current message_id as the 5th input to UPDATE that message
        bot.sendMessageWithInlineKeyboard(chat_id, msg, "Markdown", keyboardJson, message_id);
      }

      if (text == "/scheduleButton2")
      {
        // Toggle TRUE & FALSE statements
        scheduleEnabled2 = !scheduleEnabled2;
        // Debug statements
        if (scheduleEnabled2 == true)
        {
          Serial.print(scheduleEnabled2);
          Serial.println(" - Scheduler activated.");
        }
        else
        {
          Serial.print(scheduleEnabled2);
          Serial.println(" - Scheduler deactivated.");
        }

        // Now we can UPDATE the message, lets prepare it for sending:
        msg = "Halo " + from_name + ", perintah berhasil dijalankan.\n";
        msg += "Jika gagal, silahkan tekan tombol reset pada panel box.\n\n";
        // msg += "Try it again, see the button has updated as well:\n\n";

        // Prepare the buttons
        String keyboardJson = "["; // start Json
        // updateInlineKeyboard
        keyboardJson += "[{ \"text\" : \"08 AM to 02 PM is ";
        keyboardJson += (scheduleEnabled ? "ON" : "OFF");
        keyboardJson += "\", \"callback_data\" : \"/scheduleButton\" }]";
        // updateInlineKeyboard
        keyboardJson += ", [{ \"text\" : \"Schedule demo is ";
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

  // scheduleEnabled for field testing only
  bool lastScheduleEnabled = false;
  if (scheduleEnabled != lastScheduleEnabled)
  {
    lastScheduleEnabled = scheduleEnabled;
    if (digitalRead(switch_pin) == HIGH && scheduleEnabled && hour(now) >= 8 && hour(now) < 14)
    {
      // Debugging for an error
      // Serial.println(scheduleEnabled2);
      // Serial.println(hour(now));
      // Serial.println(switch_pin);
      // Serial.println(timeinfo.tm_hour);
      // BH1750 reading affected by lux value to toggle lampuFertilizer
      float lux = lightMeter.readLightLevel();
      if (lux < 3780)
      {
        digitalWrite(relay2_lampuFertilizer, LOW);
        state2_lampuFertilizer = true;
        Serial.println("lampuFertilizer turned on (Scheduler).");
      }
      else
      {
        digitalWrite(relay2_lampuFertilizer, HIGH);
        state2_lampuFertilizer = false;
        Serial.println("lampuFertilizer turned off (Scheduler).");
      }

      // Check if it's time to turn on the water pump
      int previousMinute = 0;
      int currentSecond = second(now);
      int currentMinute = minute(now);
      Serial.print("detik ke : ");
      Serial.println(currentSecond);
      Serial.print("menit ke : ");
      Serial.println(currentMinute);
      if (currentSecond <= 15 && currentMinute == 0 || currentMinute == 30)
      {
        // Turn on water pump for 15 secs
        digitalWrite(relay1_waterPump, LOW);
        state1_waterPump = true;
        Serial.println("waterPump turned on (Scheduler).");
      }
      // (currentSecond > 15 && currentMinute != previousMinute)
      else if (currentSecond > 15)
      {
        // Turn off water pump for 30 min
        digitalWrite(relay1_waterPump, HIGH);
        state1_waterPump = false;
        Serial.println("waterPump turned off (Scheduler).");
      }
    }
  }
  else if (digitalRead(switch_pin) == HIGH && scheduleEnabled2 != true)
  {
    // Terminating the scheduler
    digitalWrite(relay2_lampuFertilizer, HIGH);
    state2_lampuFertilizer = false;
    digitalWrite(relay1_waterPump, HIGH);
    state1_waterPump = false;
  }

  // scheduleEnabled2 for demo testing only
  bool lastScheduleEnabled2 = false;
  if (scheduleEnabled2 != lastScheduleEnabled2)
  {
    lastScheduleEnabled2 = scheduleEnabled2;
    if (digitalRead(switch_pin) == HIGH && scheduleEnabled2 && hour(now) >= 8 && hour(now) < 24)
    {
      // Debugging for an error
      // Serial.println(scheduleEnabled2);
      // Serial.println(hour(now));
      // Serial.println(switch_pin);
      // Serial.println(timeinfo.tm_hour);
      // BH1750 reading affected by lux value to toggle lampuFertilizer
      float lux = lightMeter.readLightLevel();
      if (lux < 3780)
      {
        digitalWrite(relay2_lampuFertilizer, LOW);
        state2_lampuFertilizer = true;
        Serial.println("lampuFertilizer turned on (Scheduler).");
      }
      else
      {
        digitalWrite(relay2_lampuFertilizer, HIGH);
        state2_lampuFertilizer = false;
        Serial.println("lampuFertilizer turned off (Scheduler).");
      }

      // Check if it's time to turn on the water pump
      int previousMinute = 0;
      int currentSecond = second(now);
      int currentMinute = minute(now);
      Serial.print("detik ke : ");
      Serial.println(currentSecond);
      Serial.print("menit ke : ");
      Serial.println(currentMinute);
      if (currentSecond <= 15)
      {
        // Turn on water pump for 15 secs
        digitalWrite(relay1_waterPump, LOW);
        state1_waterPump = true;
        Serial.println("waterPump turned on (Scheduler).");
      }
      else if (currentSecond > 15 && currentMinute != previousMinute)
      {
        // Turn off water pump for 45 secs
        digitalWrite(relay1_waterPump, HIGH);
        state1_waterPump = false;
        Serial.println("waterPump turned off (Scheduler).");
      }
    }
  }
  else if (digitalRead(switch_pin) == HIGH && scheduleEnabled2 != true)
  {
    // Terminating the scheduler
    digitalWrite(relay2_lampuFertilizer, HIGH);
    state2_lampuFertilizer = false;
    digitalWrite(relay1_waterPump, HIGH);
    state1_waterPump = false;
  }

  // Control Manual GPIO Switch
  if (digitalRead(switch_pin) == LOW) //&& WiFi.status() != WL_CONNECTED || WiFi.status() == WL_CONNECTED
  {
    if (digitalRead(switch_waterPump) == LOW)
    {
      digitalWrite(relay1_waterPump, LOW);
      state1_waterPump = true;
      Serial.println("waterPump turned on (Manual).");
    }
    else
    {
      digitalWrite(relay1_waterPump, HIGH);
      state1_waterPump = false;
      Serial.println("waterPump turned off (Manual).");
    }
    // LampuFertilizer
    if (digitalRead(switch_lampuFertilizer) == LOW)
    {
      digitalWrite(relay2_lampuFertilizer, LOW);
      state2_lampuFertilizer = true;
      Serial.println("lampuFertilizer turned on (Manual).");
    }
    else
    {
      digitalWrite(relay2_lampuFertilizer, HIGH);
      state2_lampuFertilizer = false;
      Serial.println("lampuFertilizer turned off (Manual).");
    }
  }
  else
  {
    Serial.println("Bot Telegram mode.");
    if (digitalRead(switch_waterPump) == HIGH)
    {
      digitalWrite(relay1_waterPump, HIGH);
      state1_waterPump = false;
    }
    if (digitalRead(switch_lampuFertilizer) == HIGH)
    {
      digitalWrite(relay2_lampuFertilizer, HIGH);
      state2_lampuFertilizer = false;
    }
  }

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
  String lowState = "on";
  String highState = "off";

  // Get the state of the waterPump and lampuFertilizer variables as a string
  String waterPumpState = state1_waterPump == true ? lowState : highState;
  String lampuFertilizerState = state2_lampuFertilizer == true ? lowState : highState;

  // sensorStartTime = millis();
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

  if (isnan(humidity) || isnan(temperature) /* || isnan(f) */)
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
}
