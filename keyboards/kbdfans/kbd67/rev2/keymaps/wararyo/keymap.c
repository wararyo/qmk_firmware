/* Copyright 2018 'mechmerlin'
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

// Defines the keycodes used by our macros in process_record_user
enum tap_dances {
  TD_ESC_GRV = 0,
};

// Tap dances
void dance_esc (qk_tap_dance_state_t *state, void *user_data);
void dance_esc_finished (qk_tap_dance_state_t *state, void *user_data);
void dance_esc_reset (qk_tap_dance_state_t *state, void *user_data);
qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_ESC_GRV] = ACTION_TAP_DANCE_FN_ADVANCED(dance_esc, dance_esc_finished, dance_esc_reset)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap (Base Layer) Default Layer
   * ,----------------------------------------------------------------.
   * |Esc|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|Backsp |F12 |
   * |----------------------------------------------------------------|
   * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|  \  |F7  |
   * |----------------------------------------------------------------|
   * |Win    |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|Return |F5  |
   * |----------------------------------------------------------------|
   * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift | Up|F3  |
   * |----------------------------------------------------------------|
   * |Ctrl|Alt |NoConv|        Space          |Conv| FN  |Lef|Dow|Rig |
   * `----------------------------------------------------------------'
   */
[0] = LAYOUT_65_ansi_blocker(
  TD(TD_ESC_GRV),KC_1,KC_2,KC_3,KC_4,KC_5,KC_6,KC_7,KC_8,KC_9,KC_0,KC_MINS,KC_EQL,KC_BSPC,KC_F12,\
  LT(1,KC_TAB),KC_Q,KC_W,KC_E,KC_R,KC_T,KC_Y,KC_U,KC_I,KC_O,KC_P,KC_LBRC,KC_RBRC,KC_BSLS,KC_F7,\
  KC_LGUI,KC_A,KC_S,KC_D,KC_F,KC_G,KC_H,KC_J,KC_K,KC_L,KC_SCLN,KC_QUOT,KC_ENT,KC_F5,\
  KC_LSFT,KC_Z,KC_X,KC_C,KC_V,KC_B,KC_N,KC_M,KC_COMM,KC_DOT,KC_SLSH,KC_RCTL,KC_UP,KC_F3,\
  KC_LCTL,KC_LALT,KC_MHEN,KC_SPC,KC_HENK,TT(1),KC_LEFT,KC_DOWN,KC_RGHT),

  /* Keymap Fn Layer
   * ,----------------------------------------------------------------.
   * |Esc|F1 |F2 |F3 |F4 |F5 |F6 |F7 |F8 |F9 |F10|F11|F12|Del    |PrSc|
   * |----------------------------------------------------------------|
   * |     |   |Up |   |   |   |   |   |7  |8  |9  |   |   |     |Home|
   * |----------------------------------------------------------------|
   * |      |Lef|Dow|Rig|   |   |   |   |4  |5  |6  |:  |        |PgUp|
   * |----------------------------------------------------------------|
   * |        |   |   |   |   |   |   |   |1  |2  |3  |      |WUp|PgDn|
   * |----------------------------------------------------------------|
   * |    |    |    |                       |   |   |    |WLf|WDn|WRig|
   * `----------------------------------------------------------------'
   */
[1] = LAYOUT_65_ansi_blocker(
  KC_ESC,KC_F1,KC_F2,KC_F3,KC_F4,KC_F5,KC_F6,KC_F7,KC_F8,KC_F9,KC_F10,KC_F11,KC_F12,KC_DEL,KC_PSCR,\
  _______,_______,KC_UP,_______,_______,_______,_______,KC_P7,KC_P8,KC_P9,_______,_______,_______,KC_MPLY,KC_HOME,\
  _______,KC_LEFT,KC_DOWN,KC_RGHT,_______,_______,_______,KC_P4,KC_P5,KC_P6,KC_COLN,_______,KC_ENT,KC_PGUP,\
  _______,_______,_______,_______,_______,_______,_______,KC_P1,KC_P2,KC_P3,_______,KC_APP,LGUI(KC_UP),KC_PGDN,\
  RGB_MOD,RGB_HUI,RGB_SAI,KC_P0,KC_PDOT,KC_TRNS,LGUI(KC_LEFT),LGUI(KC_DOWN),LGUI(KC_RGHT)),

};

void dance_esc (qk_tap_dance_state_t *state, void *user_data) {
  if(state->count >= 3) {
    SEND_STRING ("```");
    reset_tap_dance (state);
  }
}
void dance_esc_finished (qk_tap_dance_state_t *state, void *user_data) {
  if(state->count == 1) {
    register_code (KC_ESC);
  }
  else if(state->count == 2) {
    register_code (KC_GRV);
  }
}
void dance_esc_reset (qk_tap_dance_state_t *state, void *user_data) {
  if(state->count == 1) {
    unregister_code (KC_ESC);
  }
  else if(state->count == 2) {
    unregister_code (KC_GRV);
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  return true;
}

void matrix_init_user(void) {

}

void matrix_scan_user(void) {

}

void led_set_user(uint8_t usb_led) {

}

layer_state_t layer_state_set_user(layer_state_t state) {
  switch(biton32(state)) {
    case 0:
      rgblight_disable();
      break;
    case 1:
      rgblight_enable();
    break;
  }
  return state;
}
