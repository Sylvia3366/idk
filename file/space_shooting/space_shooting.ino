#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int xPin = A2, yPin = A1, potPin = A0, swPin = 4;

const int MAX_ASTEROIDS = 3;
int astX[MAX_ASTEROIDS] = {16, 18, 20};
int astY[MAX_ASTEROIDS] = {0, 1, 0};
char astChar[MAX_ASTEROIDS] = {'*', '#', 'X'};

int shipX = 0, shipY = 0;
int laserX = -1, laserY = -1;
unsigned long startTime = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(potPin, INPUT);
  pinMode(swPin, INPUT_PULLUP);
  startTime = millis();
}

void loop() {
  int xVal = analogRead(xPin);
  int yVal = analogRead(yPin);
  int speed = map(analogRead(potPin), 0, 1023, 600, 50);

  if (xVal < 200 && shipX > 0) shipX--;
  if (xVal > 800 && shipX < 15) shipX++;
  if (yVal > 800) shipY = 1;
  if (yVal < 200) shipY = 0;

  if (digitalRead(swPin) == LOW && laserX == -1) {
    laserX = shipX + 1;
    laserY = shipY;
  }
  if (laserX != -1) {
    laserX++;
    if (laserX > 15) laserX = -1;
  }

  lcd.clear();
  for (int i = 0; i < MAX_ASTEROIDS; i++) {
    astX[i]--;
    if (astX[i] < 0) {
      astX[i] = 16;
      astY[i] = random(0, 2);
    }

    if (laserX == astX[i] && laserY == astY[i]) {
      astX[i] = 16;
      laserX = -1;
    }

    if (astX[i] == shipX && astY[i] == shipY) {
      gameOver();
    }
    
    lcd.setCursor(astX[i], astY[i]);
    lcd.print(astChar[i]);
  }

  lcd.setCursor(shipX, shipY);
  lcd.print(">");
  if (laserX != -1) {
    lcd.setCursor(laserX, laserY);
    lcd.print("-");
  }

  unsigned long secondsAlive = (millis() - startTime) / 1000;
  lcd.setCursor(13, 1);
  lcd.print(secondsAlive);

  delay(speed);
}

void gameOver() {
  unsigned long finalScore = (millis() - startTime) / 1000;
  lcd.clear();
  lcd.print("GAME OVER!");
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.print(finalScore);
  delay(3000);
  
  startTime = millis();
  shipX = 0;
  for (int i = 0; i < MAX_ASTEROIDS; i++) astX[i] = 16 + (i * 2);
}