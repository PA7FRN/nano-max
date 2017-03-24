const int pinBtnRow = 5; //ASTRIX row
const int pinBtnCol = 6; //ASTRIX column
const int pinLed    = 13; // D13 = on board LED

int buttonState = LOW;

void setup() {
  Serial.begin(9600); 

  pinMode(pinLed, OUTPUT);
  pinMode(pinBtnCol, OUTPUT);
  pinMode(pinBtnRow, INPUT_PULLUP);
  digitalWrite(pinBtnCol, LOW);
}

void loop() {
  int newButtonState = digitalRead(pinBtnRow);
  if (buttonState != newButtonState) {
    Serial.print(String(millis()));
    Serial.println(": button changed");
    buttonState = newButtonState;
    if (buttonState == LOW) {
      togleLight();
    }
  }
}

void togleLight() {
  if (digitalRead(pinLed) == LOW) {
    digitalWrite(pinLed, HIGH);
  }
  else {
    digitalWrite(pinLed, LOW);
  }
}

