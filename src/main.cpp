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
#define BASE_COLOR 0xFF0000
#define TIP_COLOR 0x280059

#define FLAME_SIZE 10 // Flame size in pixels (0 - shortest led strip concerned by the effect)
#define FLAME_TIP_SIZE 3 // Flame tip size in pixels (0 - FLAME_SIZE)
#define COOLING 50 // Cooling factor (0 - no cooling, 255 - maximum cooling)
#define SPARKING 50 // Spark chance (0 - no sparks, 255 - maximum sparks)
#define ANIMATION_DELAY 125 // Animation delay in milliseconds

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

// --- Global variables for fire effect ---
uint8_t MASTER_BRIGHTNESS = 150;
uint8_t MIN_FLAME_BRIGHTNESS = MASTER_BRIGHTNESS * 0.2;
uint8_t heat[STRIPS_COUNT][FLAME_SIZE];
uint32_t flame_gradient[FLAME_SIZE];

// --- Utility functions ---
uint8_t qadd8(const uint8_t i, const uint8_t j) {
    unsigned int t = i + j;
    if (t > 255) t = 255;
    return t;
}

uint8_t qsub8(const uint8_t i, const uint8_t j) {
    int t = i - j;
    if (t < 0) t = 0;
    return t;
}

uint8_t random8(const uint8_t min_val, const uint8_t max_val) {
    return min_val + random(max_val - min_val);
}

uint8_t random8() {
    return random(256);
}

void animateFire(Adafruit_NeoPixel &strip, uint8_t heat[FLAME_SIZE]) {
    for (uint8_t i = 0; i < FLAME_SIZE; i++) {
        heat[i] = qsub8(heat[i], random8(0, COOLING * 10 / FLAME_SIZE + 2));
    }

    for (uint8_t i = FLAME_SIZE - 1; i >= 2; i--) {
        heat[i] = (heat[i - 1] + heat[i - 2] + heat[i - 2]) / 3;
    }

    if (random8() < SPARKING) {
        const uint8_t y = random8(0, 3);
        heat[y] = qadd8(heat[y], random8(160, 255));
    }

    for (int j = 0; j < FLAME_SIZE; j++) {
        const uint32_t base_color = flame_gradient[j];
        const uint8_t pixel_brightness = heat[j] == 0 ? 0 : map(heat[j], 0, 255, MIN_FLAME_BRIGHTNESS, 255);

        strip.setPixelColor(
            strip.numPixels() - FLAME_SIZE + j,
            (base_color >> 16 & 0xFF) * pixel_brightness / 255,
            (base_color >> 8 & 0xFF) * pixel_brightness / 255,
            (base_color & 0xFF) * pixel_brightness / 255
        );
    }
}

// --- Lifecycle functions ---
void setup() {
    Serial.begin(115200);
    randomSeed(analogRead(0));

    for (uint8_t i = 0; i < FLAME_SIZE; i++) {
        flame_gradient[i] = BASE_COLOR;
    }

    for (uint8_t i = FLAME_SIZE - FLAME_TIP_SIZE; i < FLAME_SIZE; i++) {
        flame_gradient[i] = TIP_COLOR;
    }

    for (uint8_t i = 0; i < STRIPS_COUNT; i++) {
        left_strips[i].begin();
        left_strips[i].fill(BASE_COLOR);

        right_strips[i].begin();
        right_strips[i].fill(BASE_COLOR);
    }
}

void loop() {
    //MASTER_BRIGHTNESS = map(analogRead(A0), 0, 1023, 0, 255); //TODO later

    for (uint8_t i = 0; i < STRIPS_COUNT; i++) {
        left_strips[i].setBrightness(MASTER_BRIGHTNESS);

        right_strips[i].setBrightness(MASTER_BRIGHTNESS);
    }

    for (const uint8_t i: {1, 3, 5, 6}) {
        animateFire(left_strips[i], heat[i]);

        animateFire(right_strips[i], heat[i]);
    }

    for (uint8_t i = 0; i < STRIPS_COUNT; i++) {
        left_strips[i].show();

        right_strips[i].show();
    }

    delay(ANIMATION_DELAY);
}
