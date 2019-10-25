#include <HTTP_Method.h>
#include <WebServer.h>
#include "WiFi.h"
#include "mainPage.h"


#define FAN                                           //
#define LIGHT                                         //
#define PUMP                                          //

const char* ssid = "Redmi";                                //SSID WiFi
const char* password = "uuubbb555";                            //Password WiFi

const int countJSON = 5;                              //size JSON
String outJSON[countJSON];                            //value JSON
String outIndexJSON[countJSON];                       //key JSON

IPAddress ip(192, 168, 43, 155);                      //IP server
IPAddress gateway(192, 168, 1, 1);                    //sluice
IPAddress subnet(255, 255, 255, 0);                   //mask


WebServer server(80);


//
//  Connection to WiFi
//

void connectToWiFi(bool ipFlag)
{
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  if (ipFlag) {
    // Setting static IP
    WiFi.config(ip, gateway, subnet);
  }
  WiFi.begin(ssid, password);
 
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP adress: ");
  Serial.println(WiFi.localIP());
}

//
//  Main Page
//

void handleRoot()
{
  Serial.println();
  Serial.println("Open main page");
  server.send(200, "text/html", MAIN_page);
  
}

//
//  FAN
//

void fan()
{
}

//
//  LIGHT
//
int l = 0;
void light()
{
  l = (l == 0)?1:0;
  digitalWrite(5, l);
}

//
//  PUMP
//

void pump()
{
  
}

//
//  GET DATA
//

String getData()
{
  
}

//
//  STATUS
//

String equipmentStatus()
{
  
}


void setup() {
  pinMode(5,OUTPUT);
  digitalWrite(5, LOW);
  
  Serial.begin(115200);
  delay(1000);
  connectToWiFi(false);
  server.on("/", handleRoot);
  server.on("/on", light);
  server.on("/off", light);
  server.begin();
}

void loop() {
  server.handleClient();
}
