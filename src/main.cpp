#include <Arduino.h>
#include <tcs3200.h>

#define COLOR_COUNT 5

// General Color Sensor pins
#define S0 11
#define S1 8
#define S2 7
#define S3 6

// Colour Sensor Out
#define C_OUT_0 13
#define C_OUT_1 12

#define UOut 3

// Sensors
tcs3200 TCS_0(S0, S1, S2, S3, C_OUT_0);
tcs3200 TCS_1(S0, S1, S2, S3, C_OUT_1);

// Colour defines
int RGBColors[COLOR_COUNT][3] = {
    {255, 0, 0}, // Red
    {0, 255, 0}, // Green
    {0, 0, 0}, // Black
    {255, 255, 0}, // Yellow
    {255, 255, 255} // White
};

String ColorNames[COLOR_COUNT] = {
    "Red",
    "Green",
    "Black",
    "Yellow",
    "White"
};

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}
