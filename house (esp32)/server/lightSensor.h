int pinLightSensorh;
int pinLedLightSensorDigitalh;
int brightness = 0;

void setupLightSensor(int pin1, int pin2) { // pin1 - пин датчика освещенности; pin2 - пин диода
  pinLightSensorh = pin1;
  pinLedLightSensorDigitalh = pin2;
  pinMode(pinLightSensorh, INPUT); 
  pinMode(pinLedLightSensorDigitalh, OUTPUT); 
}

void loopLedLightSensor() {
  int darkness = analogRead(pinLightSensorh);
  if(darkness > 500)
    digitalWrite(pinLedLightSensorDigitalh, LOW);
  else                                      
    digitalWrite(pinLedLightSensorDigitalh, HIGH);
}
