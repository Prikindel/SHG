#include <iarduino_DHT.h>

int pin;

float value[] = {0, 0}; // 0 - Температура; 1 - влажность

void setPinTemHum(int value) {
  pin = value;
}

void getTempHum() {
  iarduino_DHT sensor(pin);
  Serial.println("Read data...");
  switch (sensor.read()) {
    case DHT_OK: { // OK
      value[0] = sensor.tem;
      value[1] = sensor.hum;
      break;
    }
    default: { // ERROR
      value[0] = -999;
      value[1] = -999;
      break;
    }
  }
  Serial.print("temp = ");
  Serial.println(value[0]);
  Serial.print("hum = ");
  Serial.println(value[1]);
}

float getTemp() {   
  return value[0];
}

float getHum() {  
  return value[1];
}
