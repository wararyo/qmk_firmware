/* Copyright 2019 peraneko
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
  TD_MODE_GRV = 0,
};

// Tap dances
void dance_mode (qk_tap_dance_state_t *state, void *user_data);
void dance_mode_finished (qk_tap_dance_state_t *state, void *user_data);
void dance_mode_reset (qk_tap_dance_state_t *state, void *user_data);
qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_MODE_GRV] = ACTION_TAP_DANCE_FN_ADVANCED(dance_mode, dance_mode_finished, dance_mode_reset)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( 
        TD(TD_MODE_GRV), KC_A, KC_B, KC_C,
        KC_D, KC_E, KC_F, KC_G
    )
};

/* Rotary encoder settings */
void encoder_update_user(uint16_t index, bool clockwise) {
   if (clockwise) {
        tap_code(KC_UP);    //Rotary encoder clockwise
    } else {
        tap_code(KC_DOWN);  //Rotary encoder Reverse clockwise
    }
}

void dance_mode (qk_tap_dance_state_t *state, void *user_data) {
  if(state->count >= 3) {
    SEND_STRING ("```");
    reset_tap_dance (state);
  }
}
void dance_mode_finished (qk_tap_dance_state_t *state, void *user_data) {
  if(state->count == 1) {
    register_code (KC_ESC);
  }
  else if(state->count == 2) {
    register_code (KC_GRV);
  }
}
void dance_mode_reset (qk_tap_dance_state_t *state, void *user_data) {
  if(state->count == 1) {
    unregister_code (KC_ESC);
  }
  else if(state->count == 2) {
    unregister_code (KC_GRV);
  }
}
