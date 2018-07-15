#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "YOUR_FIREBASE_PROJECT.firebaseio.com"
#define FIREBASE_AUTH "YOUR KEY HERE"
#define WIFI_SSID "YOUR_WIFI_NETWORK_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_NETWORK_PASSWORD"
#define buzzerPin D2

//password must be at least 8 characters
const char WiFiAPPSK[] = "YOUR_ESP8266_PASSWORD";

void setup()
{
  Serial.begin(9600);
  setupWiFi();
  
  pinMode(LED_BUILTIN, OUTPUT); 
  digitalWrite(LED_BUILTIN,LOW);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setBool("share/buzz", false);
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
bool buzz = false;
void loop()
{
  if (timePassed (checkFirebaseTime) >= 2000) {
    buzz = Firebase.getBool("share/buzz");
    if(buzz && Firebase.success()) buzzBuzzer();
    checkFirebaseTime = millis();
  }
}

void buzzBuzzer(){
  Firebase.setBool("buzz", false);
  //tone( pin number, frequency in hertz, duration in milliseconds);
  tone(buzzerPin,2000,1000);
  delay(2000);
  digitalWrite(buzzerPin,LOW);
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
