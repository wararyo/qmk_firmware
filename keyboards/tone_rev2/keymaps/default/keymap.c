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

#define PRO_MICRO_LED_TX D5
#define PRO_MICRO_LED_RX B0

enum keymap_layer {
    KL_WINDOWS,
    KL_CUBASE,
    KL_LIGHTROOM,
    keymap_length
};

enum custom_keycodes {
    NEXT_LAYER = SAFE_RANGE
};

void blinkLed(uint8_t count);

uint8_t currentLayer = 0; // 初期化時にEEPROMから読み込まれる

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // 汎用キーマップ (Windows)
    // レイヤーを切り替え / ウィンドウ一覧を表示 / 前のウィンドウ / 次のウィンドウ
    // 戻る / 進む / 前のタブ / 次のタブ
    [KL_WINDOWS] = LAYOUT(
        NEXT_LAYER, LGUI(KC_UP), LALT(LSFT(KC_TAB)), LALT(KC_TAB),
        LALT(KC_LEFT), LALT(KC_RIGHT), LCTL(LSFT(KC_TAB)), LCTL(KC_TAB)
    ),
    // Cubase向けキーマップ
    // レイヤーを切り替え / MixConsoleを開く / エディターを開く / コードパッドを開く
    // 選択ツール / 鉛筆ツール / はさみツール / のりツール
    [KL_CUBASE] = LAYOUT(
        NEXT_LAYER, LALT(KC_F3), KC_ENT, LCTL(LSFT(KC_C)),
        KC_1, KC_8, KC_3, KC_4
    ),
    // Lightroom Classic向けキーマップ
    // レイヤーを切り替え / クリップの表示 / 現像設定のコピー / 現像設定の貼り付け
    // 削除 / Enter / 前の写真 / 次の写真
    [KL_LIGHTROOM] = LAYOUT(
        NEXT_LAYER, KC_J, LCTL(LSFT(KC_C)), LCTL(LSFT(KC_V)),
        KC_DEL, KC_ENTER, KC_LEFT, KC_RIGHT
    )
};

/* Rotary encoder settings */
void encoder_update_user(uint16_t index, bool clockwise) {
   if (clockwise) {
        tap_code(KC_MS_WH_LEFT);    //Rotary encoder clockwise
    } else {
        tap_code(KC_MS_WH_RIGHT);  //Rotary encoder Reverse clockwise
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if(record->event.pressed && keycode == NEXT_LAYER) {
        // 次のレイヤーへ移動
        currentLayer++;
        if(currentLayer == keymap_length) currentLayer = 0;
        set_single_persistent_default_layer(currentLayer); // この時EEPROMに記録されるため再起動してもレイヤー設定は保持される
        blinkLed(currentLayer+1); // LEDの点滅でレイヤーIDを表す
    }
    return true;
}

// LEDの点滅
uint16_t ledLastChecked = 0; // 最後にLEDの状態をチェックした時間
bool ledValue = false; // falseで消灯 trueで点灯
uint8_t ledCountToBlink = 0;

void blinkLed(uint8_t count) {
    ledCountToBlink = count;
}

void matrix_init_user(void) {
    setPinOutput(PRO_MICRO_LED_TX);
    setPinOutput(PRO_MICRO_LED_RX);
    writePin(PRO_MICRO_LED_TX, !ledValue); //LED消灯
    writePin(PRO_MICRO_LED_RX, !ledValue); //LED消灯
    ledLastChecked = timer_read();
    // EEPROMに記録されたレイヤー設定を復元
    uint8_t defaultLayer = eeconfig_read_default_layer();
    for(uint8_t i=0;i<keymap_length;i++) {
        if(defaultLayer == 1U << i) {
            currentLayer = i;
            break;
        }
    }
    // 起動時にもLED点滅で現在のレイヤー設定を表す
    blinkLed(currentLayer+1);
}

void matrix_scan_user(void) {
    if(ledCountToBlink > 0 && timer_elapsed(ledLastChecked) > 100) {
        ledLastChecked = timer_read();
        ledValue = !ledValue;
        writePin(PRO_MICRO_LED_TX, !ledValue); // writePinはtrueだと消灯 falseだと点灯
        writePin(PRO_MICRO_LED_RX, !ledValue);
        if(ledValue == false) ledCountToBlink--;
    }
}

bool led_update_user(led_t led_state) {
    writePin(PRO_MICRO_LED_TX, !ledValue); // writePinはtrueだと消灯 falseだと点灯
    writePin(PRO_MICRO_LED_RX, !ledValue);
    return true;
}
