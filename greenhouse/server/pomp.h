int pinPumph;

void setupPump(int pin) {
  pinPumph = pin;
  pinMode(pinPumph, OUTPUT);
}

void loopPump(int value) {
  digitalWrite(pinPumph, value);
}
