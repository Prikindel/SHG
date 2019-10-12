#include <Servo.h>
#include <Wire.h>
#include "servoDoor.h"
//#include "tempHum.h"
#include "tempHumPres.h"
#include "motionSensorLed.h"
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
#define LEDRING             6         // Код диодного кольца
#define DOOR                7         // Код управления дверью
#define FAN                 8         // Код управления вентилятором
#define CODEUNO             9         // Код платы

#define PINMOTIONSENSOR     A0        // Пин для датчика движения
#define PINWATER            A1        // Пин для получения данных воды
#define PINLIGHTSENSOR      A2        // Пин для получения данных освещенности
#define PINPORCHLED         2         // Пин для светодиода на крыльце
#define PINFAN              3         // Пин для работы вентилятора
#define PINSERVO            4         // Пин для сервомашинка (двери)
#define PINLIGHTROOMDOWN    5         // Пин для работы света в помещении на первом этаже
#define PINLIGHTROOMUP      6         // Пин для работы света в помещении на втором этаже
#define PINCO2              7         // Пин для получения данных CO2

int countLedsOfLightInTheRoomDown = 64;// Колличество светодиодов на панели в помещении на первом этаже
int countLedsOfLightInTheRoomUp = 64; // Колличество светодиодов на панели в помещении на втором этаже
int codeGet = 0;                      // Код запроса
int argument[5] = {};                    // Получаемый аргумент
uint32_t timerLed;                    // Timer
uint32_t timerSensors;                // Timer get data sensors
char outChar[20] = {};                // Возвращаемый параметр
int rgbb[4] = {0, 0, 0, 0};           // RGB and brightness

Servo servoDoor;

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
  setLightInTheRoomDown(red, green, blue, br);
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
String withCode(int code) {
  String out = "";
  switch (code) {
    case TEMPERATURA: {
      out = (String)getTemperatureThis();
      break;
    }
    case HUMIDITY: {
      out = (String)getHumidityThis();
      break;
    }
    case PRESSURE: {
      out = (String)getPressureThis();
      break;
    }
    case CO2: {
      out = (String)getCO2This();
      break;
    }
    case WATER: {
      out = (String)getWaterThis();
      break;
    }
    case LEDRING: {
      out = setLight(rgbb[0], rgbb[1], rgbb[2], rgbb[3]);
      break;
    }
    case DOOR: {
      out = setDoor(argument[0]);
      break;
    }
    case FAN: {
      out = setFan(argument[0]);
      break;
    }
    default:
      out = "";
  }
  return out;
}

/*
 *  Get the request code 
 *  and arguments
 */

void receiveEvent(int howMany) {
  if (howMany) {
    int i = 0;
    codeGet = 0;
    argument[0] = -1;
    while (Wire.available()) { 
      int c = Wire.read();
      if (i == 0) {
        codeGet = c;
      } else {
        argument[i - 1] = c;
      }
      i++;
    }
    if (i > 2) {
      rgbb[0] = argument[0];
      rgbb[1] = argument[1];
      rgbb[2] = argument[2];
      rgbb[3] = argument[3];
    }
    withCode(codeGet).toCharArray(outChar, 20);
  }
}

/*
 *  Return the requested data
 */
 
void requestEvent(){
  Wire.write(outChar);
  Wire.write(0);
}

void setup() {
  Wire.begin(9);
  Wire.onReceive(receiveEvent);               // get Data
  Wire.onRequest(requestEvent);               // set (put) data to Client
  Serial.begin(9600);  
  delay(1000);                                // переход в активное состояние

  // инициализация всех пинов
  setMotionSensor(PINMOTIONSENSOR,            // installing the Motion Sensor and Led
                  PINPORCHLED, 
                  10000);    
  setupFan(PINFAN);                           // installing the fan
  setupWater(PINWATER);                       // installing the water sensor
  setupCO2(PINCO2);                           // installing the CO2 sensor
  setupLightSensor(PINLIGHTSENSOR,            // installing the Light Sensor
                   PINPORCHLED);
  setupServo(PINSERVO);                       // installing the servo
  setupLightInTheRoomDown(PINLIGHTROOMDOWN,   // installing the light in the room down
                        countLedsOfLightInTheRoomDown); 
  setupLightInTheRoomUp(PINLIGHTROOMUP,       // installing the light in the room up  
                        countLedsOfLightInTheRoomUp); 

  // загрузка данных
  if(!checkBME()) { 
    Serial.println("BME280 ERROR!");
  }
  loopWater();
  getTempHumPresBME();
}

void loop() {
  loopMotionSensor();
  loopLedLightSensor();
  loopCO2();
  if (millis() - timerSensors >= 2000) {      // обновляем данные с датчиков каждые 2 секунды
    Serial.println();
    timerSensors = millis();
    loopWater();
    getTempHumPresBME();
  }
}
