#include "Arduino.h"
#include "nm_key_board.h"

NmKeyBoard::NmKeyBoard(int *pinBtnRow, int *pinBtnCol, void (*onKeyDownHandler)(int), void (*onKeyUpHandler)(int)) {
  for (int c=0; c < C_COUNT; c++) {
    _pinBtnCol[c] = pinBtnCol[c];
    pinMode(_pinBtnCol[c], INPUT_PULLUP);
  }

  for (int r=0; r < R_COUNT; r++) {
    _pinBtnRow[r] = pinBtnRow[r];
    pinMode(_pinBtnRow[r], OUTPUT);
    digitalWrite(_pinBtnRow[r], HIGH);
  }

  for (int c=0; c < C_COUNT; c++) {
    for (int r=0; r < R_COUNT; r++) {
      _buttonState[r][c] = BTN_STATE_OPENED;
    }
  } 

  this->onKeyDown = onKeyDownHandler;
  this->onKeyUp = onKeyUpHandler;
}

void NmKeyBoard::keyBoardTask() {
  unsigned long currentMillis = millis();
  if (currentMillis - _keyBoardTimer >= _keyBoardTaskTime) {
    _keyBoardTimer = currentMillis;
    for (int r=0; r < R_COUNT; r++) {
      digitalWrite(_pinBtnRow[r], LOW);
      for (int c=0; c < C_COUNT; c++) {
        processButtonState(r, c);
      }
      digitalWrite(_pinBtnRow[r], HIGH);
    }  
  }  
}

void NmKeyBoard::processButtonState(int r, int c) {
  int newPinState = digitalRead(_pinBtnCol[c]);
  unsigned long timeCurrent = millis();

  switch (_buttonState[r][c]) {
    case BTN_STATE_CLOSING:
      if (newPinState == HIGH) {
        _buttonState[r][c] = BTN_STATE_OPENED;
      }
      else if (timeCurrent - _timeBtnStateClosing >= MIN_CLOSING_TIME) {
        _buttonState[r][c] = BTN_STATE_CLOSED;
        if (this->onKeyDown) {
          this->onKeyDown(r * C_COUNT + c);
        }
      }
      break;
    case BTN_STATE_CLOSED:
      if (newPinState == HIGH) {
         _timeBtnStateOpening = millis();
         _buttonState[r][c] = BTN_STATE_OPENING;
      } 
      break;
    case BTN_STATE_OPENING:  
      if (newPinState == LOW) {
        _buttonState[r][c] = BTN_STATE_CLOSED;
      }
      else if (timeCurrent - _timeBtnStateOpening >= MIN_OPENING_TIME) {
        _buttonState[r][c] = BTN_STATE_OPENED;
        if (this->onKeyUp) {
          this->onKeyUp(r * C_COUNT + c);
        }
      }
      break;
    case BTN_STATE_OPENED:  
      if (newPinState == LOW) {
        _timeBtnStateClosing = millis();
        _buttonState[r][c] = BTN_STATE_CLOSING;
      }
      break;
  }
}
