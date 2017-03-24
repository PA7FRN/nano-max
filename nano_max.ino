#define BTN_STATE_CLOSING 0
#define BTN_STATE_CLOSED  1
#define BTN_STATE_OPENING 2
#define BTN_STATE_OPENED  3

#define MIN_CLOSING_TIME 10  // milliseconds
#define MIN_OPENING_TIME 50  // milliseconds

const int pinBtnRow = 5; //ASTRIX row
const int pinBtnCol = 6; //ASTRIX column
const int pinLed    = 13; // D13 = on board LED

int buttonState = BTN_STATE_OPENED;

unsigned long timeBtnStateClosing = 0;
unsigned long timeBtnStateOpening = 0;

void setup() {
  Serial.begin(9600); 

  pinMode(pinLed, OUTPUT);
  pinMode(pinBtnCol, OUTPUT);
  pinMode(pinBtnRow, INPUT_PULLUP);
  digitalWrite(pinBtnCol, LOW);

  Serial.println(String(millis()) + ": BTN_STATE_OPENED");
}

void loop() {
  int newPinState = digitalRead(pinBtnRow);
  unsigned long timeCurrent = millis();

  switch (buttonState) {
    case BTN_STATE_CLOSING:
      if (newPinState == HIGH) {
        buttonState = BTN_STATE_OPENED;
        Serial.println(String(timeCurrent) + ": BTN_STATE_OPENED");
      }
      else {
        if (timeCurrent - timeBtnStateClosing >= MIN_CLOSING_TIME) {
          buttonState = BTN_STATE_CLOSED;
          Serial.println(String(timeCurrent) + ": BTN_STATE_CLOSED");
          togleLight();
        }
      }
      break;
    case BTN_STATE_CLOSED:
      if (newPinState == HIGH) {
         timeBtnStateOpening = millis();
         buttonState = BTN_STATE_OPENING;
         Serial.println(String(timeCurrent) + ": BTN_STATE_OPENING");
      } 
      break;
    case BTN_STATE_OPENING:  
      if (newPinState == LOW) {
        buttonState = BTN_STATE_CLOSED;
        Serial.println(String(timeCurrent) + ": BTN_STATE_CLOSED");
      }
      else if (timeCurrent - timeBtnStateOpening >= MIN_OPENING_TIME) {
        buttonState = BTN_STATE_OPENED;
        Serial.println(String(timeCurrent) + ": BTN_STATE_OPENED");
      }
      break;
    case BTN_STATE_OPENED:  
      if (newPinState == LOW) {
         timeBtnStateClosing = millis();
         buttonState = BTN_STATE_CLOSING;
         Serial.println(String(timeCurrent) + ": BTN_STATE_CLOSING");
      }
      break;
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
