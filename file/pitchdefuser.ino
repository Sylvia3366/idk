#include <DHT.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int potentiometerPin = A0; 
const int ldrPin = A1;
const int laserPin = A3;
const int sharedButtonPin = A2; 
const int dhtPin = 5;           
const int servoPin = 7;         
const int dhtType = DHT22;
DHT dht(dhtPin, dhtType);

const int buzzerPin = 3;        

const int ledBlue = 2;
const int ledYellow = 6; 
const int ledWhite = 4;

const int checkLed1 = 8;
const int checkLed2 = 9;
const int checkLed3 = 10;
const int checkLed4 = 11;
const int checkLed5 = 12;
const int checkLed6 = 13;

Servo myServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

int currentState = 1;
int greenCount = 0; 
unsigned long stateTimer = 0;
unsigned long globalGameStartTime = 0; 
const unsigned long totalGlobalGameDuration = 90000; 

int targetPitch;
int demoNoteExtraTime = 0; 
unsigned long pitchPhaseTimer = 0;
bool isDemoPlaying = true;

bool taskBlueActive = false;
unsigned long blueTaskTimer = 0;
bool taskYellowActive = false;
unsigned long yellowTaskTimer = 0;
bool taskWhiteActive = false;
unsigned long whiteTaskTimer = 0;

bool laserTaskActive = false;
unsigned long laserTaskStartTime = 0;
unsigned long dhtLastCheckTime = 0;

bool servoTaskActive = false;
int currentServoAngle = 0;
unsigned long servoTaskStartTime = 0;

int activeTasks[4] = {0, 0, 0, 0}; 
int tasksRolledCount = 0;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  dht.begin();
  
  myServo.attach(servoPin);
  myServo.write(0);

  pinMode(buzzerPin, OUTPUT);
  pinMode(ldrPin, INPUT);
  pinMode(sharedButtonPin, INPUT);
  pinMode(laserPin, OUTPUT);

  pinMode(ledBlue, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledWhite, OUTPUT);

  pinMode(checkLed1, OUTPUT);
  pinMode(checkLed2, OUTPUT);
  pinMode(checkLed3, OUTPUT);
  pinMode(checkLed4, OUTPUT);
  pinMode(checkLed5, OUTPUT);
  pinMode(checkLed6, OUTPUT);

  randomSeed(analogRead(0));
  resetCheckLEDs();
  
  currentState = 1;
  globalGameStartTime = millis(); 
  startNewRound();
  Serial.println("Game Started. State 1: Pitch Matching Active.");
}

void loop() {
  unsigned long currentMillis = millis();

  checkResistorLadderButtons();

  unsigned long globalElapsed = currentMillis - globalGameStartTime;
  if (globalElapsed >= totalGlobalGameDuration) {
    currentState = 6; 
  }

  switch (currentState) {
    case 1: 
      runState1PitchMatching(currentMillis);
      updateLCDGlobalCountdown("State 1", currentMillis);
      
      if (greenCount > 1) {
        currentState = 2;
        tasksRolledCount = 1;
        rollRandomTask(); 
        stateTimer = currentMillis;
      }
      break;

      if (greenCount >= 6) {
        currentState = 7; 
      }

    case 2: 
      runActiveTasks(currentMillis);
      updateLCDGlobalCountdown("State 2", currentMillis);
      if (currentMillis - stateTimer >= 15000) {
        currentState = 3;
        tasksRolledCount = 2;
        rollRandomTask(); 
        stateTimer = currentMillis;
      }

      if (greenCount >= 6) {
        currentState = 7; 
      }
      break;

    case 3: 
      runActiveTasks(currentMillis);
      updateLCDGlobalCountdown("State 3", currentMillis);
      if (currentMillis - stateTimer >= 10000) {
        currentState = 4;
        tasksRolledCount = 3;
        rollRandomTask(); 
        stateTimer = currentMillis;
      }

      if (greenCount >= 6) {
        currentState = 7; 
      }
      break;

    case 4: 
      runActiveTasks(currentMillis);
      updateLCDGlobalCountdown("State 4", currentMillis);
      if (currentMillis - stateTimer >= 10000) {
        currentState = 5;
        tasksRolledCount = 4;
        rollRandomTask(); 
        stateTimer = currentMillis;
      }

      if (greenCount >= 6) {
        currentState = 7; 
      }
      break;

    case 5: 
      runActiveTasks(currentMillis);
      updateLCDGlobalCountdown("DEFUSE", currentMillis); 
      
      if (greenCount >= 6) {
        currentState = 7; 
      }
      break;

    case 6: 
      handleFailState();
      break;

    case 7: 
      handleWinState();
      break;
  }
}

void startNewRound() {
  targetPitch = random(131, 1047); 
  isDemoPlaying = true;
  pitchPhaseTimer = millis();
  
  tone(buzzerPin, targetPitch, 2000 + demoNoteExtraTime);
}

void runState1PitchMatching(unsigned long currentMillis) {
  unsigned long demoDuration = 2000 + demoNoteExtraTime;
  unsigned long matchWindow = 3000; 

  if (isDemoPlaying) {
    if (currentMillis - pitchPhaseTimer >= demoDuration) {
      isDemoPlaying = false;
      pitchPhaseTimer = currentMillis; 
      noTone(buzzerPin);
    }
  } else {
    int potVal = analogRead(potentiometerPin);
    int playerPitch = map(potVal, 0, 1023, 131, 1047);
    tone(buzzerPin, playerPitch); 

    if (currentMillis - pitchPhaseTimer >= matchWindow) {
      noTone(buzzerPin);
      evaluatePitchGuess(playerPitch, targetPitch);
      delay(1000); 
      demoNoteExtraTime = 0; 
      startNewRound();
    }
  }
}

void evaluatePitchGuess(int guess, int target) {
  int diff = abs(guess - target);
  
  if (diff == 0 || diff <= 50) {
    greenCount = min(6, greenCount + 1);
    updateCheckLEDs();
  }
}

void checkResistorLadderButtons() {
  int val = analogRead(sharedButtonPin);
  
  if (val > 850 && val < 920) { 
    if (taskWhiteActive) {
      digitalWrite(ledWhite, LOW);
      taskWhiteActive = false;
    }
  } 
  else if (val > 810 && val < 870) { 
    if (taskYellowActive) {
      digitalWrite(ledYellow, LOW);
      taskYellowActive = false;
    }
  } 
  else if (val > 670 && val < 740) { 
    if (taskBlueActive) {
      digitalWrite(ledBlue, LOW);
      taskBlueActive = false;
    }
  } 
  else if (val > 570 && val < 640) { 
    if (servoTaskActive) {
      currentServoAngle = max(0, currentServoAngle - 5);
      myServo.write(currentServoAngle);
      if (currentServoAngle <= 0) {
        servoTaskActive = false;
      }
    }
  }
}

void rollRandomTask() {
  int newTask = random(1, 5); 
  for (int i = 0; i < 4; i++) {
    if (activeTasks[i] == 0) {
      activeTasks[i] = newTask;
      if (newTask == 1) Serial.println("NEW TASK INTRODUCED: Press the corresponding button when the blue, yellow, or white LED lights up.");
      else if (newTask == 2) Serial.println("NEW TASK INTRODUCED: When the laser turns on, block it.");
      else if (newTask == 3) Serial.println("NEW TASK INTRODUCED: Keep the temperature above 22º celcius at all times.");
      else if (newTask == 4) Serial.println("NEW TASK INTRODUCED: The servo has moved, return it 0º by pressing the button.");
      break;
    }
  }
}

void runActiveTasks(unsigned long currentMillis) {
  unsigned long demoDuration = 2000 + demoNoteExtraTime;
  unsigned long matchWindow = 3000;

  if (isDemoPlaying) {
    if (currentMillis - pitchPhaseTimer >= demoDuration) {
      isDemoPlaying = false;
      pitchPhaseTimer = currentMillis;
      noTone(buzzerPin);
    }
  } else {
    int potVal = analogRead(potentiometerPin);
    int playerPitch = map(potVal, 0, 1023, 131, 1047);
    tone(buzzerPin, playerPitch);

    if (currentMillis - pitchPhaseTimer >= matchWindow) {
      noTone(buzzerPin);
      evaluatePitchGuess(playerPitch, targetPitch);
      delay(1000);
      demoNoteExtraTime = 0;
      startNewRound();
    }
  }

  for (int i = 0; i < tasksRolledCount; i++) {
    int taskId = activeTasks[i];
    if (taskId == 1) handleLedTask(currentMillis);
    else if (taskId == 2) handleLaserTask(currentMillis);
    else if (taskId == 3) handleDhtTask(currentMillis);
    else if (taskId == 4) handleServoTask(currentMillis);
  }
}

void handleLedTask(unsigned long currentMillis) {
  if (!taskBlueActive && !taskYellowActive && !taskWhiteActive && random(0, 100) < 5) {
    int chosenLed = random(0, 3); 
    if (chosenLed == 0) { taskBlueActive = true; digitalWrite(ledBlue, HIGH); blueTaskTimer = currentMillis; }
    if (chosenLed == 1) { taskYellowActive = true; digitalWrite(ledYellow, HIGH); yellowTaskTimer = currentMillis; }
    if (chosenLed == 2) { taskWhiteActive = true; digitalWrite(ledWhite, HIGH); whiteTaskTimer = currentMillis; }
  }

  if (taskBlueActive && (currentMillis - blueTaskTimer > 3000)) { applyTaskPenalty(); digitalWrite(ledBlue, LOW); taskBlueActive = false; }
  if (taskYellowActive && (currentMillis - yellowTaskTimer > 3000)) { applyTaskPenalty(); digitalWrite(ledYellow, LOW); taskYellowActive = false; }
  if (taskWhiteActive && (currentMillis - whiteTaskTimer > 3000)) { applyTaskPenalty(); digitalWrite(ledWhite, LOW); taskWhiteActive = false; }
}

void handleLaserTask(unsigned long currentMillis) {
  int ldrVal = analogRead(ldrPin);
  if (digitalRead(laserPin) == LOW && random(0, 100) < 2) {
    int chosenLaser = random(0, 2); 
    if (chosenLaser == 0) { 
      digitalWrite(laserPin, HIGH); 
      laserTaskActive = true;
      laserTaskStartTime = currentMillis;
      delay(500);
    }
  }
  if (laserTaskActive && (currentMillis - laserTaskStartTime > 5000)) {
    applyTaskPenalty();
    digitalWrite(laserPin, LOW);
    laserTaskActive = false;
  } else if (ldrVal < 400){
    digitalWrite(laserPin, LOW);
    laserTaskActive = false;
  }
  if (ldrVal > 400) { 
    demoNoteExtraTime += 1000; 
    digitalWrite(laserPin, LOW);
    laserTaskActive = false;
    delay(500);
  }
}

void handleDhtTask(unsigned long currentMillis) {
  if (currentMillis - dhtLastCheckTime > 10000) { 
    float temp = dht.readTemperature();
    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(temp);
    if (!isnan(temp) && temp < 22.0) {
      demoNoteExtraTime += 2000; 
    }
    dhtLastCheckTime = currentMillis;
  }
}

void handleServoTask(unsigned long currentMillis) {
  if (!servoTaskActive && random(0, 200) < 2) {
    currentServoAngle = random(30, 181);
    myServo.write(currentServoAngle);
    servoTaskActive = true;
    servoTaskStartTime = currentMillis;
  }

  if (servoTaskActive && (currentMillis - servoTaskStartTime > 5000)) {
    applyTaskPenalty();
    servoTaskActive = false;
    myServo.write(0);
    currentServoAngle = 0;
  }
}

void applyTaskPenalty() {
  if (greenCount > 0) {
    greenCount--;
    updateCheckLEDs();
  } else {
    demoNoteExtraTime += 2000; 
  }
}

void updateCheckLEDs() {
  digitalWrite(checkLed1, greenCount >= 1 ? LOW : HIGH); 
  digitalWrite(checkLed2, greenCount >= 2 ? LOW : HIGH);
  digitalWrite(checkLed3, greenCount >= 3 ? LOW : HIGH);
  digitalWrite(checkLed4, greenCount >= 4 ? LOW : HIGH);
  digitalWrite(checkLed5, greenCount >= 5 ? LOW : HIGH);
  digitalWrite(checkLed6, greenCount >= 6 ? LOW : HIGH);
}

void resetCheckLEDs() {
  digitalWrite(checkLed1, HIGH);
  digitalWrite(checkLed2, HIGH);
  digitalWrite(checkLed3, HIGH);
  digitalWrite(checkLed4, HIGH);
  digitalWrite(checkLed5, HIGH);
  digitalWrite(checkLed6, HIGH);
}

void updateLCDGlobalCountdown(String stateName, unsigned long currentMillis) {
  unsigned long globalElapsed = currentMillis - globalGameStartTime;
  int timeLeft = (totalGlobalGameDuration > globalElapsed) ? (totalGlobalGameDuration - globalElapsed) / 1000 : 0;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Time Left: ");
  lcd.print(timeLeft);
  lcd.print("s   ");
}

void handleFailState() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("FAILED!");
  lcd.setCursor(0, 1);
  lcd.print("BOMBS AWAY");
  
  resetCheckLEDs();
  digitalWrite(ledBlue, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledWhite, LOW);
  myServo.write(0);

  tone(buzzerPin, 100, 1500); 
  delay(3000);

  currentState = 1;
  greenCount = 0;
  demoNoteExtraTime = 0;
  tasksRolledCount = 0;
  for(int i=0; i<4; i++) activeTasks[i] = 0;
  resetCheckLEDs();
  globalGameStartTime = millis();
  startNewRound();
  Serial.println("Game Reset. Returning to State 1.");
}

void handleWinState() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SUCCESS!");
  lcd.setCursor(0, 1);
  lcd.print("DEFUSED!");

  tone(buzzerPin, 523, 200);
  delay(250);
  tone(buzzerPin, 659, 200);
  delay(250);
  tone(buzzerPin, 784, 400);
  delay(2000);

  currentState = 1;
  greenCount = 0;
  demoNoteExtraTime = 0;
  tasksRolledCount = 0;
  for(int i=0; i<4; i++) activeTasks[i] = 0;
  resetCheckLEDs();
  globalGameStartTime = millis();
  startNewRound();
  Serial.println("Game Reset. Returning to State 1 after win.");
}