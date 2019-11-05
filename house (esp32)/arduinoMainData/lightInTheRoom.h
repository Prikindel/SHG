#include <Adafruit_NeoPixel.h>

int pinLightInTheRoomh1 = 0;
int ledsCountOfLightInTheRoomh1 = 0;
int pinLightInTheRoomh2 = 0;
int ledsCountOfLightInTheRoomh2 = 0;

Adafruit_NeoPixel strip1;
Adafruit_NeoPixel strip2;

void setupLightInTheRoomDown(int pin, int count) {
    pinLightInTheRoomh1 = pin;
    ledsCountOfLightInTheRoomh1 = count;
    strip1 = Adafruit_NeoPixel(ledsCountOfLightInTheRoomh1, pinLightInTheRoomh1, NEO_GRB + NEO_KHZ800);
    strip1.begin();
    strip1.setBrightness(50);    // яркость, от 0 до 255
    strip1.clear();                          // очистить
    strip1.show();                           // отправить на ленту
}

void setLightInTheRoomDown(int col1, int col2, int col3, int br1) {  // col1 - red; col2 - green; col3 - blue; br1 - brightness
  for (int i = 0; i < ledsCountOfLightInTheRoomh1; i++ ) {   // всю ленту
      strip1.setPixelColor(i, col1, col2, col3);   // заливка
      strip1.setBrightness(br1);
      strip1.show();                         // отправить на ленту
  }
}

void setupLightInTheRoomUp(int pin, int count) {
    pinLightInTheRoomh2 = pin;
    ledsCountOfLightInTheRoomh2 = count;
    strip2 = Adafruit_NeoPixel(ledsCountOfLightInTheRoomh2, pinLightInTheRoomh2, NEO_GRB + NEO_KHZ800);
    strip2.begin();
    strip2.setBrightness(50);    // яркость, от 0 до 255
    strip2.clear();                          // очистить
    strip2.show();                           // отправить на ленту
}

void setLightInTheRoomUp(int col1, int col2, int col3, int br1) {  // col1 - red; col2 - green; col3 - blue; br1 - brightness
  for (int i = 0; i < ledsCountOfLightInTheRoomh2; i++ ) {   // всю ленту
      strip2.setPixelColor(i, col1, col2, col3);   // заливка
      strip2.setBrightness(br1);
      strip2.show();                         // отправить на ленту
  }
}
