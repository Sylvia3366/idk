const int xPin = A0;
const int yPin = A1;
const int ledBlue = 11, ledGreen = 6, ledYellow = 5, ledRed = 10;
int xCen, yCen;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");

  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);

  xCen = analogRead(xPin);
  yCen = analogRead(yPin);
}

void loop() {
  int xVal = analogRead(xPin);
  int yVal = analogRead(yPin);

  int blueRange = constrain(map(xVal, xCen, 0, 0, 255), 0, 255);
  int greenRange = constrain(map(xVal, xCen, 1023, 0, 255), 0, 255);
  int yellowRange = constrain(map(yVal, yCen, 0, 0, 255), 0, 255);
  int redRange = constrain(map(yVal, yCen, 1023, 0, 255), 0, 255);

  analogWrite(ledBlue, blueRange);
  analogWrite(ledGreen, greenRange);
  analogWrite(ledYellow, yellowRange);
  analogWrite(ledRed, redRange);
}