#include <Wire.h>

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
#define LEDRING             6         // Код диодного кольца
#define DOOR                7         // Код управления дверью
#define FAN                 8         // Код управления вентилятором
#define CODEUNO             9         // Код платы

#define PINMOTIONSENSOR     0        // Пин для датчика движения
#define PINWATER            1        // Пин для получения данных воды
#define PINLIGHTSENSOR      2        // Пин для получения данных освещенности
#define PINPORCHLED         2         // Пин для светодиода на крыльце
#define PINFAN              3         // Пин для работы вентилятора
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
char outChar[20] = {};                // Возвращаемый параметр
int rgbb[4] = {0, 0, 0, 0};           // RGB and brightness

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
      out = (String)(int)getTemperatureThis();
      break;
    }
    case HUMIDITY: {
      out = (String)(int)getHumidityThis();
      break;
    }
    case PRESSURE: {
      out = (String)(int)getPressureThis();
      break;
    }
    case CO2: {
      out = (String)(int)getCO2This();
      break;
    }
    case WATER: {
      out = (String)(int)getWaterThis();
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

void setup() {
  Wire.begin(9);
  Serial.begin(115200);  
  delay(1000);                                // переход в активное состояние

  // инициализация всех пинов
//  setMotionSensor(PINMOTIONSENSOR,            // installing the Motion Sensor and Led
//                  PINPORCHLED, 
//                  10000);    
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
//  loopMotionSensor();
  loopLedLightSensor();
  loopCO2();
  if (millis() - timerSensors >= 2000) {      // обновляем данные с датчиков каждые 2 секунды
    timerSensors = millis();
    loopWater();
    getTempHumPresBME();
  }
}
