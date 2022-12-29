/*
IMPORTANT NOTES https://docs.platformio.org/en/latest/faq/ino-to-cpp.html

CREATED BY SETIYADI_BEN ALIAS BENNY HARTANTO SETIYADI FROM POLITEKNIK NEGERI SEMARANG AS A PURPOSE FOR
COMPLETING THESIS AKA "TUGAS AKHIR" FOR GET BASc IN TELECOMMUNICATION ENGINEERING.

IN ADDITION, THESE SCRIPTS LIKE SENSOR READINGS, TELEGRAM BOT, TIMING, .etc ARE MODIFIED FROM ORIGINAL SOURCE
FROM GITHUB AND MENTIONED BELOW
*/

// Lib for communicating with ESP32 PlatformIO Framework
#include <Arduino.h>

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

const int relay1 = 23;
const int relay2 = 19;
const int relay3 = 18;
const int relay4 = 5;

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

void setup(){
  Serial.begin(9600);

  dht.begin();

  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  // For Wemos / Lolin D1 Mini Pro and the Ambient Light shield use Wire.begin(D2, D1);

  lightMeter.begin();

  // Setup Relay Pin as OUTPUT
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);

  Serial.println(F("Penggabungan Sensor Reading DHT11, BH1750 dan Relay"));
}

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  // float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) /* || isnan(f) */) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  // float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  // float hic = dht.computeHeatIndex(t, h, false);

  // Print sensor readings
  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("% "));
  // Serial.print(f);
  // Serial.print(F("°F  Heat index: "));
  // Serial.print(hic);
  // Serial.print(F("°C "));
  // Serial.print(hif);
  // Serial.println(F("°F"));

  // Compute value of light intensity
  float lux = lightMeter.readLightLevel();
  // Print sensor readings
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(2000);

  // Normally Open configuration, send LOW signal to let current flow
  // (if you're usong Normally Closed configuration send HIGH signal)
  digitalWrite(relay1, LOW);
  Serial.println("Relay 1_Current Flowing");
  delay(1000); 
  
  digitalWrite(relay2, LOW);
  Serial.println("Relay 2_Current Flowing");
  delay(2000);

  digitalWrite(relay3, LOW);
  Serial.println("Relay 3_Current Flowing");
  delay(3000);

  digitalWrite(relay4, LOW);
  Serial.println("Relay 4_Current Flowing");
  delay(4000);
  
  // Normally Open configuration, send HIGH signal stop current flow
  // (if you're usong Normally Closed configuration send LOW signal)
  digitalWrite(relay1, HIGH);
  Serial.println("Relay1_Current not Flowing");
  delay(1000);

  digitalWrite(relay2, HIGH);
  Serial.println("Relay2_Current not Flowing");
  delay(2000);

  digitalWrite(relay3, HIGH);
  Serial.println("Relay3_Current not Flowing");
  delay(3000);

  digitalWrite(relay4, HIGH);
  Serial.println("Relay4_Current not Flowing");
  delay(4000);
}
