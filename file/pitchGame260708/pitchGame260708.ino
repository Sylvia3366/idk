const int potPin = A0;
const int btnPin = 2;
const int btnDiff = 4;
const int buzzerPin = 9;
const int ledBlue = 13, ledGreen = 12, ledYellow = 11, ledRed = 10;
const int cd1 = 6, cd2 = 7, cd3 = 8;
const int life1 = 1, life2 = 3, life3 = 5;

//global
int targetPitch;
bool gameActive = true;
int difficulty = 1; //diff
unsigned long guessingStartTime = 0;
bool selecting = true;
int temp;
unsigned long pressStartTime = 0;
bool isPressing = false;
int life = 2;

void setup() {
  pinMode(btnPin, INPUT_PULLUP);
  pinMode(btnDiff, INPUT_PULLUP);
  pinMode(ledBlue, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(cd1, OUTPUT);
  pinMode(cd2, OUTPUT);
  pinMode(cd3, OUTPUT);
  pinMode(life1, OUTPUT);
  pinMode(life2, OUTPUT);
  pinMode(life3, OUTPUT);
  
  randomSeed(analogRead(0));
  selectDifficulty();
  refreshLives();
  startNewRound();
}

void selectDifficulty() {
  bool inMenu = true;
  while (inMenu) {
    // Show current difficulty
    digitalWrite(cd1, HIGH);
    digitalWrite(cd2, (difficulty >= 2) ? HIGH : LOW);
    digitalWrite(cd3, (difficulty >= 3) ? HIGH : LOW);

    // Change difficulty
    if (digitalRead(btnDiff) == LOW) {
      difficulty++;
      if (difficulty > 3) difficulty = 1;
      delay(300); // Simple debounce
    }
    
    // Exit menu with main button
    if (digitalRead(btnPin) == LOW) {
      inMenu = false;
      delay(500);
      clearLEDs();
    }
  }
}

void refreshLives() {
  // Keep lives within 0 and 3
  life = constrain(life, 0, 3);
  
  // Turn pins on/off based on the 'life' variable
  digitalWrite(life1, (life >= 1) ? HIGH : LOW);
  digitalWrite(life2, (life >= 2) ? HIGH : LOW);
  digitalWrite(life3, (life >= 3) ? HIGH : LOW);

  // If lives hit 0, handle game over (optional)
  if (life <= 0) {
    // Add Game Over logic here (e.g., flash lights, reset to 3)
    delay(500);
    tone(buzzerPin,500,200);
    delay(200);
    tone(buzzerPin,500,200);
    delay(200);
    tone(buzzerPin,500,200);
    delay(200);
    tone(buzzerPin,800,150);
    delay(150);
    tone(buzzerPin,500,500);
    delay(500);
    tone(buzzerPin,600,1000);
    delay(2000);
    clearLEDs();
    selectDifficulty();
    life = 2;
    refreshLives();
    startNewRound();
  }
}

void startNewRound() {
  // --- Variable Parameters based on Difficulty ---
  int minPitch = 131;
  int maxPitch;
  int duration;
  int playCount;

  if (difficulty == 1) {
    maxPitch = 523; // C5
    duration = 6000;
    playCount = 2;
  } else if (difficulty == 2) {
    maxPitch = 1047; // C6
    duration = 6000;
    playCount = 1;
  } else {
    maxPitch = 2093; // C7
    duration = 3000;
    playCount = 1;
  }

  targetPitch = random(minPitch, maxPitch + 1);

    //countdown
  int cd[] = {cd1, cd2, cd3};
  for(int i=0; i<3; i++){
    digitalWrite(cd[i], HIGH);
  }
  delay(1000);
  for(int i=0; i<3; i++){
    digitalWrite(cd[i], LOW);
    delay(1000);
  }

  // Play target pitch 2 times
  for(int i = 0; i < playCount; i++) {
    tone(buzzerPin, targetPitch, 1500);
    delay(1700);
  }
  for(int i=0; i<3; i++){
    digitalWrite(cd[i], HIGH);
  }
  guessingStartTime = millis();
}

void loop() {
  if (digitalRead(btnDiff) == LOW) {
      noTone(buzzerPin);
      clearLEDs();
      life = 0;
      refreshLives();
      selectDifficulty(); // Go back to the menu
      life = 2;
      refreshLives();
      startNewRound();    // Restart after menu
      return;             // Exit this loop cycle
  } 

  unsigned long currentMillis = millis();
  unsigned long elapsed = currentMillis - guessingStartTime;
  if (difficulty == 3){
    temp = 3000;
  } else {
    temp = 6000;
  }

// 1. Timer Logic (Check the biggest numbers first!)
  if (elapsed >= temp) {
    // Time is up!
    digitalWrite(cd1, LOW); 
    noTone(buzzerPin);
    evaluate(-1, targetPitch); 
    delay(2000);
    clearLEDs();
    startNewRound();
  } 
  else if (elapsed >= ((temp/3)*2)) {
    digitalWrite(cd2, LOW); // 4 seconds passed, turn off 3rd LED
  } 
  else if (elapsed >= (temp/3)) {
    digitalWrite(cd3, LOW); // 2 seconds passed, turn off 2nd LED
  }

  // 2. Play/Control Logic
  int potVal = analogRead(potPin);
  int playerPitch = map(potVal, 0, 1023, 131, 1047);
  tone(buzzerPin, playerPitch);

  // 3. Button Logic
  if (digitalRead(btnPin) == LOW) {
    noTone(buzzerPin);
    evaluate(playerPitch, targetPitch);
    delay(2000);
    clearLEDs();
    startNewRound();
  }
}

void evaluate(int guess, int target) {
  int diff = abs(guess - target);
  
  if (diff == 15) {
    digitalWrite(ledBlue, HIGH);
    life++;
  } else if (diff <= 50) {
    digitalWrite(ledGreen, HIGH);
  } else if (diff <= 150) {
    digitalWrite(ledYellow, HIGH);
    life--;
  } else {
    digitalWrite(ledRed, HIGH);
    life--;
  }
  refreshLives();
}

void clearLEDs() {
  digitalWrite(ledBlue, LOW);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledRed, LOW);
}