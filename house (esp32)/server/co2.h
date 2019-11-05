int pinCO2h;
int prevVal = LOW;
long th, tl, h, l, ppm;

void setupCO2(int pin) {
  pinCO2h = pin;
  pinMode(pinCO2h, INPUT);
}

void loopCO2() {
  long tt = millis();
  int myVal = digitalRead(pinCO2h);

  //Если обнаружили изменение
  if (myVal == HIGH) {
    if (myVal != prevVal) {
      h = tt;
      tl = h - l;
      prevVal = myVal;
    }
  }  else {
   if (myVal != prevVal) {
      l = tt;
      th = l - h;
      prevVal = myVal;
      ppm = 5000 * (th - 2) / (th + tl - 4);
    }
  }
}

float getCO2() {
  return (float)ppm;
}
