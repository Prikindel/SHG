int pinWaterh;
int waterValueh = 0;

void setupWater(int pin) {
  pinWaterh = pin;
  //pinMode(pinWaterh, INPUT);
}

void loopWater() {
  waterValueh = analogRead(pinWaterh);
  waterValueh = map(waterValueh, 3770, 1300, 0, 100);
  waterValueh = constrain(waterValueh, 0, 100);
}

float getWater() {
  return (float)waterValueh;
}
