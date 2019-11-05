#include <HTTP_Method.h>
#include <WebServer.h>
#include <Wire.h>
#include "WiFi.h"
#include "mainPage.h"
#include "servoDoor.h"
//#include "tempHum.h"
#include "tempHumPres.h"
//#include "motionSensorLed.h"
#include "co2.h"
#include "fan.h"
#include "water.h"
#include "lightSensor.h"
#include "lightInTheRoom.h"

#define TEMPERATURA         1         // Код температуры
#define HUMIDITY            2         // Код влажности
#define PRESSURE            3         // Код давления
#define CO2                 4         // Код СО2
#define WATER               5         // Код воды
#define LEDRINGUP           10         // Код диодного кольца на втором этаже
#define LEDRINGDOWN         6         // Код диодного кольца на первом этаже
#define DOOR                7         // Код управления дверью
#define FAN                 8         // Код управления вентилятором
#define CODEUNO             9         // Код платы

#define PINCONNECTION_LED   0          // Код диода состояния подключения к сети
#define PINDCONNECTION_LED  10         // Код диода состояния подключения к сети
#define PINMOTIONSENSOR     1          // Пин для датчика движения
#define PINWATER            2          // Пин для получения данных воды
#define PINLIGHTSENSOR      3          // Пин для получения данных освещенности
#define PINPORCHLED         2          // Пин для светодиода на крыльце
#define PINFAN              3          // Пин для работы вентилятора
#define PINSERVO            4         // Пин для сервомашинка (двери)
#define PINLIGHTROOMDOWN    5         // Пин для работы света в помещении на первом этаже
#define PINLIGHTROOMUP      6         // Пин для работы света в помещении на втором этаже
#define PINCO2              7         // Пин для получения данных CO2

int countLedsOfLightInTheRoomDown = 12;// Колличество светодиодов на панели в помещении на первом этаже
int countLedsOfLightInTheRoomUp = 12; // Колличество светодиодов на панели в помещении на втором этаже
int codeGet = 0;                      // Код запроса
int argument[5] = {};                    // Получаемый аргумент
uint32_t timerLed;                    // Timer
uint32_t timerSensors;                // Timer get data sensors
uint32_t timerConnectLed;             // timer LED connection
char outChar[20] = {};                // returned value
int led         = 0;                  // value LED REST API
int connectLed  = 0;                  // value LED connection WiFi
int flooor      = 0;                  // value of floor
int light       = 0;                  // value brightness LED RING
int red         = 0;                  // value red LED RING
int green       = 0;                  // value green LED RING
int blue        = 0;                  // value blue LED RING
int doorInt     = 0;                  // value open door
int fan         = 0;                  // value fan on
int flagConnect = false;              // flag connection led


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


IPAddress ip(192, 168, 1, 177);       // IP server
IPAddress gateway(192,168,1,1);       // sluice
IPAddress subnet(255,255,255,0);      // mask
 
WebServer server(80);          // port connection


void connectLedOnOff() {
  connectLed = !connectLed;
  digitalWrite(PINCONNECTION_LED, connectLed);
  digitalWrite(PINDCONNECTION_LED, connectLed);
}

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
String setLight(int red, int green, int blue, int br, int flooor) {
  if(flooor == 1)
    setLightInTheRoomDown(red, green, blue, br);
  if(flooor == 2)
    setLightInTheRoomUp(red, green, blue, br);
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
    case LEDRINGDOWN: {
      setLight(red, green, blue, light, 1);
      break;
    }
    case LEDRINGUP: {
      setLight(red, green, blue, light, 2);
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
    default:
      break;
  }
}

/*
 * Receiving data depending on the code
 */

void setParameter(int index, int value) {
    outJSON[index] = (String)value;
}

/*
 * OUT JSON
 */

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
    flooor = server.arg("floor").toInt();
  }
  // Управляем светом здесь
  Serial.print(light == 0 ? "OFF" : "ON");
  Serial.println(" light");
  if(flooor == 1)
    withCode(LEDRINGDOWN);
  if(flooor == 2)
    withCode(LEDRINGUP);
  String out = "light = " + (String)light + " RGB(" + red + ";" + green + ";" + blue + ")" + " floor: " + (String)flooor;
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
  withCode(DOOR);
  String out = "door = " + (String)doorInt;
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
  withCode(FAN);
  String out = "fan = " + (String)fan;
  server.send(200, "text/html", out);
}


/*
 * Wire write code
 */

void writeCode(int code) {
  Wire.beginTransmission(CODEUNO);
  Wire.write(code);
  switch(code) {
    case LEDRINGUP: {
      Wire.write(red);
      Wire.write(green);
      Wire.write(blue);
      Wire.write(light);
      Wire.write(flooor);
      break;
    }
    case LEDRINGDOWN: {
      Wire.write(red);
      Wire.write(green);
      Wire.write(blue);
      Wire.write(light);
      Wire.write(flooor);
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


// fun setup
void setup() {
  pinMode(PINCONNECTION_LED, OUTPUT);
  pinMode(PINDCONNECTION_LED, OUTPUT);
  connectLedOnOff();
  
  Serial.begin(115200);
  delay(100);

  connectLedOnOff();
  
  Wire.begin();
 
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

  connectLedOnOff();
  
 
  // Start the server

  /* 
   *
   *  REST API
   *    
   */
   
  server.on("/",            handleRoot);
  server.on("/getdata",     outData);
  server.on("/light-room-up",  lightRoom);
  server.on("/light-room-down",  lightRoom);
  server.on("/open-door",   openDoor);
  server.on("/fan",         fanOnOff);

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
  
   connectLedOnOff();

  // инициализация всех пинов
  setupFan(PINFAN);                           // installing the fan
  setupWater(PINWATER);                       // installing the water sensor
  setupCO2(PINCO2);                           // installing the CO2 sensor
  setupServo(PINSERVO);                       // installing the servo
  setupLightInTheRoomDown(PINLIGHTROOMDOWN,           // installing the light in the room down
                        countLedsOfLightInTheRoomDown); 
  setupLightInTheRoomUp(PINLIGHTROOMUP,           // installing the light in the room up
                        countLedsOfLightInTheRoomUp);

  Serial.println("init suc");
  connectLedOnOff();

  // загрузка данных
  if(!checkBME()) { 
    Serial.println("BME280 ERROR!");
  }
  Serial.println("no bme err");
  loopWater();
  Serial.println("loop water");
  getTempHumPresBME();
  Serial.println("thpBME");
  setLight(0, 0, 0, 0, 1);
  Serial.println("fl1 light set");
  setLight(0, 0, 0, 0, 2);
  Serial.println("fl2 light set");

  connectLedOnOff();
  delay(500);
  connectLedOnOff();
  delay(500);
  connectLedOnOff();
  delay(500);

  connectLed = 0;
  connectLedOnOff();
  
  flagConnect = true;
  setLight(0, 0, 0, 0, 1);
  setLight(0, 0, 0, 0, 2);
}

// fun loop
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
    connectLedOnOff();
  }
}
