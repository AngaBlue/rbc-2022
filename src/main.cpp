#include <Arduino.h>
#include "includes/tcs3200.h"
#include <stdint.h>

#define COLOR_COUNT 5

// General Color Sensor pins
#define S0 11
#define S1 8
#define S2 7
#define S3 6

// Colour Sensor Output
#define C_OUT_LEFT 13
#define C_OUT_RIGHT 12

// Ultrasonic Sensor Control
#define UT 2

// Ultrasonic Output
#define UOut 3

// Sensors
tcs3200 TCS_LEFT(S0, S1, S2, S3, C_OUT_LEFT);
tcs3200 TCS_RIGHT(S0, S1, S2, S3, C_OUT_RIGHT);

// Define the colours using an enum
enum Color {
    RED,
    GREEN,
    BLACK,
    YELLOW,
    WHITE
};

String colourNameFromEnum(Color input) {
    switch (input) {
        case RED:
            return "RED";
        case GREEN:
            return "GREEN";
        case BLACK:
            return "BLACK";
        case YELLOW:
            return "YELLOW";
        case WHITE:
            return "WHITE";
        default:
            return "UNKNOWN";
    }
}

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
    auto c_left = TCS_LEFT.closestColorIndex(RGBColors, COLOR_COUNT);
    auto c_right = TCS_RIGHT.closestColorIndex(RGBColors, COLOR_COUNT);

    // Print the color values
    Serial.println("Left: " + colourNameFromEnum((Color)c_left) + " Right: " + colourNameFromEnum((Color)c_right));
}
