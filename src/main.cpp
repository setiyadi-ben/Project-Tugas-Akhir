/*
IMPORTANT NOTES https://docs.platformio.org/en/latest/faq/ino-to-cpp.html

CREATED BY SETIYADI_BEN ALIAS BENNY HARTANTO SETIYADI FROM POLITEKNIK NEGERI SEMARANG AS A PURPOSE FOR
COMPLETING THESIS AKA "TUGAS AKHIR" FOR GET BASc IN TELECOMMUNICATION ENGINEERING.

IN ADDITION, THESE SCRIPTS LIKE SENSOR READINGS, TELEGRAM BOT, TIMING, .etc ARE MODIFIED FROM ORIGINAL SOURCE
FROM GITHUB AND MENTIONED BELOW
*/

// Lib for communicating with ESP32 PlatformIO Framework
#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Wifi network station credentials
#define WIFI_SSID "Ariaqi 3"
#define WIFI_PASSWORD "lisa2218"
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "5935516261:AAEQApSV3YAfcGkdDzwi13YwcQNMWIJ_3xg"
// Using CHAT_ID to lock user that can access your bot outside the Group, 
// get the id on @RawDataBot
#define CHAT_ID "-1001825459630"

const unsigned long BOT_MTBS = 1000; // mean time between scan messages
unsigned long bot_lasttime; // last time messages' scan has been done
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

// Import lib sensor DHT11
/*
- DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
- Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
- Example testing sketch for various DHT humidity/temperature sensors written by ladyada
*/
#include <DHT.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 4  // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Import lib sensor BH1750
#include <Wire.h>
#include <BH1750.h>

/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-relay-module-ac-web-server/
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/
// relay pinout
const int relay1_waterPump = 23;
const int relay2_lampuFertilizer = 19;
const int relay3_solenoidValve = 18;
const int relay4 = 5;
// relay default state
bool state1_waterPump = LOW;
bool state2_lampuFertilizer = LOW;
bool state3_solenoidValve = LOW;
bool state4_NaN = LOW;
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

// Initialize BH1750 sensor.
BH1750 lightMeter;

// test state using builtin led esp32
const int ledPin = 2;
bool ledState = LOW;

void handleNewMessages(int numNewMessages)
{
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);

    for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      return;
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

  String answer;
  for (int i = 0; i < numNewMessages; i++)
  {
    telegramMessage &msg = bot.messages[i];
    Serial.println("Received " + msg.text);
    // add @ to mention the bot in command message
    if (msg.text == "/setup@bsfcontrol_bot")
      answer = "===========================";
    if (msg.text == "/help@bsfcontrol_bot")
      answer = "So, you need _help_, uh? me too! use /start or /status and /print";
    if (msg.text == "/switch@bsfcontrol_bot")
      answer = "----------------------------";
    if (msg.text == "/start")
      answer = "Welcome my new friend! You are the first *" + msg.from_name + "* I've ever met";
    if (msg.text == "/print@bsfcontrol_bot"){
      // Define the lowState and highState variables
      String lowState = "idle";
      String highState = "running";

      // Get the state of the waterPump and lampuFertilizer variables as a string
      String waterPumpState = state1_waterPump == LOW ? lowState : highState;
      String lampuFertilizerState = state2_lampuFertilizer == LOW ? lowState : highState;

      // Use the state variable in the answer variable
      answer = "Berikut merupakan data monitoring dari hasil perhitungan :" "\n"
            "- Temp : " + String(temperature) + "°C\n" +
            "- Humi : " + String(humidity) + "%\n" +
            "- Lux : " + String(lux) + "lx\n" +
            "- waterPump : " + waterPumpState + "\n"+
            "- lampuFertilizer : " + lampuFertilizerState + "\n";
    }



    if (msg.text == "/led_on@bsfcontrol_bot"){
      answer = "LED state set to ON";
      ledState = HIGH;
      digitalWrite(ledPin, ledState);}
    if (msg.text == "/led_off@bsfcontrol_bot"){
      answer = "LED state set to OFF";
      ledState = LOW;
      digitalWrite(ledPin, ledState);}

    // else
    //   answer = "Say what?";

    bot.sendMessage(msg.chat_id, answer, "Markdown");
  }
}

void bot_setup()
{
  const String commands = F("[" 
  /*
  Command format is customizeable with combination of lowercase and "_". Except that, it will not shown
  in telegram bot command "/" ex: "{\"command\":\"sensorVal\",  \"description\":\"Print nilai output sensor\"},"
  */
                            // Auto setup with provided timeInput data
                            "{\"command\":\"setup\", \"description\":\"Setup penjadwalan otomatis\"},"
                            // Setmode for actuators to work auto with timeInput or manually
                            //"{\"command\":\"setmode\", \"description\":\"Setup esp32 manual atau auto\"},"

                            "{\"command\":\"help\",  \"description\":\"Panduan penggunaan bot\"},"
                            
                            // Switch On / Off actuator
                            "{\"command\":\"switch\", \"description\":\"Saklar digital aktuator scr manual\"},"
                            
                            // Print sensor reading from DHT11 & BH1750
                            "{\"command\":\"print\", \"description\":\"Output status dari esp32\"},"

                            "{\"command\":\"led_on\", \"description\":\"turn led on\"},"
                            "{\"command\":\"led_off\", \"description\":\"turn led off\"}" // no comma on last command
                            "]");
  bot.setMyCommands(commands);
  //bot.sendMessage("25235518", "Hola amigo!", "Markdown");
}

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

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
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
  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature) /* || isnan(f) */) {
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
