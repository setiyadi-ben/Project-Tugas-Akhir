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


void setup() {
  Serial.begin(115200);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
}

void loop() {
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