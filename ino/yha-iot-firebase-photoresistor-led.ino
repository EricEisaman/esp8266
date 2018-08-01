#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "YOUR_FIREBASE_PROJECT.firebaseio.com"
#define FIREBASE_AUTH "YOUR KEY HERE"
#define WIFI_SSID "YOUR_WIFI_NETWORK_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_NETWORK_PASSWORD"
#define LIGHT_IN A0
#define LED_PIN D8
#define DARKNESS_THRESHOLD 120

void setup()
{
  Serial.begin(9600);
  setupWiFi();
  
  pinMode(LED_PIN, OUTPUT); 
  digitalWrite(LED_PIN,LOW);
  
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
bool isDark = false;
int brightness = 0;
void loop()
{
  if (timePassed (checkFirebaseTime) >= 1000) {
    brightness = analogRead(LIGHT_IN);
    Firebase.setInt("share/brightness", brightness);
    if(brightness <= DARKNESS_THRESHOLD && !isDark){
      digitalWrite(LED_PIN,HIGH);
      isDark = true;
      Serial.print("");
      Serial.println("IT IS DARK. I WILL TURN THE LED ON.");
    } else if(brightness > DARKNESS_THRESHOLD){
      isDark = false;
      digitalWrite(LED_PIN,LOW);
    } 
    checkFirebaseTime = millis();
  }
}

void toggleLED(){
  digitalWrite(LED_PIN,!digitalRead(LED_PIN));
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
