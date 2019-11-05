#include <HTTP_Method.h>
#include <WebServer.h>
#include <Wire.h>
#include "WiFi.h"
#include "mainPage.h"

#define TEMPERATURA       1           // Код температуры
#define HUMIDITY          2           // Код влажности
#define PRESSURE          3           // Код давления
#define CO2               4           // Код СО2
#define WATER             5           // Код воды
#define LEDRING           6           // Код диодного кольца
#define DOOR              7           // Код управления дверью
#define FAN               8           // Код управления вентилятором
#define CODEUNO           9           // Код платы
#define CONNECTION_LED    5          // Код диода состояния подключения к сети
#define LED               4          // Код диода REST API

const char* ssid = "Redmi";          // SSID WiFi
const char* password = "uuubbb555";   // Password WiFi

const int countJSON = 5;              // size JSON
String outJSON[countJSON];            // value JSON
String outIndexJSON[countJSON] = {    // key JSON
                                "temperature",
                                "humidity",
                                "pressure",
                                "co2",
                                "water"
};
char outChar[20] = {};                // returned value

int led         = 0;                  // value LED REST API
int connectLed  = 0;                  // value LED connection WiFi
int light       = 0;                  // value brightness LED RING
int red         = 0;                  // value red LED RING
int green       = 0;                  // value green LED RING
int blue        = 0;                  // value blue LED RING
int doorInt     = 0;                  // value open door
int fan         = 0;                  // value fan on

IPAddress ip(192, 168, 1, 177);       // IP server
IPAddress gateway(192,168,1,1);       // sluice
IPAddress subnet(255,255,255,0);      // mask
 
WebServer server(80);          // port connection

uint32_t timerConnectLed;             // timer LED connection

/*
 * Connect to WiFi
 */
 
void connectToWiFi(bool ipFlag) {
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
}

/*
 * Main Page
 */
 
void handleRoot() {
  Serial.println();
  Serial.println("Open main page");
  digitalWrite(LED, 1);
  server.send(200, "text/html", MAIN_page);
  digitalWrite(LED, 0);
}

/*
 * Led ON/OFF
 */

void handleLed() {
  Serial.println();
  Serial.println("Open led page");
  if(server.args() == 0) {
    led = (led == 0) ? 1 : 0;
  } else {
    led = server.arg("led").toInt();
  }
  Serial.println(led);
  digitalWrite(LED, led);
  server.send(200, "text/html", "Led = " + (String)led);
}

/*
 * Temperature data transmission
 */

void temperatureTransmission() {
  // Get temperature
  getParameter(0, TEMPERATURA);
}

/*
 * Humidity data transmission
 */

void humidityTransmission() {
  // Get humidity
  getParameter(1, HUMIDITY);
}

/*
 * Pressure data transmission
 */

void pressureTransmission() {
  // Get pressure
  getParameter(2, PRESSURE);
}

/*
 * CO2
 */

void co2Transmission() {
  // Get CO2
  getParameter(3, CO2);
}

/*
 * Water
 */

void waterTransmission() {
  // Get lvl water
  getParameter(4, WATER);
}

/*
 * Receiving data depending on the code
 */

 void getParameter(int index, int code) {
    outJSON[index] = getDataCode(code);
 }

/*
 * OUT JSON
 */

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

/*
 * Light room
 */

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

/*
 * Open door
 */
 
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

/*
 * fan ON/OFF
 */

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

/*
 * Test function
 */

void test() {
  Serial.println();
  Serial.println("Open test page");
  String test[] = {"test1", "test2"};
  int count = 2;
  String out = "{";
  for ( int i = 0; i < count; i++ ) {
    out += "\"" + (String)i + "\":" + "\"" + test[i] + "\"";
    if (i < count - 1) {
      out += ",";
    }
  }
  out += "}";
  Serial.println(out);
  server.send(200, "text/html", out);
}

/*
 * Wire write code
 */

void writeCode(int code) {
  Wire.beginTransmission(CODEUNO);
  Wire.write(code);
  switch(code) {
    case LEDRING: {
      Wire.write(red);
      Wire.write(green);
      Wire.write(blue);
      Wire.write(light);
      break;
    }
    case DOOR: {
      Wire.write(doorInt);
      break;
    }
    case FAN: {
      Wire.write(fan);
      break;
    }
    default: {
    }
  }
  Wire.endTransmission();
}

String getDataCode(int code) {
  writeCode(code);
  Serial.println();
  Serial.print("getData code=");
  Serial.println(code);
  Wire.beginTransmission(CODEUNO);
  Wire.requestFrom(CODEUNO, 20);
  int i = 0;
  while(Wire.available()) {
     outChar[i] = Wire.read();
     i++;
  }
  Wire.endTransmission();
  Serial.print("outChar = ");
  Serial.println(outChar);
  String out = "";
  if (i) {
    out = (String)outChar;
  }
  return out;
}

char t[10] = {};

// fun setup
void setup() {
  Serial.begin(115200);
  delay(10);
  
  Wire.begin();
  
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 0);
 
  connectToWiFi(false);

  IPAddress localIp = WiFi.localIP();
  ip[0] = localIp[0];
  ip[1] = localIp[1];
  ip[2] = localIp[2];
  gateway[0] = localIp[0];
  gateway[1] = localIp[1];
  gateway[2] = localIp[2];

  WiFi.disconnect();
  Serial.println("Disconnect");
  
  connectToWiFi(true);
  
 
  // Start the server

  /* 
   *
   *  REST API
   *    
   */
   
  server.on("/",            handleRoot);
//  server.on("/led",         handleLed);
  server.on("/getdata",     outData);
  server.on("/light-room",  lightRoom);
  server.on("/open-door",   openDoor);
  server.on("/fan",         fanOnOff);
//  server.on("/test",        test);

  /* 
   *
   *  START SERVER
   *    
   */
  
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  
  pinMode(CONNECTION_LED, OUTPUT);
  digitalWrite(CONNECTION_LED, 1);
  connectLed = 1;
  
}

// fun loop
void loop() {
  server.handleClient();
  if (millis() - timerConnectLed >= 100) {            // Timer
    timerConnectLed = millis();
    connectLed = !connectLed;
    digitalWrite(CONNECTION_LED, connectLed);    
  }
}
