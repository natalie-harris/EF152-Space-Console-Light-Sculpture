/*
  Fading

  This example shows how to fade an LED using the analogWrite() function.

  The circuit:
  - LED attached from digital pin 9 to ground through 220 ohm resistor.

  created 1 Nov 2008
  by David A. Mellis
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Fading
*/

int digitalLedPin1 = 4;
int digitalLedPin2 = 5;
int digitalLedPin3 = 6;
int digitalLedPin4 = 7;

int ledPin1 = A0;  // LED connected to digital pin 9
int ledPin2 = A1;

void setup() {
  pinMode(digitalLedPin1, OUTPUT);
  pinMode(digitalLedPin2, OUTPUT);
  pinMode(digitalLedPin3, OUTPUT);
  pinMode(digitalLedPin4, OUTPUT);
}

void loop() {
  // fade in from min to max in increments of 5 points:
  for (int fadeValue = 0; fadeValue <= 255; fadeValue += 5) {
    // sets the value (range from 0 to 255):
    digitalWrite(digitalLedPin1, HIGH);
    digitalWrite(digitalLedPin2, HIGH);
    digitalWrite(digitalLedPin3, HIGH);
    digitalWrite(digitalLedPin4, HIGH);
    analogWrite(ledPin1, fadeValue);
    analogWrite(ledPin2, fadeValue);    
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }

  // fade out from max to min in increments of 5 points:
  for (int fadeValue = 255; fadeValue >= 0; fadeValue -= 5) {
    // sets the value (range from 0 to 255):
    digitalWrite(digitalLedPin1, LOW);
    digitalWrite(digitalLedPin2, LOW);
    digitalWrite(digitalLedPin3, LOW);
    digitalWrite(digitalLedPin4, LOW);
    analogWrite(ledPin1, fadeValue);
    analogWrite(ledPin2, fadeValue);

    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }
}
