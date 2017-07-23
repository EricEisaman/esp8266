#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server;

char* ssid = <YOUR WIFI SSID>;
char* password = <YOUR WIFI PASSWORD>;

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
  pinMode(LED_BUILTIN, OUTPUT); 
  digitalWrite(LED_BUILTIN,LOW);

  server.on("/",sendIndex);
  server.on("/toggle", toggleLED);
  server.begin();
}

void loop()
{
  server.handleClient();
}

void sendIndex(){
  server.send(200,"text/html",INDEX_HTML);  
}

void toggleLED(){
  digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
  //server.send(204,"");
  server.send(200,"text/plain","Toggle!\n");
}
