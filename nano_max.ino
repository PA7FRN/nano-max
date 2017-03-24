#include <LiquidCrystal_I2C.h>

#define LCD_ROW_DEBUG    3
#define LCD_ROW_COUNT    4
#define LCD_COL_DEBUG_1  0
#define LCD_COL_DEBUG_2  4
#define LCD_COL_DEBUG_3  8
#define LCD_COL_DEBUG_4 12
#define LCD_COL_DEBUG_5 16
#define LCD_COL_COUNT   20

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

// set the LCD address to 0x27 
// A FEW use address 0x3F
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  

void setup() {
  Serial.begin(9600); 
  lcd.begin(LCD_COL_COUNT, LCD_ROW_COUNT);

  pinMode(pinLed, OUTPUT);
  pinMode(pinBtnCol, OUTPUT);
  pinMode(pinBtnRow, INPUT_PULLUP);
  digitalWrite(pinBtnCol, LOW);

  Serial.println(String(millis()) + ": BTN_STATE_OPENED");

  lcd.setCursor(LCD_COL_DEBUG_1, LCD_ROW_DEBUG);
  lcd.print("open");
  lcd.setCursor(LCD_COL_DEBUG_5, LCD_ROW_DEBUG);
  lcd.print("clsd");
}

void loop() {
  int newPinState = digitalRead(pinBtnRow);
  unsigned long timeCurrent = millis();

  switch (buttonState) {
    case BTN_STATE_CLOSING:
      if (newPinState == HIGH) {
        buttonState = BTN_STATE_OPENED;

        Serial.println(String(timeCurrent) + ": BTN_STATE_OPENED");
        lcd.setCursor(LCD_COL_DEBUG_3, LCD_ROW_DEBUG);
        lcd.print("    ");
        lcd.setCursor(LCD_COL_DEBUG_1, LCD_ROW_DEBUG);
        lcd.print("OPEN");
      }
      else {
        if (timeCurrent - timeBtnStateClosing >= MIN_CLOSING_TIME) {
          buttonState = BTN_STATE_CLOSED;
 
          Serial.println(String(timeCurrent) + ": BTN_STATE_CLOSED");
          lcd.setCursor(LCD_COL_DEBUG_3, LCD_ROW_DEBUG);
          lcd.print("    ");
          lcd.setCursor(LCD_COL_DEBUG_5, LCD_ROW_DEBUG);
          lcd.print("CLSD");

          togleLight();
        }
      }
      break;
    case BTN_STATE_CLOSED:
      if (newPinState == HIGH) {
        timeBtnStateOpening = millis();
        buttonState = BTN_STATE_OPENING;
		
        Serial.println(String(timeCurrent) + ": BTN_STATE_OPENING");
        lcd.setCursor(LCD_COL_DEBUG_3, LCD_ROW_DEBUG);
        lcd.print("<---");
        lcd.setCursor(LCD_COL_DEBUG_5, LCD_ROW_DEBUG);
        lcd.print("clsd");
      } 
      break;
    case BTN_STATE_OPENING:  
      if (newPinState == LOW) {
        buttonState = BTN_STATE_CLOSED;
		
        Serial.println(String(timeCurrent) + ": BTN_STATE_CLOSED");
        lcd.setCursor(LCD_COL_DEBUG_3, LCD_ROW_DEBUG);
        lcd.print("    ");
        lcd.setCursor(LCD_COL_DEBUG_5, LCD_ROW_DEBUG);
        lcd.print("CLSD");
      }
      else if (timeCurrent - timeBtnStateOpening >= MIN_OPENING_TIME) {
        buttonState = BTN_STATE_OPENED;
		
        Serial.println(String(timeCurrent) + ": BTN_STATE_OPENED");
        lcd.setCursor(LCD_COL_DEBUG_3, LCD_ROW_DEBUG);
        lcd.print("    ");
        lcd.setCursor(LCD_COL_DEBUG_1, LCD_ROW_DEBUG);
        lcd.print("OPEN");
      }
      break;
    case BTN_STATE_OPENED:  
      if (newPinState == LOW) {
        timeBtnStateClosing = millis();
        buttonState = BTN_STATE_CLOSING;
		 
        Serial.println(String(timeCurrent) + ": BTN_STATE_CLOSING");
        lcd.setCursor(LCD_COL_DEBUG_3, LCD_ROW_DEBUG);
        lcd.print("--->");
        lcd.setCursor(LCD_COL_DEBUG_1, LCD_ROW_DEBUG);
        lcd.print("open");
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

