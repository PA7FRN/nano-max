#include <LiquidCrystal_I2C.h>

#include "nm_key_board.h"

#define LCD_ROW_DEBUG    3
#define LCD_ROW_COUNT    4
#define LCD_COL_DEBUG_1  0
#define LCD_COL_DEBUG_2  5
#define LCD_COL_DEBUG_3  9
#define LCD_COL_DEBUG_4 11
#define LCD_COL_DEBUG_5 16
#define LCD_COL_COUNT   20

int pinBtnRow[R_COUNT] = {9,8,7,6}; //  D6 = header pin 9, D7 = pin 10 ...
int pinBtnCol[C_COUNT] = {5,4,3,2}; //  D2 = header pin 5, D3 = pin 6 ...
const int pinLed       = 13; // D13 = on board LED

void keyDown(int key);
void keyUp(int key);

// set the LCD address to 0x27 
// A FEW use address 0x3F
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  

NmKeyBoard keyBoard(pinBtnRow, pinBtnCol, keyDown, keyUp);

void setup() {
  Serial.begin(9600); 
  lcd.begin(LCD_COL_COUNT, LCD_ROW_COUNT);

  pinMode(pinLed, OUTPUT);
  
  lcd.setCursor(LCD_COL_DEBUG_2, LCD_ROW_DEBUG);
  lcd.print("up:");
  lcd.setCursor(LCD_COL_DEBUG_4, LCD_ROW_DEBUG);
  lcd.print("down:");
}

void loop() {
  keyBoard.keyBoardTask();
}

void keyDown(int key) {
  lcd.setCursor(LCD_COL_DEBUG_5, LCD_ROW_DEBUG);
  printKeyToLcd(key);
}

void keyUp(int key) {
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
