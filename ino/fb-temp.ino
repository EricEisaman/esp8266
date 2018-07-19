#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "YOUR_HOST"
#define FIREBASE_AUTH "YOUR_FB_LEGACY_KEY"
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_NETWORK_PASSWORD"
#define ONE_WIRE_BUS  D4  // temperature data pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);
char temperatureString[6];

void setup() {
  Serial.begin(9600);
  tempSensor.begin();
  setupWiFi();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setFloat("share/temp", 9000.00);
  if(Firebase.success()){
    Serial.print("Firebase write success!");
  } else {
    Serial.print("Firebase write failed!");
    Serial.println("\n");
    Serial.print(Firebase.error());
  }
  Serial.println("\n");
}

float getTemperature() {
  float temp;
  do {
    tempSensor.requestTemperatures(); 
    temp = tempSensor.getTempCByIndex(0);
    delay(100);
  } while (temp == 85.0 || temp == (-127.0));
  Firebase.setFloat("share/temp",temp);
  return temp;
}

void setupWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.println();
}

void loop() {
  float temperature = getTemperature();
  Serial.print("\n");
  Serial.print("Current temperature: ");
  Serial.print(temperature);
  Serial.print("\n");
  delay(1000);
}
