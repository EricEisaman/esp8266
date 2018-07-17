#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "YOUR_FIREBASE_PROJECT.firebaseio.com"
#define FIREBASE_AUTH "YOUR KEY HERE"
#define WIFI_SSID "YOUR_WIFI_NETWORK_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_NETWORK_PASSWORD"
#define LIGHT_IN A0
#define LED_RED D2
#define LED_GREEN D1
#define LED_BLUE D0
#define DARKNESS_THRESHOLD 250

void setup()
{
  Serial.begin(9600);
  setupWiFi();
  
  pinMode(LED_RED, OUTPUT); 
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setInt("share/brightness", -1);
  if(Firebase.success()){
    Serial.print("Firebase write success!");
  } else {
    Serial.print("Firebase write failed!");
    Serial.println("");
    Serial.print(Firebase.error());
  }
  Serial.println("");
}

int timePassed (int time) {
  int diff = 0;
  if (millis() <= time) {
    diff = (69666 - time) + millis();
  } else {
    diff = millis() - time;
  }
  return diff;
}

int checkFirebaseTime = 0;
int brightness = 0;
void loop()
{
  brightness = analogRead(LIGHT_IN);
  if(brightness <= DARKNESS_THRESHOLD){
      analogWrite(LED_RED,0);
      analogWrite(LED_GREEN,0);
      analogWrite(LED_BLUE,0);
    } else {
      analogWrite(LED_RED,1023);
      analogWrite(LED_GREEN,1023);
      analogWrite(LED_BLUE,1023);
    } 
  if (timePassed (checkFirebaseTime) >= 1000) {
    Firebase.setInt("share/brightness", brightness);
    checkFirebaseTime = millis();
  }
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
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}
