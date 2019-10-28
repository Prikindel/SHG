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
String outIndexJSON[countJSON] = {                    //key JSON
                                  "temperature",  //-температура
                                  "humidity",     //-влажность 
                                  "pressure",     //-давление
                                  "co2",          //-CO2
                                  "water"         //-влажность почвы
                                  };

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
//  Temperature data transmission
//

void temperatureTransmission() {
  // Get temperature
  getParameter(0, TEMPERATURA);
}

//
//  Humidity data transmission
//

void humidityTransmission() {
  // Get humidity
  getParameter(1, HUMIDITY);
}

//
//  Pressure data transmission
//

void pressureTransmission() {
  // Get pressure
  getParameter(2, PRESSURE);
}

//
//  CO2
//

void co2Transmission() {
  // Get CO2
  getParameter(3, CO2);
}

//
//  WATER
//

void waterTransmission() {
  // Get lvl water
  getParameter(4, WATER);
}

//
//  Receiving data depending on the code
//

void getParameter(int index, int code) {
    outJSON[index] = getDataCode(code);
}

//
//  OUT JSON
//

void outData() {
  Serial.println();
  Serial.println("Open getdata page");
  temperatureTransmission();
  humidityTransmission();
  pressureTransmission();
  co2Transmission();
  waterTransmission();
  
  String out = "{";
  for ( int i = 0; i < countJSON; i++ ) {
    
    out += "\"" + outIndexJSON[i] + "\":" /*+ "\""*/ + outJSON[i] /*+ "\""*/;
    if (i < countJSON - 1) {
      out += ",";
    }
  }
  out += "}";
  Serial.println(out);
  server.send(200, "text/html", out);
}

//
//  fan ON/OFF
//

void fanOnOff() {
  Serial.println();
  Serial.println("Open open-door page");
  if(server.args() != 0) {
    fan = server.arg("fan").toInt();
  }
  // Управляем вентилятором здесь
  Serial.print(fan == 0 ? "OFF" : "ON");
  Serial.println(" fan");
  String feedback = getDataCode(FAN);
  String out = "";
  Serial.println(feedback);
  if (feedback == "false" || feedback == "") {
    out = "ERROR";
  } else {
    out = "fan = " + (String)fan;
  }
  server.send(200, "text/html", out);
}

//
// OPEN DOOR
//

void openDoor() {
  Serial.println();
  Serial.println("Open open-door page");
  if(server.args() != 0) {
    doorInt = server.arg("door").toInt();
  }
  // Управляем дверью здесь
  Serial.print(doorInt == 0 ? "CLOSE" : ("OPEN on " + (String)doorInt));
  Serial.println(" door");
  String feedback = getDataCode(DOOR);
  String out = "";
  Serial.println(feedback);
  if (feedback == "false" || feedback == "") {
    out = "ERROR";
  } else {
    out = "door = " + (String)doorInt;
  }
  server.send(200, "text/html", out);
}

//
//  LIGHT
//

void lightRoom() {
  Serial.println();
  Serial.println("Open light-room page");
  if(server.args() != 0) {
    red   = server.arg("red").toInt();
    green = server.arg("green").toInt();
    blue  = server.arg("blue").toInt();
    light = server.arg("light").toInt();
  }
  // Управляем светом здесь
  Serial.print(light == 0 ? "OFF" : "ON");
  Serial.println(" light");
  String feedback = getDataCode(LEDRING);
  String out = "";
  Serial.println(feedback);
  if (feedback == "false" || feedback == "") {
    out = "ERROR";
  } else {
    out = "light = " + (String)light + " RGB(" + red + ";" + green + ";" + blue + ")";
  }
  server.send(200, "text/html", out);
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
