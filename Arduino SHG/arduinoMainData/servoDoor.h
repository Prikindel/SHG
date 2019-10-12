#include <Servo.h>
Servo servo1;

void setupServo(int pin) {
    servo1.attach(pin);
    servo1.write(0);
    delay(100);
}

void loopServo(int value) {
    int i = constrain(value, 0, 90);
    servo1.write(i);
}
