#ifndef NM_KEY_BOARD_H
#define NM_KEY_BOARD_H

#include "Arduino.h"

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

class NmKeyBoard {
  public:
    NmKeyBoard(int *pinBtnRow, int *pinBtnCol, void (*onKeyDown)(int), void (*onKeyUp)(int));
    void keyBoardTask();
  private:
    int _pinBtnRow[R_COUNT];
    int _pinBtnCol[C_COUNT];
    int _buttonState[R_COUNT][C_COUNT];
    unsigned long _timeBtnStateClosing = 0;
    unsigned long _timeBtnStateOpening = 0;
    unsigned long _keyBoardTimer = 0;
    unsigned long _keyBoardTaskTime  = 10; // milliseconds
    void processButtonState(int r, int c);
    void buttonDown(int r, int c);
    void buttonUp(int r, int c);
    void (*onKeyDown)(int);
    void (*onKeyUp)(int);
};

#endif
