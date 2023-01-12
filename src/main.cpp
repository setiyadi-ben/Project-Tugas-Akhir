/*
  IMPORTANT NOTES VISIT :
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
#include "time.h"
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 25200; // GMT+7 timezone
const int daylightOffset_sec = 3600;

// Wifi network station credentials
#define WIFI_SSID "Ariaqi 3"
#define WIFI_PASSWORD "lisa2218"

// Telegram BOT Token (Get from Botfather)
/*
  More details at : https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
  The MIT License

  Copyright (c) 2016-2020 Brian Lough brian.d.lough@gmail.com

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#define BOT_TOKEN "5935516261:AAEQApSV3YAfcGkdDzwi13YwcQNMWIJ_3xg"
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
const int relay1_waterPump = 23;
const int relay2_lampuFertilizer = 19;
const int relay3_solenoidValve = 18;
const int relay4 = 5;
// relay default state (my relay configuration is Normally Closed)
bool state1_waterPump = LOW;
bool state2_lampuFertilizer = LOW;
bool state3_solenoidValve = LOW;
bool state4_NaN = LOW;

// test state using builtin led esp32
// const int ledPin = 2;
// bool ledState = LOW;

void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
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

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour, 3, "%H", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay, 10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println();
}

void handleNewMessages(int numNewMessages)
{
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);

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

    // Inline buttons with callbacks when pressed will raise a callback_query message
    if (bot.messages[i].type == "callback_query")
    {
      Serial.print("Call back button pressed by: ");
      Serial.println(bot.messages[i].from_id);
      Serial.print("Data on the button: ");
      Serial.println(bot.messages[i].text);

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
        msg = "Hi " + from_name + "!\n";
        msg += "Notice how the LED state has changed!\n\n";
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
        msg = "Hi " + from_name + "!\n";
        msg += "Notice how the LED state has changed!\n\n";
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
    }

    // 'Normal' messages are handled here
    else
    {

      if (text == "/switch@bsfcontrol_bot")
      {
        // lets create a friendly welcome message
        // msg = "Hi " + from_name + "!\n";
        msg = "Menu ini adalah saklar/switch manual.\n\n";
        msg += "Silahkan tekan tombol dibawah untuk mengendalikan aktuator:\n\n";

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
        keyboardJson += "]"; // end Json                                                                                            // end of keyboard json

        // first time, send this message as a normal inline keyboard message:
        bot.sendMessageWithInlineKeyboard(chat_id, msg, "Markdown", keyboardJson);
      }

      if (text == "/setup@bsfcontrol_bot")
      {
        // lets create a friendly welcome message
        // msg = "Hi " + from_name + "!\n";
        msg = "Menu ini adalah saklar/switch otomatis.\n\n";
        msg += "Silahkan tekan tombol dibawah untuk mengendalikan aktuator:\n\n";

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
        keyboardJson += "]"; // end Json                                                                                            // end of keyboard json

        // first time, send this message as a normal inline keyboard message:
        bot.sendMessageWithInlineKeyboard(chat_id, msg, "Markdown", keyboardJson);
      }
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
        answer = "So, you need _help_, uh? me too! use /start or /status and /print";
      // if (msg.text == "/switch@bsfcontrol_bot")
      //   answer = "----------------------------";
      // if (msg.text == "/start")
      // answer = "Welcome my new friend! You are the first *" + msg.from_name + "* I've ever met";
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
  }
}

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
                            "{\"command\":\"setup\", \"description\":\"Setup penjadwalan otomatis\"}" // no comma on last command
                            // Setmode for actuators to work auto with timeInput or manually
                            //"{\"command\":\"setmode\", \"description\":\"Setup esp32 manual atau auto\"},"
                            // "{\"command\":\"led_on\", \"description\":\"turn led on\"},"
                            // "{\"command\":\"led_off\", \"description\":\"turn led off\"}" // no comma on last command
                            "]");
  bot.setMyCommands(commands);
  // bot.sendMessage("25235518", "Hola amigo!", "Markdown");
}

void setup()
{
  Serial.begin(9600);
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
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
  digitalWrite(ledPin, ledState);

  // Setup Relay Pin as OUTPUT
  pinMode(relay1_waterPump, OUTPUT);
  pinMode(relay2_lampuFertilizer, OUTPUT);
  pinMode(relay3_solenoidValve, OUTPUT);
  pinMode(relay4, OUTPUT);

  // attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  // Serial.print("Retrieving time: ");
  // configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  // time_t now = time(nullptr);
  // while (now < 24 * 3600)
  // {
  //   Serial.print(".");
  //   delay(100);
  //   now = time(nullptr);
  // }
  // Serial.println(now);
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  bot_setup();
}

void loop()
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  // float f = dht.readTemperature(true);
  // Read light intensity
  float lux = lightMeter.readLightLevel();
  // Define the lowState and highState variables
  String lowState = "off";
  String highState = "on";

  // Get the state of the waterPump and lampuFertilizer variables as a string
  String waterPumpState = state1_waterPump == LOW ? lowState : highState;
  String lampuFertilizerState = state2_lampuFertilizer == LOW ? lowState : highState;

  String date = String(day) + " ";
  // switch (month)
  // {
  // case 1:
  //   date += "Jan";
  //   break;
  // case 2:
  //   date += "Feb";
  //   break;
  // // ...
  // case 12:
  //   date += "Dec";
  //   break;
  // }

  // String time = String(hour) + ":" + String(minute);
  // if (minute < 10)
  //   time = String(hour) + ":0" + String(minute);

  // Print the sensor data on the LCD display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C ");
  // lcd.print(date);
  lcd.setCursor(0, 1);
  lcd.print("Humi: ");
  lcd.print(humidity);
  lcd.print(" % ");
  // lcd.print(time);
  lcd.setCursor(0, 2);
  lcd.print("Lux : ");
  lcd.print(lux);
  lcd.print(" lx");
  lcd.setCursor(0, 3);
  lcd.print("Pump: ");
  lcd.print(waterPumpState);
  lcd.print(" Lamp: ");
  lcd.print(lampuFertilizerState);
  printLocalTime();
  delay(1000);

  if (isnan(humidity) || isnan(temperature) /* || isnan(f) */)
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  if (millis() - bot_lasttime > BOT_MTBS)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
}
