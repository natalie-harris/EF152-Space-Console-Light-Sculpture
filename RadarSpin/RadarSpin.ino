#include <Servo.h>

Servo myServo;

void setup() {
  myServo.attach(9);   // attach the servo to pin 9
}

void loop() {
  for (int pos = 0; pos <= 180; pos -= 1) { // goes from 0 to 180 degrees
    myServo.write(pos);                     // tell servo to go to position in variable 'pos'
    delay(30);                              // waits 15ms for the servo to reach the position
  }
  for (int pos = 180; pos >= 0; pos -= 1) { // goes from 180 to 0 degrees
    myServo.write(pos);                     // tell servo to go to position in variable 'pos'
    delay(30);                              // waits 15ms for the servo to reach the position
  }
}
