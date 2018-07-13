#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "YOUR_FIREBASE_PROJECT.firebaseio.com"
#define FIREBASE_AUTH "YOUR FIREBASE AUTHORIZATION KEY"
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_PASSWORD"
#define ESP8266_SSID_PREFIX "ANY_PREFIX_YOU_WANT"
#define RED_LED D2
#define BLUE_LED D0
#define BUTTON D3
ESP8266WebServer server;
//Password for direct ESP8266 connection. Must be at least 8 characters
const char WiFiAPPSK[] = "12345678";

const char INDEX_HTML[] =
  "<!DOCTYPE HTML>"
  "<html>"
  "<head>"
  "<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\">"
  "<title>ESP8266</title>"
  "<style>"
    "body { background-color: #808080; font-family: Arial, Helvetica, Sans-Serif; Color: Maroon; }"
  "</style>"
  "</head>"
  "<body>"
  "<h1>ESP8266 Direct Access Demo</h1>"
  "<button onclick='toggleLED()'>Toggle LED</button>"
  "<script>"
    "function toggleLED(){"
      "fetch('/toggleLED').then(stream=>stream.text()).then(text=>console.log(text))"
    "}"
  "</script>"
  "</body>"
  "</html>";

void setup()
{
  Serial.begin(9600);
  setupWiFi();
  
  pinMode(RED_LED, OUTPUT); 
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, HIGH);

  server.on("/",sendIndex);
  server.on("/toggleLED", toggleLED);
  server.begin();
  Serial.println("");
  Serial.println("");
  Serial.print("Server running on http://192.168.4.1/");
  Serial.println("");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setString("share/colorState", "red");
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
String colorState = "red";
void loop()
{
  if(!digitalRead(BUTTON)){
     toggleLED();
     if(colorState == "red") colorState = "purple";
     else colorState = "red";
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
  server.handleClient();
}

void sendIndex(){
  server.send(200,"text/html",INDEX_HTML);  
}

void toggleLED(){
  digitalWrite(BLUE_LED,!digitalRead(BLUE_LED));
  //digitalWrite(RED_LED,!digitalRead(RED_LED));
  //server.send(204,"");
  server.send(200,"text/plain","Toggle LED!\n");
}

void setupWiFi()
{
  WiFi.mode(WIFI_AP_STA);
  //Set up Access Point
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
  
  // Set up Station
  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "Thing-":
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = ESP8266_SSID_PREFIX + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);
  //AP_NameChar is the SSID for the Wireless Access Point
  WiFi.softAP(AP_NameChar, WiFiAPPSK);
}
