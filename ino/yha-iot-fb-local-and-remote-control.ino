#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "YOUR_FIREBASE_PROJECT.firebaseio.com"
#define FIREBASE_AUTH "YOUR FIREBASE AUTHORIZATION KEY"
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_PASSWORD"
#define RED_LED D2
#define BLUE_LED D0
#define BUTTON D3

void setup()
{
  Serial.begin(9600);
  setupWiFi();
  
  pinMode(RED_LED, OUTPUT); 
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, HIGH);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setString("share/colorState", "red");
  if(Firebase.failed()){
    Serial.print("Firebase connection failed!");
    Serial.println("");
    Serial.print(Firebase.error());
  } else {
    Serial.print("Firebase connection success!");
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
String colorState = "red";
void loop()
{
  if(!digitalRead(BUTTON)){
     toggleLED();
     if(colorState == "red") colorState = "purple";
     else colorState = "red";
     Firebase.setString("share/colorState", colorState); 
    }
  if (timePassed (checkFirebaseTime) >= 100) {
    Serial.println(digitalRead(BUTTON));
    String fbColorState = Firebase.getString("share/colorState");
    if(fbColorState != colorState){
       colorState = fbColorState;
       toggleLED();
      }
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
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.println();
  Serial.print("Local IP: ");
  Serial.println();
  Serial.println(WiFi.localIP());
}
