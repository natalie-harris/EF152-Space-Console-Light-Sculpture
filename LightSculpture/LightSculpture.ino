/*
  Author: Natalie Harris and Brenna Bentley


*/

#include <SoftwareSerial.h>
#include <SparkFunSerialGraphicLCD.h>
#include <ezButton.h>
#include <Vector.h>

// pin assignments
#define RX_PIN 2
#define TX_PIN 3
#define missile_PIN 4
#define panic_PIN 5
#define allClear_PIN 6
#define radar_PIN 7

#define BAUD_RATE 115200
#define FRAME_LENGTH 150
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

  lcd.drawBox(11, 17, 16, 11, 1);

  drawTurret(lcd, TURRET_1_X, TURRET_1_Y);
  drawTurret(lcd, TURRET_2_X, TURRET_2_Y);

}

SoftwareSerial mySerial(RX_PIN, TX_PIN); // Create a software serial port
LCD lcd; // Create an instance of the SparkFunSerialGraphicLCD class

ezButton missileButton(missile_PIN);
int missileMode = 1;

ezButton panicButton(panic_PIN);
int panicMode = 0;

ezButton allClearButton(allClear_PIN);
int allClearMode = 0;

ezButton radarButton(radar_PIN);
int radarMode = 1;

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

Asteroid** asteroids = new Asteroid*[10];

// Asteroid functions
int Asteroid::move() {
  x -= x_speed;
  y += y_speed;
  if (x < 0 || y > MAX_Y || y < 0) {
    this->reset();
  }    
}

void setup() {
  mySerial.begin(BAUD_RATE); // Initialize the software serial port
  Serial.begin(9600);
  //lcd.begin(mySerial); // Initialize the LCD
  lcd.clearScreen();

  // set up shots vector
  // for (int i = 0; i < numTurrets * 3; i++) {
  //   Shot tempShot = new Shot();
  //   shots.push_back(const Shot &value)
  // }

  // set up asteroids vector
  for (int i = 0; i < 10; i++) {
    asteroids[i] = new Asteroid();
  }


  delay(FRAME_LENGTH);
  //lcd.toggleReverseMode(); // ONLY use this if the lcd is in light mode
}

void loop() {
  //lcd.toggleReverseMode();
  lcd.clearScreen();
  drawShip(lcd);

  missileButton.loop();
  //missileMode = missileButton.getState();
  //missileMode = true;
  panicMode = true;

  if (panicMode) {

    ast1->drawAsteroid(lcd);
    ast1->move();

    ast2->drawAsteroid(lcd);
    ast2->move();

    ast3->drawAsteroid(lcd);
    ast3->move();

    ast4->drawAsteroid(lcd);
    ast4->move();
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
