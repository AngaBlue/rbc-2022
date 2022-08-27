#include <Arduino.h>
#include <tcs3200.h>

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

char ColorNames[COLOR_COUNT] = {
    'R', // Red
    'G', // Green
    'K', // Black
    'Y', // Yellow
    'W'  // White
};

void setup() {
  // Colour Sensor Control
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Colour Sensor Readout
  pinMode(COut1, INPUT);
  pinMode(COut2, INPUT);

  // Ultrasonic Sensor Control
  pinMode(UT, OUTPUT);

  // Ultrasonic Sensor Readout
  pinMode(UOut, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
}
