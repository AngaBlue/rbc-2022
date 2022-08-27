#include <Arduino.h>
#include <tcs3200.h>
#include <stdint.h>

#define COLOR_COUNT 5

// General Color Sensor pins
#define S0 11
#define S1 8
#define S2 7
#define S3 6

// Colour Sensor Output
#define C_OUT_0 13
#define C_OUT_1 12

// Ultrasonic Sensor Control
#define UT 2

// Ultrasonic Output
#define UOut 3

typedef struct Color {
    uint8_t R;
    uint8_t G;
    uint8_t B;
} Color;

// Sensors
tcs3200 TCS_0(S0, S1, S2, S3, C_OUT_0);
tcs3200 TCS_1(S0, S1, S2, S3, C_OUT_1);

Color C0, C1;

// Colour defines
int RGBColors[COLOR_COUNT][3] = {
    {25, 7, 9}, // Red
    {6, 9, 10}, // Green
    {5, 4, 5}, // Black
    {35, 23, 14}, // Yellow
    {52, 47, 60} // White
};

String ColorNames[COLOR_COUNT] = {
    "R", // Red
    "G", // Green
    "K", // Black
    "Y", // Yellow
    "W"  // White
};

void setup() {
    Serial.begin(9600);
}

void loop() {
    // Read both color sensors
    auto start = micros();
    C0.R = TCS_0.colorRead('r');
    C0.G = TCS_0.colorRead('g');
    C0.B = TCS_0.colorRead('b');
    C1.R = TCS_1.colorRead('r');
    C1.G = TCS_1.colorRead('g');
    C1.B = TCS_1.colorRead('b');
    Serial.println(micros() - start);

    // Print the color values
    Serial.println("C0: #" + String(C0.R)  + String(C0.G) + " " + String(C0.B));
    Serial.println(TCS_0.closestColor(RGBColors, ColorNames, COLOR_COUNT));

    delay(300);
}
