#include <Adafruit_NeoPixel.h>

// --- Strips configuration ---
#define LED_TYPE (NEO_GRB + NEO_KHZ800)
#define STRIPS_COUNT 7
#define STRIP_1 70
#define STRIP_2 62
#define STRIP_3 46
#define STRIP_4 64
#define STRIP_5 28
#define STRIP_6 52
#define STRIP_7 50

// --- Fire effect configuration ---
#define RED 0xFF0000

uint8_t brightness = 100;

// --- Strips configuration ---
Adafruit_NeoPixel left_strips[STRIPS_COUNT] = {
    Adafruit_NeoPixel(STRIP_1, 16, LED_TYPE),
    Adafruit_NeoPixel(STRIP_2, 17, LED_TYPE),
    Adafruit_NeoPixel(STRIP_3, 18, LED_TYPE),
    Adafruit_NeoPixel(STRIP_4, 19, LED_TYPE),
    Adafruit_NeoPixel(STRIP_5, 21, LED_TYPE),
    Adafruit_NeoPixel(STRIP_6, 22, LED_TYPE),
    Adafruit_NeoPixel(STRIP_7, 23, LED_TYPE)
};

Adafruit_NeoPixel right_strips[STRIPS_COUNT] = {
    Adafruit_NeoPixel(STRIP_1, 13, LED_TYPE),
    Adafruit_NeoPixel(STRIP_2, 14, LED_TYPE),
    Adafruit_NeoPixel(STRIP_3, 27, LED_TYPE),
    Adafruit_NeoPixel(STRIP_4, 26, LED_TYPE),
    Adafruit_NeoPixel(STRIP_5, 25, LED_TYPE),
    Adafruit_NeoPixel(STRIP_6, 33, LED_TYPE),
    Adafruit_NeoPixel(STRIP_7, 32, LED_TYPE)
};

// --- Lifecycle functions ---
void setup() {
    Serial.begin(115200);
    randomSeed(analogRead(0));

    for (uint8_t i = 0; i < STRIPS_COUNT; i++) {
        left_strips[i].begin();
        left_strips[i].fill(RED);
        left_strips[i].show();

        right_strips[i].begin();
        right_strips[i].fill(RED);
        right_strips[i].show();
    }
}

void loop() {
    //brightness = map(analogRead(A0), 0, 1023, 0, 255); //TODO later

    const uint8_t new_brightness = brightness;

    if (brightness != new_brightness) return;
    
    for (uint8_t i = 0; i < STRIPS_COUNT; i++) {
        left_strips[i].setBrightness(brightness);
        left_strips[i].show();

        right_strips[i].setBrightness(brightness);
        right_strips[i].show();
    }
}
