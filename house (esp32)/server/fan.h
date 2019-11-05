int pinFanh;

void setupFan(int pin) {
  pinFanh = pin;
  pinMode(pinFanh,   OUTPUT);
}

void loopFan(int value) {
  digitalWrite(pinFanh, value);
}
