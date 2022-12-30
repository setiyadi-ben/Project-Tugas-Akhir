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

const int ledPin = 2;
bool ledState = LOW;

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

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
    if (msg.text == "/print@bsfcontrol_bot")
      answer = "----------------------------";
    if (msg.text == "/print@bsfcontrol_bot")
      answer = "All is good here, thanks for asking!";
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
  Serial.println();

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);

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
