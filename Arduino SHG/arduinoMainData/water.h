int pinWaterh;
int waterValueh = 0;

void setupWater(int pin) {
  pinWaterh = pin;
  pinMode(pinWaterh, INPUT);
}

void loopWater() {
  waterValueh = analogRead(pinWaterh);
}

float getWater() {
  return (float)waterValueh;
}
