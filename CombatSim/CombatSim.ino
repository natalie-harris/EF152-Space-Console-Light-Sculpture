/*
  Authors: Natalie Harris and Brenna Bentley


*/

#include <SoftwareSerial.h>
#include <SparkFunSerialGraphicLCD.h>
#include <ezButton.h>
#include <Vector.h>
#include <Servo.h> // for the radar spin

// pin assignments
#define RX_PIN 2
#define TX_PIN 3
#define missile_PIN 4
#define panic_PIN 5
#define allClear_PIN 6
#define radar_PIN 7
#define servo_PIN 8

#define BAUD_RATE 115200
#define FRAME_LENGTH 150
#define DEBOUNCE_TIME 50
#define MAX_X 127
#define MAX_Y 63

#define TURRET_1_X 21
#define TURRET_1_Y 20
#define TURRET_2_X 21
#define TURRET_2_Y 47

class Asteroid {
  public:
    int x;
    int y;
    int x_speed;
    int y_speed;

    int move();

    int reset() {
      x = MAX_X;
      y = (int) random(MAX_Y);
      x_speed = (int) random(3, 6);
      y_speed = (int) random(-1, 1);
    }

    void drawAsteroid(LCD lcd) {
      // lcd.drawLine(this->x, this->y, this->x + 4, this->y, 1);
      // lcd.drawLine(this->x, this->y-1, this->x, this->y+1, 1);
      lcd.drawCircle(this->x, this->y, 2, 1);
    }
    
    Asteroid() {
      x = MAX_X;
      y = (int) random(1, MAX_Y - 1);
      x_speed = (int) random(3, 6);
      y_speed = (int) random(-1, 1);

    }
};

class Shot {
  public:
    int origX;
    int x;
    int y;
    int speed;
    bool shouldPrint;

  int move();

  drawShot(LCD lcd) {
    if (this->shouldPrint) {
      lcd.drawLine(this->x, this->y, this->x + 4, this->y, 1);
      lcd.drawLine(this->x, this->y-1, this->x, this->y+1, 1);

    }
    //lcd.setPixel(shot.x + 1, byte y, byte set)
  }

  Shot() {
    origX = 0;
    x = 0;
    y = 0;
    speed = 0;
    shouldPrint = true;
  }

  Shot(int new_x, int new_y, int new_speed) {
    origX = new_x;
    x = new_x;
    y = new_y;
    speed = new_speed;
    shouldPrint = true;
  }

  ~Shot() {};

};

void drawFilledBox(LCD lcd, byte x1, byte y1, byte x2, byte y2) {
  while (x1 <= x2 && y1 >= y2) {
    lcd.drawBox(x1, y1, x2, y2, 1);
    x1++;
    y1--;
    x2--;
    y2++;
  }
}

void drawTurret(LCD lcd, byte x, byte y) {
  drawFilledBox(lcd, x, y, x+5, y-4);
  drawFilledBox(lcd, x+6, y-1, x+12, y-3);
}


void drawShip(LCD lcd) {
  lcd.drawCircle(-120, 32, 150, 1);

  drawTurret(lcd, TURRET_1_X, TURRET_1_Y);
  drawTurret(lcd, TURRET_2_X, TURRET_2_Y);

}

SoftwareSerial mySerial(RX_PIN, TX_PIN); // Create a software serial port
LCD lcd; // Create an instance of the SparkFunSerialGraphicLCD class

Servo radar; // used to control servo motor
int pos = 0;

// button assignments
ezButton missileButton(missile_PIN);
int missileMode = 0;
ezButton panicButton(panic_PIN);
int panicMode = 0;
ezButton allClearButton(allClear_PIN);
int allClearMode = 0;
ezButton radarButton(radar_PIN);
int radarMode = 0;

const int numTurrets = 2;

// Shots
Shot* t1_shot = new Shot(TURRET_1_X + 14, TURRET_1_Y - 2, 4);
Shot* t2_shot = new Shot(TURRET_2_X + 14, TURRET_2_Y - 2, 4);

// Shot functions
int Shot::move() {
  if (shouldPrint) x += speed;
  if (x > MAX_X) {
    x = origX;
    missileMode = false;
    //shouldPrint = false;
    //return 1;
  }
  return 0;
}

// Asteroids
Asteroid* ast1 = new Asteroid();
Asteroid* ast2 = new Asteroid();
Asteroid* ast3 = new Asteroid();
Asteroid* ast4 = new Asteroid();
Asteroid* ast5 = new Asteroid();

Asteroid** asteroids = new Asteroid*[10];

// Asteroid functions
int Asteroid::move() {
  x -= x_speed;
  y += y_speed;
  if (x < 28 || y > MAX_Y || y < 0) {
    this->reset();
  }    
}

void setup() {

  // for rotating the radar
  radar.attach(servo_PIN);   // attach the servo to pin 8

  mySerial.begin(BAUD_RATE); // Initialize the software serial port
  Serial.begin(BAUD_RATE);
  //lcd.begin(mySerial); // Initialize the LCD

  //initialize button debounce times
  missileButton.setDebounceTime(DEBOUNCE_TIME);
  panicButton.setDebounceTime(DEBOUNCE_TIME);
  allClearButton.setDebounceTime(DEBOUNCE_TIME);
  radarButton.setDebounceTime(DEBOUNCE_TIME);

  lcd.clearScreen();

  delay(FRAME_LENGTH);
  //lcd.toggleReverseMode(); // ONLY use this if the lcd is in light mode
}

void loop() {
  
  // get button states
  missileButton.loop();
  if (missileButton.isPressed()) missileMode = 1; // if button is pressed, toggle bool mode
  panicButton.loop();  
  if (panicButton.isPressed()) panicMode = 1;
  allClearButton.loop();
  if (allClearButton.isPressed()) panicMode = 0;
  radarButton.loop();
  if (radarButton.isPressed()) radarMode = !radarMode;

  // clears frame and draws static parts of image (hull, turrets)
  lcd.clearScreen();
  drawShip(lcd);

  // Debugging code for buttons
  Serial.print("Missiles: ");
  Serial.println(missileMode);
  Serial.print("Panic: ");
  Serial.println(panicMode);
  Serial.print("Radar: ");
  Serial.println(radarMode);
  Serial.println();

  // spins the radar in a funny circle
  if (true) { // if radar mode is true, move the servo
    radar.write(pos); // move the servo to the current position
    delay(15); // wait for the servo to reach the position
    
    pos -= 1; // decrement the servo position by 1 degree
    if (pos < 0) { // if we have reached 0 degrees, reset to 180
      pos = 180;
    }
  }


  // panic mode draws asteroids to the screen
  if (panicMode) {
    ast1->drawAsteroid(lcd);
    ast1->move();
    ast2->drawAsteroid(lcd);
    ast2->move();
    ast3->drawAsteroid(lcd);
    ast3->move();
    ast4->drawAsteroid(lcd);
    ast4->move();    
    ast5->drawAsteroid(lcd);
    ast5->move();
  }

  if (missileMode) {
    t1_shot->drawShot(lcd);
    t1_shot->move();
    t2_shot->drawShot(lcd);
    t2_shot->move();
  }

  lcd.setPixel(0, 0, 0);
  delay(FRAME_LENGTH);
}
