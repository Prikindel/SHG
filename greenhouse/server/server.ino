#include <HTTP_Method.h>
#include <WebServer.h>
#include <Wire.h>
#include "WiFi.h"
#include "mainPage.h"
#include "pinoutForShield.h"
#include "servoDoor.h"
//#include "tempHum.h"
#include "tempHumPres.h"
#include "co2.h"
#include "fan.h"
#include "water.h"
#include "lightInTheRoom.h"
#include "pomp.h"

#define TEMPERATURA         1           // Код температуры
#define HUMIDITY            2           // Код влажности
#define PRESSURE            3           // Код давления
#define CO2                 4           // Код СО2
#define WATER               5           // Код воды
#define LEDRING             6           // Код диодного кольца
#define DOOR                7           // Код управления дверью
#define FAN                 8           // Код управления вентилятором
#define PUMP                9           // Код управления помпой

#define PINCONNECTION_LED   A0          // Код диода состояния подключения к сети
#define PINWATER            A3          // Пин для получения данных воды
#define PINFAN              D3          // Пин для работы вентилятора
#define PINSERVO            D4          // Пин для сервомашинка (двери)
#define PINLIGHTROOM        D5          // Пин для работы света в помещении
#define PINCO2              D6          // Пин для получения данных CO2
#define PINPUMP             D7          // Пин для помпы

int countLedsOfLightInTheRoom = 12;     // Колличество светодиодов на панели в помещении на первом этаже

const char* ssid = "Crafto";            // SSID WiFi
const char* password = "FktrcR888";     // Password WiFi

const int countJSON = 5;                              //size JSON
String outJSON[countJSON];                            //value JSON
String outIndexJSON[countJSON] = {                    //key JSON
                                  "temperature",  //-температура
                                  "humidity",     //-влажность 
                                  "pressure",     //-давление
                                  "co2",          //-CO2
                                  "water"         //-влажность почвы
                                  };

                                  
uint32_t timerSensors;                // Timer get data sensors
uint32_t timerConnectLed;             // timer LED connection
int connectLed  = 0;                  // value LED connection WiFi
int light       = 0;                  // value brightness LED RING
int red         = 0;                  // value red LED RING
int green       = 0;                  // value green LED RING
int blue        = 0;                  // value blue LED RING
int doorInt     = 0;                  // value open door
int fan         = 0;                  // value fan on
int pump        = 0;                  // value pomp on
int flagConnect = false;              // flag connection led

IPAddress ip(192, 168, 1, 155);                      //IP server
IPAddress gateway(192, 168, 1, 1);                   //sluice
IPAddress subnet(255, 255, 255, 0);                  //mask


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

/*
 *  Read and return temperature readings
 */
float getTemperatureThis() {
  float value = getTempBME();
  return value;
}

/*
 *  Read and return humidity readings
 */
float getHumidityThis() {
  float value = getHumBME();
  return value;
}

/*
 *  Read and return pressure readings
 */
float getPressureThis() {
  float value = getPresBME();
  return value;
}

/*
 *  Read and return CO2 readings
 */
float getCO2This() {
  float value = getCO2();
  return value;
}

/*
 *  Read and return the water level
 */
float getWaterThis() {
  float value = getWater();
  return value;
}

/*
 *  Control the light in the room
 */
String setLight(int red, int green, int blue, int br) {
  setLightInTheRoom(red, green, blue, br);
  return "true";
}

/*
 *  Manage the opening of the door
 */
String setDoor(int value) {
  loopServo(value);
  return "true";
}

/*
 *  Manage the ON/OFF of the fan
 */
String setFan(int value) {
  loopFan(value);
  return "true";
}

/*
 *  Manage the ON/OFF of the pomp
 */
String setPump(int value) {
  loopPump(value);
  return "true";
}

//
//  Receiving data depending on the code
//

void setParameter(int index, int value) {
    outJSON[index] = (String)value;
}

/*
 *  Depending on the code 
 *  we perform certain actions 
 *  and return the result
 */
void withCode(int code) {
  switch (code) {
    case TEMPERATURA: {
      setParameter(0, (int)getTemperatureThis());
      break;
    }
    case HUMIDITY: {
      setParameter(1, (int)getHumidityThis());
      break;
    }
    case PRESSURE: {
      setParameter(2, (int)getPressureThis());
      break;
    }
    case CO2: {
      setParameter(3, (int)getCO2This());
      break;
    }
    case WATER: {
      setParameter(4, (int)getWaterThis());
      break;
    }
    case LEDRING: {
      setLight(red, green, blue, light);
      break;
    }
    case DOOR: {
      setDoor(doorInt);
      break;
    }
    case FAN: {
      setFan(fan);
      break;
    }
    case PUMP: {
      setPump(pump);
      break;
    }
    default:
      break;
  }
}

//
//  OUT JSON
//

void outData() {
  Serial.println();
  Serial.println("Open getdata page");
  withCode(TEMPERATURA);
  withCode(HUMIDITY);
  withCode(PRESSURE);
  withCode(CO2);
  withCode(WATER);
  
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
  Serial.println("Open fan page");
  if(server.args() != 0) {
    fan = server.arg("fan").toInt();
  }
  // Управляем вентилятором здесь
  Serial.print(fan == 0 ? "OFF" : "ON");
  Serial.println(" fan");
  withCode(FAN);
  String out = "";
  out = "fan = " + (String)fan;
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
  withCode(DOOR);
  String out = "";
  out = "door = " + (String)doorInt;
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
  withCode(LEDRING);
  String out = "";
  out = "light = " + (String)light + " RGB(" + red + ";" + green + ";" + blue + ")";
  server.send(200, "text/html", out);
}

//
//  PUMP
//

void pumpOnOff()
{
  Serial.println();
  Serial.println("Open pump page");
  if(server.args() != 0) {
    pump = server.arg("pump").toInt();
  }
  // Управляем помпой здесь
  Serial.print(pump == 0 ? "OFF" : "ON");
  Serial.println(" pump");
  withCode(PUMP);
  String out = "";
  out = "pump = " + (String)pump;
  server.send(200, "text/html", out);
}

//
//  STATUS
//
String outIndexJSONStatus[7] = {                    //key JSON
                                  "fan",  
                                  "door",     
                                  "pump",     
                                  "red",          
                                  "green",  
                                  "blue",     
                                  "light"        
                                  };
void equipmentStatus()
{
  Serial.println();
  Serial.println("Open status page");
  
  String out = "{";
  out += "\"" + outIndexJSONStatus[0] + "\":" /*+ "\""*/ + fan      /*+ "\""*/ + ",";
  out += "\"" + outIndexJSONStatus[1] + "\":" /*+ "\""*/ + doorInt  /*+ "\""*/ + ",";
  out += "\"" + outIndexJSONStatus[2] + "\":" /*+ "\""*/ + pump     /*+ "\""*/ + ",";
  out += "\"" + outIndexJSONStatus[3] + "\":" /*+ "\""*/ + red      /*+ "\""*/ + ",";
  out += "\"" + outIndexJSONStatus[4] + "\":" /*+ "\""*/ + green    /*+ "\""*/ + ",";
  out += "\"" + outIndexJSONStatus[5] + "\":" /*+ "\""*/ + blue     /*+ "\""*/ + ",";
  out += "\"" + outIndexJSONStatus[6] + "\":" /*+ "\""*/ + light    /*+ "\""*/;
  out += "}";
  Serial.println(out);
  server.send(200, "text/html", out);
}

void connectLedOnOff(int value) {
  digitalWrite(PINCONNECTION_LED, value);
  connectLed = value;
}

void setup() {  
  pinMode(PINCONNECTION_LED, OUTPUT);
  connectLedOnOff(1);
  
  Serial.begin(115200);
  delay(100);

  connectLedOnOff(0);

  Wire.begin();
  connectToWiFi(false);
  
  connectLedOnOff(1);
  
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
  
 
  connectLedOnOff(0);
  
  server.on("/",            handleRoot);
  server.on("/status",      equipmentStatus);
  server.on("/getdata",     outData);
  server.on("/light-room",  lightRoom);
  server.on("/open-door",   openDoor);
  server.on("/fan",         fanOnOff);
  server.on("/pump",        pumpOnOff);
  
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
  
  
  connectLedOnOff(1);

  // инициализация всех пинов
  setupFan(PINFAN);                           // installing the fan
  setupWater(PINWATER);                       // installing the water sensor
  setupCO2(PINCO2);                           // installing the CO2 sensor
  setupServo(PINSERVO);                       // installing the servo
  setupPump(PINPUMP);                         // installing the pump
  setupLightInTheRoom(PINLIGHTROOM,           // installing the light in the room down
                        countLedsOfLightInTheRoom); 

                        
  
  connectLedOnOff(0);

  // загрузка данных
  if(!checkBME()) { 
    Serial.println("BME280 ERROR!");
  }
  loopWater();
  getTempHumPresBME();

  
  connectLedOnOff(1);
  
  flagConnect = true;
}

void loop() {
  server.handleClient();
  loopCO2();
  if (millis() - timerSensors >= 1000) {      // обновляем данные с датчиков каждую 1 секунду
    timerSensors = millis();
    loopWater();
    getTempHumPresBME();
  }
  if (millis() - timerConnectLed >= 100 && !flagConnect) {            // Timer
    timerConnectLed = millis();
    connectLed = !connectLed;
    connectLedOnOff(connectLed);
  }
}
