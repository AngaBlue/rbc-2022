#include <Arduino.h>
#include <tcs3200.h>

#define COLOR_COUNT 5

// General Color Sensor pins
#define S0 11
#define S1 8
#define S2 7
#define S3 6

// Colour Sensor Out
#define COut1 13
#define COut2 12

#define UOut 3

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
