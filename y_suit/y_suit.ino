/**
 * Yukichi LED Controller
 *
 * 接続:
 * - 左腕: Pin 3 (36個)
 * - 左胴: Pin 4 (27個)  
 * - 右胴: Pin 6 (27個)
 * - 右腕: Pin 7 (36個)
 * - 右足: Pin 8 (21個)
 * - 左足: Pin 9 (21個)
 */

#include <Adafruit_NeoPixel.h>

// ==================== 設定値 ====================
// LED設定
#define BRIGHTNESS 100

// LED個数
#define ARM_LEFT_LED   36
#define BODY_LEFT_LED  27
#define BODY_RIGHT_LED 27
#define ARM_RIGHT_LED  36
#define LEG_RIGHT_LED  21
#define LEG_LEFT_LED   21

// ピン番号
#define PIN_ARM_LEFT   3
#define PIN_BODY_LEFT  4
#define PIN_BODY_RIGHT 6
#define PIN_ARM_RIGHT  7
#define PIN_LEG_RIGHT  8
#define PIN_LEG_LEFT   9

// モード設定 (1:本番モード 2:開発モード)
#define MODE 1

// ==================== タイミング設定 ====================
// じわじわ光る（消える）
#define PULSE_WHITE_WAIT      7500  // 開始待ち時間
#define PULSE_WHITE_WAIT2     1500  // 部位間待ち時間
#define PULSE_WHITE_TIME      1     // 点滅回数
#define PULSE_WHITE_SPACE     5     // 点滅間隔
#define PULSE_WHITE_SPEED     1     // 点灯速度

// ランダム点灯
#define RANDOM_ALL_WAIT       1000  // 1回目の待ち時間
#define RANDOM_ALL_WAIT2      14000 // 2回目の待ち時間  
#define RANDOM_ALL_TIME       25    // 点灯回数
#define RANDOM_ALL_SPACE      50    // 点灯間隔
#define RANDOM_ALL_NUM        5     // 同時点灯個数

// 下から上への点灯
#define WAVE_FOOT_WAIT        500   // 開始待ち時間
#define WAVE_TIME             10    // 点灯間隔

// 衣装切り替え
#define SUITS_CHANGE_WAIT     8000  // 開始待ち時間
#define SUITS_CHANGE_TIME     2     // 切替回数
#define SUITS_CHANGE_SPACE    2000  // 切替間隔

// じわじわ光る（消えない）
#define PULSE_HOLD_WAIT       0     // 開始待ち時間
#define PULSE_HOLD_WAIT2      1000  // 部位間待ち時間
#define PULSE_HOLD_TIME       1     // 点滅回数
#define PULSE_HOLD_SPACE      5     // 点滅間隔
#define PULSE_HOLD_SPEED      3     // 点灯速度

// 中央への集光
#define LED_CENTER_WAIT       1500  // 開始待ち時間
#define LED_CENTER_SPEED      20    // 点灯速度
#define LED_CENTER_NUM_MOVE   5     // 移動個数
#define LED_CENTER_TIME       3     // 繰り返し回数
#define LED_CENTER_OFFSET     2     // 位置調整

// シアターチェイス
#define THEATER_CHASE_WAIT    4500  // 開始待ち時間
#define THEATER_CHASE_SPEED   20    // 点灯速度
#define THEATER_CHASE_TIME    60    // 繰り返し回数

// ==================== グローバル変数 ====================
// LEDストリップオブジェクト
Adafruit_NeoPixel armLeft(ARM_LEFT_LED, PIN_ARM_LEFT, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel bodyLeft(BODY_LEFT_LED, PIN_BODY_LEFT, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel bodyRight(BODY_RIGHT_LED, PIN_BODY_RIGHT, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel armRight(ARM_RIGHT_LED, PIN_ARM_RIGHT, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel legRight(LEG_RIGHT_LED, PIN_LEG_RIGHT, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel legLeft(LEG_LEFT_LED, PIN_LEG_LEFT, NEO_GRBW + NEO_KHZ800);

// ガンマ補正テーブル
const byte neopix_gamma[] PROGMEM = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255
};

// ==================== 初期設定 ====================
void setup() {
  initializeLED(&armLeft);
  initializeLED(&bodyLeft);
  initializeLED(&bodyRight);
  initializeLED(&armRight);
  initializeLED(&legRight);
  initializeLED(&legLeft);
}

void initializeLED(Adafruit_NeoPixel* strip) {
  strip->begin();
  strip->setBrightness(BRIGHTNESS);
  strip->show();
}

// ==================== メインループ ====================
void loop() {
  if (MODE == 1) {
    performMainSequence();
  } else if (MODE == 2) {
    performDebugSequence();
  }
}

// ==================== シーケンス制御 ====================
void performMainSequence() {
  // 1. じわじわ光る（脛→もも→腰→アーム）消える
  delay(PULSE_WHITE_WAIT);
  pulseWhite_series_sune(PULSE_WHITE_TIME, PULSE_WHITE_SPACE, PULSE_WHITE_SPEED);
  delay(PULSE_WHITE_WAIT2);
  pulseWhite_series_momo(PULSE_WHITE_TIME, PULSE_WHITE_SPACE, PULSE_WHITE_SPEED);
  delay(PULSE_WHITE_WAIT2);
  pulseWhite_series_body(PULSE_WHITE_TIME, PULSE_WHITE_SPACE, PULSE_WHITE_SPEED);
  delay(PULSE_WHITE_WAIT2);
  pulseWhite_arm(PULSE_WHITE_TIME, PULSE_WHITE_SPACE, PULSE_WHITE_SPEED);

  // 2. ランダム点灯（1回目）
  delay(RANDOM_ALL_WAIT);
  random_all(getWhiteColor(), RANDOM_ALL_TIME, RANDOM_ALL_SPACE, RANDOM_ALL_NUM);

  // 3. 下から上への点灯
  delay(WAVE_FOOT_WAIT);
  colorWipeRange_wave_foot(getWhiteColor(), WAVE_TIME);
  colorWipeRange_wave_bodyarm(getWhiteColor(), WAVE_TIME);

  // 4. ランダム点灯（2回目）
  delay(RANDOM_ALL_WAIT2);
  setAllColor(0);
  random_all(getWhiteColor(), RANDOM_ALL_TIME, RANDOM_ALL_SPACE, RANDOM_ALL_NUM);
  setAllColor(0);

  // 5. 衣装切り替え
  delay(SUITS_CHANGE_WAIT);
  suits_led_change(getWhiteColor(), 0, SUITS_CHANGE_TIME, SUITS_CHANGE_SPACE);

  // 6. じわじわ光る（脛→もも→腰→アーム）消えない
  delay(PULSE_HOLD_WAIT);
  pulseWhite_series_sune_hold(PULSE_HOLD_TIME, PULSE_HOLD_SPACE, PULSE_HOLD_SPEED);
  delay(PULSE_HOLD_WAIT2);
  pulseWhite_series_momo_hold(PULSE_HOLD_TIME, PULSE_HOLD_SPACE, PULSE_HOLD_SPEED);
  delay(PULSE_HOLD_WAIT2);
  pulseWhite_series_body_hold(PULSE_HOLD_TIME, PULSE_HOLD_SPACE, PULSE_HOLD_SPEED);
  delay(PULSE_HOLD_WAIT2);
  pulseWhite_arm_hold(PULSE_HOLD_TIME, PULSE_HOLD_SPACE, PULSE_HOLD_SPEED);

  // 7. 中央への集光
  delay(LED_CENTER_WAIT);
  setAllColor(0);
  LEDtoCenter(getWhiteColor(), LED_CENTER_SPEED, ARM_LEFT_LED, 
              LED_CENTER_NUM_MOVE, LED_CENTER_TIME, LED_CENTER_OFFSET);
  setAllColor(0);

  // 8. パチ屋
  delay(THEATER_CHASE_WAIT);
  theaterChase(getWhiteColor(), THEATER_CHASE_SPEED, THEATER_CHASE_TIME);

  delay(10000000);
}

void performDebugSequence() {
  setAllColor(0);
  LEDtoCenter(getWhiteColor(), LED_CENTER_SPEED, ARM_LEFT_LED, 
              LED_CENTER_NUM_MOVE, LED_CENTER_TIME, LED_CENTER_OFFSET);
  setAllColor(0);
  theaterChase(getWhiteColor(), THEATER_CHASE_SPEED, THEATER_CHASE_TIME);
  delay(10000000);
}

// ==================== 色設定関数 ====================
uint32_t getWhiteColor() {
  return armLeft.Color(0, 0, 0, 255);
}

void setAllColor(uint32_t color) {
  setStripColor(&armLeft, color, 0, armLeft.numPixels());
  setStripColor(&bodyLeft, color, 0, bodyLeft.numPixels());
  setStripColor(&bodyRight, color, 0, bodyRight.numPixels());
  setStripColor(&armRight, color, 0, armRight.numPixels());
  setStripColor(&legRight, color, 0, legRight.numPixels());
  setStripColor(&legLeft, color, 0, legLeft.numPixels());
}

void setStripColor(Adafruit_NeoPixel* strip, uint32_t color, uint16_t start, uint16_t end) {
  for (uint16_t i = start; i < end; i++) {
    strip->setPixelColor(i, color);
  }
  strip->show();
}

void setarmLeftColor(uint32_t color) {
  setStripColor(&armLeft, color, 0, armLeft.numPixels());
}

void setarmRightColor(uint32_t color) {
  setStripColor(&armRight, color, 0, armRight.numPixels());
}

void setbodyLeftColor(uint32_t color) {
  setStripColor(&bodyLeft, color, 0, bodyLeft.numPixels());
}

void setbodyRightColor(uint32_t color) {
  setStripColor(&bodyRight, color, 0, bodyRight.numPixels());
}

void setlegRightColor(uint32_t color) {
  setStripColor(&legRight, color, 0, legRight.numPixels() - 10);
}

void setlegLeftColor(uint32_t color) {
  setStripColor(&legLeft, color, 0, legLeft.numPixels() - 10);
}

void setlegRightColor_sune(uint32_t color) {
  setStripColor(&legRight, color, 11, legRight.numPixels());
}

void setlegLeftColor_sune(uint32_t color) {
  setStripColor(&legLeft, color, 11, legLeft.numPixels());
}

// ==================== パルス効果 ====================
void pulseEffect(void (*setColorFunc1)(uint32_t), void (*setColorFunc2)(uint32_t), 
                 uint16_t time, uint8_t wait, uint8_t speed, bool fadeOut) {
  for (uint16_t k = 0; k < time; k++) {
    // フェードイン
    for (int j = 0; j < 256; j += speed) {
      uint8_t val = pgm_read_byte(&neopix_gamma[j]);
      uint32_t color = armLeft.Color(0, 0, 0, val);
      setColorFunc1(color);
      if (setColorFunc2 != NULL) setColorFunc2(color);
      delay(wait);
    }
    // フェードアウト
    if (fadeOut) {
      for (int j = 255; j >= 0; j -= 4) {
        uint8_t val = pgm_read_byte(&neopix_gamma[j]);
        uint32_t color = armLeft.Color(0, 0, 0, val);
        setColorFunc1(color);
        if (setColorFunc2 != NULL) setColorFunc2(color);
        delay(wait);
      }
    }
    delay(100);
  }
}

// 脛（すね）光る
void pulseWhite_series_sune(uint16_t time, uint8_t wait, uint8_t speed) {
  pulseEffect(setlegRightColor_sune, setlegLeftColor_sune, time, wait, speed, true);
}

// 太もも光る
void pulseWhite_series_momo(uint16_t time, uint8_t wait, uint8_t speed) {
  pulseEffect(setlegRightColor, setlegLeftColor, time, wait, speed, true);
}

// 腰光る
void pulseWhite_series_body(uint16_t time, uint8_t wait, uint8_t speed) {
  pulseEffect(setbodyLeftColor, setbodyRightColor, time, wait, speed, true);
}

// アーム光る
void pulseWhite_arm(uint16_t time, uint8_t wait, uint8_t speed) {
  pulseEffect(setarmRightColor, setarmLeftColor, time, wait, speed, true);
}

// 脛光る（ホールド）
void pulseWhite_series_sune_hold(uint16_t time, uint8_t wait, uint8_t speed) {
  pulseEffect(setlegRightColor_sune, setlegLeftColor_sune, time, wait, speed, false);
}

// 太もも光る（ホールド）
void pulseWhite_series_momo_hold(uint16_t time, uint8_t wait, uint8_t speed) {
  pulseEffect(setlegRightColor, setlegLeftColor, time, wait, speed, false);
}

// 腰光る（ホールド）
void pulseWhite_series_body_hold(uint16_t time, uint8_t wait, uint8_t speed) {
  pulseEffect(setbodyLeftColor, setbodyRightColor, time, wait, speed, false);
}

// アーム光る（ホールド）
void pulseWhite_arm_hold(uint16_t time, uint8_t wait, uint8_t speed) {
  pulseEffect(setarmRightColor, setarmLeftColor, time, wait, speed, false);
}

// ==================== ランダム点灯 ====================
void random_all(uint32_t c, uint16_t time, uint8_t wait, uint8_t num) {
  for (uint16_t t = 0; t <= time; t++) {
    // ランダム位置を生成して点灯
    uint32_t positions[6];
    positions[0] = random(0, ARM_LEFT_LED);
    positions[1] = random(0, BODY_LEFT_LED);
    positions[2] = random(0, BODY_RIGHT_LED);
    positions[3] = random(0, ARM_RIGHT_LED);
    positions[4] = random(0, LEG_RIGHT_LED);
    positions[5] = random(0, LEG_LEFT_LED);
    
    // 点灯
    setRandomPixels(&armLeft, positions[0], num, c);
    setRandomPixels(&bodyLeft, positions[1], num, c);
    setRandomPixels(&bodyRight, positions[2], num, c);
    setRandomPixels(&armRight, positions[3], num, c);
    setRandomPixels(&legRight, positions[4], num, c);
    setRandomPixels(&legLeft, positions[5], num, c);
    
    delay(wait);
    
    // 消灯
    setRandomPixels(&armLeft, positions[0], num, 0);
    setRandomPixels(&bodyLeft, positions[1], num, 0);
    setRandomPixels(&bodyRight, positions[2], num, 0);
    setRandomPixels(&armRight, positions[3], num, 0);
    setRandomPixels(&legRight, positions[4], num, 0);
    setRandomPixels(&legLeft, positions[5], num, 0);
  }
}

void setRandomPixels(Adafruit_NeoPixel* strip, uint32_t pos, uint8_t num, uint32_t color) {
  uint16_t maxPos = strip->numPixels();
  for (uint16_t i = pos; i < pos + num && i < maxPos; i++) {
    strip->setPixelColor(i, color);
  }
  strip->show();
}

// ==================== ウェーブ効果 ====================
void colorWipeRange_wave_foot(uint32_t c, uint32_t wait) {
  for (uint16_t i = LEG_RIGHT_LED; i > 0; i--) {
    legRight.setPixelColor(i, c);
    legLeft.setPixelColor(i, c);
    legRight.show();
    legLeft.show();
    delay(wait);
  }
}

void colorWipeRange_wave_bodyarm(uint32_t c, uint32_t wait) {
  uint32_t g = ARM_LEFT_LED / 2;
  
  for (uint16_t i = ARM_LEFT_LED; i > 0; i--) {
    // ボディー部分
    bodyLeft.setPixelColor(i, c);
    bodyRight.setPixelColor(i, c);
    bodyLeft.show();
    bodyRight.show();
    delay(wait);
    
    // アーム部分
    if (i >= ARM_LEFT_LED / 2) {
      uint16_t armPos = i - (ARM_LEFT_LED / 2 + 1);
      armLeft.setPixelColor(armPos, c);
      armRight.setPixelColor(armPos, c);
      armLeft.setPixelColor(g, c);
      armRight.setPixelColor(g, c);
      armLeft.show();
      armRight.show();
      g++;
      delay(wait);
    }
  }
}

// ==================== 衣装切り替え ====================
void suits_led_change(uint32_t colorOn, uint32_t colorOff, uint32_t time, uint32_t wait) {
  for (uint16_t k = 0; k < time; k++) {
    setAllColor(colorOn);
    delay(wait);
    setAllColor(colorOff);
    delay(wait);
  }
}

// ==================== 中央への集光 ====================
void LEDtoCenter(uint32_t c, uint8_t wait, uint8_t num, uint8_t numMove, 
                 uint8_t time, uint8_t offset) {
  for (uint16_t t = 0; t < time; t++) {
    uint8_t i = 0;
    uint8_t g = 0;
    uint8_t u = LEG_RIGHT_LED;
    
    for (uint16_t k = num; k > 0; k--) {
      // ボディー上から下
      if (i < (BODY_LEFT_LED / 2) - numMove - offset) {
        bodyLeft.setPixelColor(i, c);
        bodyRight.setPixelColor(i, c);
        bodyLeft.show();
        bodyRight.show();
        i++;
      }
      
      // ボディー下から上
      if (k >= (BODY_LEFT_LED / 2) - numMove - offset) {
        bodyLeft.setPixelColor(k, c);
        bodyRight.setPixelColor(k, c);
        bodyLeft.show();
        bodyRight.show();
      }
      
      // アーム下から上
      if (k >= ARM_LEFT_LED / 2) {
        uint16_t armPos = k - (ARM_LEFT_LED + 2) / 2;
        armLeft.setPixelColor(armPos, c);
        armRight.setPixelColor(armPos, c);
        armLeft.show();
        armRight.show();
      }
      
      // 足下から上
      if (u <= k) {
        legRight.setPixelColor(u, c);
        legLeft.setPixelColor(u, c);
        legRight.show();
        legLeft.show();
        u--;
      }
      
      // 前のLEDを消灯
      clearPreviousLEDs(k, g, numMove);
      delay(wait);
      g++;
    }
  }
}

void clearPreviousLEDs(uint16_t k, uint8_t g, uint8_t numMove) {
  bodyLeft.setPixelColor(g - numMove, 0);
  bodyRight.setPixelColor(g - numMove, 0);
  bodyLeft.setPixelColor(k + numMove, 0);
  bodyRight.setPixelColor(k + numMove, 0);
  armLeft.setPixelColor(k - numMove - (19 - numMove * 2), 0);
  armRight.setPixelColor(k - numMove - (19 - numMove * 2), 0);
  legRight.setPixelColor(k - numMove - (15 - numMove * 2), 0);
  legLeft.setPixelColor(k - numMove - (15 - numMove * 2), 0);
  
  armLeft.show();
  bodyLeft.show();
  bodyRight.show();
  armRight.show();
  legRight.show();
  legLeft.show();
}

// ==================== シアターチェイス ====================
void theaterChase(uint32_t c, uint8_t wait, uint8_t cycle) {
  for (int j = 0; j < cycle; j++) {
    for (int q = 0; q < 3; q++) {
      // 3つおきにLEDを点灯
      for (uint16_t i = 0; i < ARM_RIGHT_LED; i = i + 3) {
        setTheaterPixel(&armLeft, i + q, c);
        setTheaterPixel(&bodyLeft, i + q, c);
        setTheaterPixel(&bodyRight, i + q, c);
        setTheaterPixel(&armRight, i + q, c);
        setTheaterPixel(&legRight, i + q, c);
        setTheaterPixel(&legLeft, i + q, c);
      }
      showAllStrips();
      delay(wait);
      
      // 消灯
      for (uint16_t i = 0; i < ARM_RIGHT_LED; i = i + 3) {
        setTheaterPixel(&armLeft, i + q, 0);
        setTheaterPixel(&bodyLeft, i + q, 0);
        setTheaterPixel(&bodyRight, i + q, 0);
        setTheaterPixel(&armRight, i + q, 0);
        setTheaterPixel(&legRight, i + q, 0);
        setTheaterPixel(&legLeft, i + q, 0);
      }
      showAllStrips();
    }
  }
}

void setTheaterPixel(Adafruit_NeoPixel* strip, uint16_t pos, uint32_t color) {
  if (pos < strip->numPixels()) {
    strip->setPixelColor(pos, color);
  }
}

// ==================== ユーティリティ関数 ====================
// 全ストリップを更新
void showAllStrips() {
  armLeft.show();
  bodyLeft.show();
  bodyRight.show();
  armRight.show();
  legRight.show();
  legLeft.show();
}

// ==================== レインボー効果 ====================
void rainbowCycleAll(uint8_t wait) {
  uint16_t maxPixels = max(max(max(ARM_LEFT_LED, BODY_LEFT_LED), ARM_RIGHT_LED), LEG_RIGHT_LED);
  
  for (uint16_t j = 0; j < 256 * 5; j++) {
    setRainbowStrip(&armLeft, j, maxPixels);
    setRainbowStrip(&bodyLeft, j, maxPixels);
    setRainbowStrip(&bodyRight, j, maxPixels);
    setRainbowStrip(&armRight, j, maxPixels);
    setRainbowStrip(&legRight, j, maxPixels);
    setRainbowStrip(&legLeft, j, maxPixels);
    showAllStrips();
    delay(wait);
  }
}

void setRainbowStrip(Adafruit_NeoPixel* strip, uint16_t j, uint16_t maxPixels) {
  for (uint16_t i = 0; i < strip->numPixels(); i++) {
    strip->setPixelColor(i, Wheel(((i * 256 / maxPixels) + j) & 255));
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return armLeft.Color(255 - WheelPos * 3, 0, WheelPos * 3, 0);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return armLeft.Color(0, WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  WheelPos -= 170;
  return armLeft.Color(WheelPos * 3, 255 - WheelPos * 3, 0, 0);
}
