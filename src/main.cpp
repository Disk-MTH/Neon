#include <Adafruit_NeoPixel.h>

#define LED_TYPE (NEO_GRB + NEO_KHZ800)
#define BRIGHTNESS 100
#define STRIPS_PER_SIDE 7
#define STRIP_1 70
#define STRIP_2 62
#define STRIP_3 44
#define STRIP_4 64
#define STRIP_5 28
#define STRIP_6 52
#define STRIP_7 50


Adafruit_NeoPixel left_strips[STRIPS_PER_SIDE] = {
    Adafruit_NeoPixel(STRIP_1, 4, LED_TYPE),
    Adafruit_NeoPixel(STRIP_2, 13, LED_TYPE),
    Adafruit_NeoPixel(STRIP_3, 14, LED_TYPE),
    Adafruit_NeoPixel(STRIP_4, 16, LED_TYPE),
    Adafruit_NeoPixel(STRIP_5, 17, LED_TYPE),
    Adafruit_NeoPixel(STRIP_6, 18, LED_TYPE),
    Adafruit_NeoPixel(STRIP_7, 19, LED_TYPE)
};

Adafruit_NeoPixel right_strips[STRIPS_PER_SIDE] = {
    Adafruit_NeoPixel(STRIP_1, 21, LED_TYPE),
    Adafruit_NeoPixel(STRIP_2, 22, LED_TYPE),
    Adafruit_NeoPixel(STRIP_3, 23, LED_TYPE),
    Adafruit_NeoPixel(STRIP_4, 25, LED_TYPE),
    Adafruit_NeoPixel(STRIP_5, 26, LED_TYPE),
    Adafruit_NeoPixel(STRIP_6, 27, LED_TYPE),
    Adafruit_NeoPixel(STRIP_7, 32, LED_TYPE)
};

void setAllStripsColor(const uint32_t color) {
    for (uint8_t i = 0; i < STRIPS_PER_SIDE; i++) {
        left_strips[i].fill(color);
        left_strips[i].show();

        right_strips[i].fill(color);
        right_strips[i].show();
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("\n--- System Boot Log ---");

    for (uint8_t i = 0; i < STRIPS_PER_SIDE; i++) {
        left_strips[i].begin();
        left_strips[i].setBrightness(BRIGHTNESS);
        left_strips[i].show();

        right_strips[i].begin();
        right_strips[i].setBrightness(BRIGHTNESS);
        right_strips[i].show();
    }
    Serial.println("LEDs ready");

    Serial.println("Startup complete");
}

void loop() {
    static unsigned long lastChangeTime = 0;
    static int colorState = 0;

    if (millis() - lastChangeTime > 1000) {
        lastChangeTime = millis();
        switch (colorState) {
            case 0:
                Serial.println("Red");
                setAllStripsColor(Adafruit_NeoPixel::Color(255, 0, 0));
                break;
            case 1:
                Serial.println("Green");
                setAllStripsColor(Adafruit_NeoPixel::Color(0, 255, 0));
                break;
            case 2:
                Serial.println("Blue");
                setAllStripsColor(Adafruit_NeoPixel::Color(0, 0, 255));
                break;
            default:
                break;
        }
        colorState = (colorState + 1) % 3;
    }
}
