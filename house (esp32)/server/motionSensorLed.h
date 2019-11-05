int pinLedMotionSensor;                 // пин диода
int pinMotionSensor;                    // пин датчика движения

long timeTimerLedMorionServer = 5000;

uint32_t timerMotionSensor;             // Timer
uint32_t timerLedMotionSensor;          // Timer

void setupMotionSensor() {
   pinMode(pinLedMotionSensor, OUTPUT);
   pinMode(pinMotionSensor, INPUT);
   timerMotionSensor = millis();
}

void setMotionSensor(int pin1, int pin2, long timeTimer) { // pin1 - пин датчика движения; pin2 - пин диода; timeTimer - время работы диода при срабатывании датчика движения
  pinLedMotionSensor = pin2;
  pinMotionSensor = pin1;
  timeTimerLedMorionServer = timeTimer;
  setupMotionSensor();
}


void loopMotionSensor() {
   if (millis() - timerMotionSensor > 60000 && millis() - timerLedMotionSensor > timeTimerLedMorionServer) {
      int val = analogRead(pinMotionSensor);
      Serial.println(val);
      if(val>500)
      {
        digitalWrite(pinLedMotionSensor, HIGH);
        timerLedMotionSensor = millis();
      }
      else {
        digitalWrite(pinLedMotionSensor, LOW);
      }
   }
}
