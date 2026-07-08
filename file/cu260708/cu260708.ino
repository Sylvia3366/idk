#define ledPin 9
#define ledPinn 10
#define ledPinnn 11
#define buzzer 12
#define buttonPin 7
// #define interval 5000

// int ledState = LOW;

// unsigned long time = millis();
// unsigned long prev = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  pinMode(ledPinn, OUTPUT);
  pinMode(ledPinnn, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600); //bits per sec serial monitor loads
  Serial.println("Start of Blink"); //writes in serial monitor
}

void blink(int rep, int wait, int pin){
  for (int i = 0; i < rep; ++i){
    digitalWrite(pin, HIGH);
    delay(wait);
    digitalWrite(pin, LOW);
    delay(wait);
  }
}

void fade(int rep, int wait, int pin){
  for (int i = 0; i <= 255; i++) {
    analogWrite(pin, i);
    delay(wait);
  }
  for (int j = 255; j >= 0; j--) {
    analogWrite(pin, j);
    delay(wait);
  }
}

void buzz(){
  tone(buzzer, 1400, 60);
  blink(1, 200, 9);
}

void loop() {
  buzz();
  // if (digitalRead(buttonPin) == HIGH) {
  //   digitalWrite(ledPin, HIGH);
  //   digitalWrite(ledPinn, HIGH);
  //   digitalWrite(ledPinnn, HIGH);
  // } else {
  //   digitalWrite(ledPin, LOW);
  //   digitalWrite(ledPinn, LOW);
  //   digitalWrite(ledPinnn, LOW);
  // }
  // time = millis();
  // if (time - prev > interval){
  //   if (ledState == LOW){
  //     ledState = HIGH;
  //   } else {
  //     ledState = LOW;
  //   }
  //   digitalWrite(ledPin, ledState);
  // }
  // fade(1, 5, 9);
  // delay(20);
  // fade(1, 5, 10);
  // delay(20);
  // fade(1, 5, 11);
  // delay(20);
  // put your main code here, to run repeatedly:
}
