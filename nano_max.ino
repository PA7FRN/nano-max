#include <LiquidCrystal_I2C.h>

#define LCD_ROW_DEBUG    3
#define LCD_ROW_COUNT    4
#define LCD_COL_DEBUG_1  0
#define LCD_COL_DEBUG_2  5
#define LCD_COL_DEBUG_3  9
#define LCD_COL_DEBUG_4 11
#define LCD_COL_DEBUG_5 16
#define LCD_COL_COUNT   20

#define BTN_STATE_CLOSING 0
#define BTN_STATE_CLOSED  1
#define BTN_STATE_OPENING 2
#define BTN_STATE_OPENED  3

#define R_COUNT 4
#define C_COUNT 4

#define KEY_1     0
#define KEY_2     1
#define KEY_3     2
#define KEY_A     3

#define KEY_4     4
#define KEY_5     5
#define KEY_6     6
#define KEY_B     7

#define KEY_7     8
#define KEY_8     9
#define KEY_9    10
#define KEY_C    11

#define KEY_ASTR 12
#define KEY_0    13
#define KEY_HASH 14
#define KEY_D    15

#define MIN_CLOSING_TIME 10  // milliseconds
#define MIN_OPENING_TIME 50  // milliseconds

int pinBtnRow[R_COUNT] = {9,8,7,6}; //  D6 = header pin 9, D7 = pin 10 ...
int pinBtnCol[C_COUNT] = {5,4,3,2}; //  D2 = header pin 5, D3 = pin 6 ...
const int pinLed       = 13;        // D13 = on board LED

int buttonState[R_COUNT][C_COUNT];

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
  
  for (int c=0; c < C_COUNT; c++) {
    pinMode(pinBtnCol[c], INPUT_PULLUP);
  }

  for (int r=0; r < R_COUNT; r++) {
    pinMode(pinBtnRow[r], OUTPUT);
    digitalWrite(pinBtnRow[r], HIGH);
  }

  for (int c=0; c < C_COUNT; c++) {
    for (int r=0; r < R_COUNT; r++) {
      buttonState[r][c] = BTN_STATE_OPENED;
    }
  }

  lcd.setCursor(LCD_COL_DEBUG_2, LCD_ROW_DEBUG);
  lcd.print("up:");
  lcd.setCursor(LCD_COL_DEBUG_4, LCD_ROW_DEBUG);
  lcd.print("down:");
}

void loop() {
  for (int r=0; r < R_COUNT; r++) {
   digitalWrite(pinBtnRow[r], LOW);
   for (int c=0; c < C_COUNT; c++) {
      getButtonState(r, c);
    }
   digitalWrite(pinBtnRow[r], HIGH);
  }  
}

void getButtonState(int r, int c) {
  int newPinState = digitalRead(pinBtnCol[c]);
  unsigned long timeCurrent = millis();

  switch (buttonState[r][c]) {
    case BTN_STATE_CLOSING:
      if (newPinState == HIGH) {
        buttonState[r][c] = BTN_STATE_OPENED;
        debugBtnState(r, c);
      }
      else if (timeCurrent - timeBtnStateClosing >= MIN_CLOSING_TIME) {
        buttonState[r][c] = BTN_STATE_CLOSED;
        debugBtnState(r, c);
        buttonDown(r,c);
      }
      break;
    case BTN_STATE_CLOSED:
      if (newPinState == HIGH) {
        timeBtnStateOpening = millis();
        buttonState[r][c] = BTN_STATE_OPENING;
        debugBtnState(r, c);
      } 
      break;
    case BTN_STATE_OPENING:  
      if (newPinState == LOW) {
        buttonState[r][c] = BTN_STATE_CLOSED;
        debugBtnState(r, c);
      }
      else if (timeCurrent - timeBtnStateOpening >= MIN_OPENING_TIME) {
        buttonState[r][c] = BTN_STATE_OPENED;
        debugBtnState(r, c);
        buttonUp(r,c);
      }
      break;
    case BTN_STATE_OPENED:  
     if (newPinState == LOW) {
        timeBtnStateClosing = millis();
        buttonState[r][c] = BTN_STATE_CLOSING;
        debugBtnState(r, c);
      }
      break;
  }
}

void buttonDown(int r, int c) {
  int key = r * C_COUNT + c;
  lcd.setCursor(LCD_COL_DEBUG_5, LCD_ROW_DEBUG);
  printKeyToLcd(key);
}

void buttonUp(int r, int c) {
  int key = r * C_COUNT + c;
  lcd.setCursor(LCD_COL_DEBUG_3, LCD_ROW_DEBUG);
  printKeyToLcd(key);
}

void printKeyToLcd(int key) {
  switch (key) {
    case KEY_0:
      lcd.print("0");
      break;
    case KEY_1:
      lcd.print("1");
      break;
    case KEY_2:
      lcd.print("2");
      break;
    case KEY_3:
      lcd.print("3");
      break;
    case KEY_4:
      lcd.print("4");
      break;
    case KEY_5:
      lcd.print("5");
      break;
    case KEY_6:
      lcd.print("6");
      break;
    case KEY_7:
      lcd.print("7");
      break;
    case KEY_8:
      lcd.print("8");
      break;
    case KEY_9:
      lcd.print("9");
      break;
    case KEY_ASTR:
      lcd.print("*");
      break;
    case KEY_HASH:
      lcd.print("#");
      break;
    case KEY_A:
      lcd.print("A");
      break;
    case KEY_B:
      lcd.print("B");
      break;
    case KEY_C:
      lcd.print("C");
      break;
    case KEY_D:
      lcd.print("D");
      break;
  }	
}

void debugMsg(String msg) {
  Serial.println(String(millis()) + ": " + msg);  
}

void debugBtnState(int r, int c) {
  String strBtnState = "BTN_STATE_OPENED";
  String strBtnChar  = " ";
  switch (buttonState[r][c]) {
    case BTN_STATE_CLOSING:
      strBtnState = "BTN_STATE_CLOSING";
	    strBtnChar  = "v";
      break;
    case BTN_STATE_CLOSED:
      strBtnState = "BTN_STATE_CLOSED";
	  strBtnChar  = "_";
      break;
    case BTN_STATE_OPENING:  
      strBtnState = "BTN_STATE_OPENING";
	  strBtnChar  = "^";
      break;
    default:  
      strBtnState = "BTN_STATE_OPENED";
	    strBtnChar  = " ";
      break;
  }
  debugMsg("buttonState[" + String(r) + "]["+ String(c) + "] = " + strBtnState);
  lcd.setCursor(c, r);
  lcd.print(strBtnChar);
}
