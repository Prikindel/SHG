int pinLightSensorh;
int pinLedLightSensorDigitalh;
int brightness = 0;
uint32_t timerLedLightSensorDigitalh;
uint32_t timerLedLightSensorDigitalDelay = 5000;

void setupLightSensor(int pin1, int pin2) { // pin1 - пин датчика освещенности; pin2 - пин диода
  pinLightSensorh = pin1;
  pinLedLightSensorDigitalh = pin2;
  pinMode(pinLightSensorh, INPUT); 
  pinMode(pinLedLightSensorDigitalh, OUTPUT); 
}

void loopLedLightSensor() {
  int darkness = analogRead(pinLightSensorh);
  if(darkness > 650) {
    if (millis() - timerLedLightSensorDigitalh >= timerLedLightSensorDigitalDelay) {
      timerLedLightSensorDigitalh = millis();
      digitalWrite(pinLedLightSensorDigitalh, LOW);
    }
  }
  else                                      
    digitalWrite(pinLedLightSensorDigitalh, HIGH);
}
