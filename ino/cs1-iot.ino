#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

ESP8266WebServer server;

char* ssid = "YOUR_SSID";
char* password = "SSID_PASSWORD";

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
  "<image src='https://raw.githubusercontent.com/EricEisaman/EricEisaman.github.io/master/images/eddies.png'/>"
  "<h1>ESP8266 Demo</h1>"
  "<button onclick='toggle()'>Toggle LED</button>"
  "<script>"
    "function toggle(){"
      "fetch('/toggle').then(data=>console.log(data))"
    "}"
  "</script>"
  "</body>"
  "</html>";

int POLLING_PERIOD=200; 
int intervalStartMS=millis();

void setup()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  Serial.begin(9600);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);  
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.print(WiFi.localIP());
  Serial.println("");
  pinMode(LED_BUILTIN, OUTPUT); 
  digitalWrite(LED_BUILTIN,LOW);//Turn on LED

  server.on("/",sendIndex);
  server.on("/toggle", toggleLED);
  server.begin();
}

void loop()
{
  server.handleClient();
  if( millis() > intervalStartMS + POLLING_PERIOD ) {
    pollServer();
    intervalStartMS = millis();  
  }
  
  
}

void sendIndex(){
  server.send(200,"text/html",INDEX_HTML);  
}

void toggleLED(){
  digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
  //server.send(204,"");
  server.send(200,"text/plain","Toggle!\n");
}


void pollServer(){
  HTTPClient http;
    http.begin("http://iot-api.glitch.me/iot-get/?name=CS1&prop=LED");
    int httpCode = http.GET();
    // httpCode will be negative on error
    if(httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if(httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            Serial.println(payload);
            Serial.println("");
            setLED(payload);
        }
     } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
     }

     http.end(); 
}
String on = String("on");
void setLED(String state){
  if(state == on){
      Serial.println("TURNING THE LED ON.");
      Serial.println("");
      digitalWrite(LED_BUILTIN,LOW);
    }else{
      Serial.println("TURNING THE LED OFF.");
      Serial.println("");
      digitalWrite(LED_BUILTIN,HIGH);
    }
}
