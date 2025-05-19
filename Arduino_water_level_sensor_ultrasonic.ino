#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_TEMPLATE_NAME "WaterLevelMonitor"
#define BLYNK_AUTH_TOKEN "YourAuthToken"  // Get from Blynk app

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define TRIG_PIN D5
#define ECHO_PIN D6
#define RELAY_PIN D1

char ssid[] = "YourWiFiSSID";
char pass[] = "YourWiFiPassword";

BlynkTimer timer;

long duration;
float distance;
int tankHeight = 30; // in cm, height of your water tank
int motorState = 0;  // 0 = off, 1 = on

// Virtual Pins
// V1 = Manual motor control (switch)
// V2 = Display water level
// V3 = Display motor status (on/off)

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, LOW); // Keep motor off initially

  timer.setInterval(2000L, checkWaterLevel); // Check every 2 seconds
}

// Allow manual override
BLYNK_WRITE(V1) {
  int value = param.asInt();
  motorState = value;
  digitalWrite(RELAY_PIN, motorState);
  Blynk.virtualWrite(V3, motorState ? "ON" : "OFF");
}

void checkWaterLevel() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  float waterLevel = tankHeight - distance;
  if (waterLevel < 0) waterLevel = 0;
  if (waterLevel > tankHeight) waterLevel = tankHeig
