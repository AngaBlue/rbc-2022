#include <Arduino.h>
#include <tcs3200.h> 

// General Color Sensor pins
#define S0 11
#define S1 8
#define S2 7
#define S3 6

// Colour Sensor Out
#define COut1 13
#define COut2 12

// Ultrasonic
#define UT 2
#define UOut 3


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
