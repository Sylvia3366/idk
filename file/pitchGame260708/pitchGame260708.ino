const int potPin = A0;
const int btnPin = 2;
const int btnDiff = 4;
const int buzzerPin = 9;
const int ledBlue = 13, ledGreen = 12, ledYellow = 11, ledRed = 10;
const int cd1 = 6, cd2 = 7, cd3 = 8;
const int progLed = 5;

//global
int targetPitch;
bool gameActive = true;
int difficulty = 1; //diff
unsigned long guessingStartTime = 0;
int p = 50;
bool selecting = true;
int temp;
unsigned long pressStartTime = 0;
bool isPressing = false;

void setup() {
  pinMode(btnPin, INPUT_PULLUP);
  pinMode(ledBlue, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(cd1, OUTPUT);
  pinMode(cd2, OUTPUT);
  pinMode(cd3, OUTPUT);
  pinMode(progLed, OUTPUT);
  
  analogWrite(progLed, p);
  randomSeed(analogRead(0));
  selectDifficulty();
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
    if (!isPressing) {
      pressStartTime = millis();
      isPressing = true;
    } else if (millis() - pressStartTime >= 1000) {
      // Held for 1 second! Enter menu.
      isPressing = false; // Reset state
      noTone(buzzerPin);
      clearLEDs();
      selectDifficulty(); // Go back to the menu
      startNewRound();    // Restart after menu
      return;             // Exit this loop cycle
    }
  } else {
    isPressing = false; // Reset if they let go
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
    p+=30;
  } else if (diff <= 50) {
    digitalWrite(ledGreen, HIGH);
    p+=15;
  } else if (diff <= 150) {
    digitalWrite(ledYellow, HIGH);
    p-=15;
  } else {
    digitalWrite(ledRed, HIGH);
    p-=30;
  }

  p = constrain(p, 0, 255); // Keep range safe
  analogWrite(progLed, p);   // Update brightness here
}

void clearLEDs() {
  digitalWrite(ledBlue, LOW);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledRed, LOW);
}