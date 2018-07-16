#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "YOUR_HOST"
#define FIREBASE_AUTH "YOUR_FB_LEGACY_KEY"
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_NETWORK_PASSWORD"
#define BUZZER_PIN D6
#define BUTTON_PIN D0
#define ESP8266_SSID_PREFIX "ANY_PREFIX_YOU_WANT"

//password for direct ESP8266 login must be at least 8 characters
const char WiFiAPPSK[] = "12345678";

void setup()
{
  Serial.begin(9600);
  setupWiFi();
  
  pinMode(BUZZER_PIN, OUTPUT); 
  
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
bool button = false;
bool buzzing = false;
void loop()
{
  if(!button) button = digitalRead(BUTTON_PIN);

  if (timePassed (checkFirebaseTime) >= 500) {
    buzz = Firebase.getBool("share/buzz");
    if( buzz && Firebase.success() && !buzzing) 
    { 
      buzzBuzzer();
    }
    if(button && !buzzing){
      Firebase.setBool("share/buzz",true);
    }
    checkFirebaseTime = millis();
  }
}

void buzzBuzzer(){
  buzzing = true;
  Firebase.setBool("share/buzz", false);
  Serial.print("\n");
  Serial.println("Buzzing Buzzer");
  Serial.print("\n");
  //tone( pin number, frequency in hertz, duration in milliseconds);
  tone(BUZZER_PIN,2000,500);
  delay(500); 
  digitalWrite(BUZZER_PIN,LOW);
  delay(500);
  button = false;
  buzzing = false;
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
