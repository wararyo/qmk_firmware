#include QMK_KEYBOARD_H
#include <stdio.h>
#include <string.h>

#define PRO_MICRO_LED_TX D5
#define PRO_MICRO_LED_RX B0

enum custom_keycodes {
  BASE = SAFE_RANGE,
  PLUS5,
  PLUS1,
  EXECUTE
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT( /* Base */
    BASE,  PLUS5,  PLUS1, EXECUTE \
  )
};

uint8_t plusNumber = 0;
char numberStr[16] = {'\0'};

void showNumber(void) {
  // 残っている文字を消す
  for(int i=0;i < strlen(numberStr);i++) tap_code(KC_BSPC);

  // 現在のplusNumberの値を表示する
  snprintf(numberStr, sizeof(numberStr), "+%d", plusNumber);
  send_string(numberStr);
}

void resetNumber(void) {
  // リセット
  plusNumber = 0;
  numberStr[0] = '\0';
  writePin(PRO_MICRO_LED_TX, true); //LED消灯
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch(keycode){
    case BASE: //コマンドの基本部分を入力する
      if(record->event.pressed){
        //pressed
        resetNumber();
        SEND_STRING("/r 2d6");
      } else {
        //released
      }
      break;
    case PLUS5: //補正値を5増やす
      if(record->event.pressed){
        //pressed
        plusNumber += 5;
        showNumber();
        writePin(PRO_MICRO_LED_TX, false); //LED点灯
      } else {
        //released
      }
      break;
    case PLUS1: //補正値を1増やす
      if(record->event.pressed){
        //pressed
        plusNumber += 1;
        showNumber();
        writePin(PRO_MICRO_LED_TX, false); //LED点灯
      } else {
        //released
      }
      break;
    case EXECUTE: //ENTERを押し、諸々をリセット
      if(record->event.pressed){
        //pressed
        tap_code(KC_ENTER);
        resetNumber();
      } else {
        //released
      }
      break;
  }

  return true;
}

void matrix_init_user(void) {
  setPinOutput(PRO_MICRO_LED_TX);
  setPinOutput(PRO_MICRO_LED_RX);
}

void matrix_scan_user(void) {

}

bool led_update_user(led_t led_state) {
  writePin(PRO_MICRO_LED_TX, true); //LED消灯
  writePin(PRO_MICRO_LED_RX, true); //LED消灯
  return true;
}
