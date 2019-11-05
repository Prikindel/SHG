#include <Seeed_BME280.h>

BME280 bme280;

float valueBME[] = {0, 0, 0}; // 0 - Температура; 1 - влажность; 2 - давление

bool checkBME() {
  if(!bme280.init()) { 
    return false;
  }
  return true;
}

void getTempHumPresBME() {
  float temp      = bme280.getTemperature();
  float pressure  = bme280.getPressure();
  float altitude  = bme280.calcAltitude(pressure);
  int   humidity  = bme280.getHumidity();
  valueBME[0] = temp;
  valueBME[1] = (float)humidity;
  valueBME[2] = pressure * 0.00750063755419211;       // 1 паскаль [Па] = 0,00750063755419211 миллиметр ртутного столба (0°C) [мм рт.ст.]
  /*//get and print temperatures
  Serial.print("Temp: "); 
  Serial.print(temp); 
  Serial.println( " C"); //The unit for Celsius because original arduino don't support speical symbols

  //get and print atmospheric pressure data
  Serial.print("Pressure: "); 
  Serial.print(pressure); 
  Serial.println(" Pa");
  Serial.print("Pressure: "); 
  Serial.print(pressure * 0.00750063755419211); 
  Serial.println(" mm");

  //get and print altitude data
  Serial.print("Altitude: "); 
  Serial.print(altitude); 
  Serial.println(" m"); 

  //get and print humidity data
  Serial.print("Humidity: "); 
  Serial.print(humidity);
  Serial.println( " % " );*/
}

float getTempBME() {   
  return valueBME[0];
}

float getHumBME() {  
  return valueBME[1];
}

float getPresBME() {  
  return valueBME[2];
}
