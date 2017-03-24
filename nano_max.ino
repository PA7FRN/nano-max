const int pinBtnRow = 5; //ASTRIX row
const int pinBtnCol = 6; //ASTRIX column
const int pinLed    = 13;

void setup() {
  pinMode(pinLed, OUTPUT);
  pinMode(pinBtnCol, OUTPUT);
  pinMode(pinBtnRow, INPUT_PULLUP);
  digitalWrite(pinBtnCol, LOW);
}

void loop() {
  int buttonState;
  
  buttonState = digitalRead(pinBtnRow);
  digitalWrite(pinLed, !buttonState);
}
