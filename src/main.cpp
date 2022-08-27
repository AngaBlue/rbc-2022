#include <Arduino.h>
#include <stdint.h>
#include "libs/tcs3200.h"
#include "util.h"

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
#define U_OUT 3

// Motor Control
#define LEFT_ENA A0
#define LEFT_IN1 A1
#define LEFT_IN2 A2

#define RIGHT_ENA A3
#define RIGHT_IN1 A4
#define RIGHT_IN2 A5

// Sensors
tcs3200 TCS_LEFT(S0, S1, S2, S3, C_OUT_LEFT);
tcs3200 TCS_RIGHT(S0, S1, S2, S3, C_OUT_RIGHT);

// Colour defines
int RGBColors[COLOR_COUNT][3] = {
    {6, 9, 10},   // Green
    {35, 23, 14}, // Yellow
    {25, 7, 9}, // Red
    {5, 4, 5},   // Black
    {52, 47, 60} // White
};

void move(Direction direction)
{
    Serial.println("Moving " + directionNameFromEnum(direction));

    switch (direction)
    {
    case LEFT:
        digitalWrite(LEFT_ENA, HIGH);
        digitalWrite(RIGHT_ENA, HIGH);
        digitalWrite(LEFT_IN1, LOW);
        digitalWrite(LEFT_IN2, HIGH);
        digitalWrite(RIGHT_IN1, HIGH);
        digitalWrite(RIGHT_IN2, LOW);
        break;
    case RIGHT:
        digitalWrite(LEFT_ENA, HIGH);
        digitalWrite(RIGHT_ENA, HIGH);
        digitalWrite(LEFT_IN1, HIGH);
        digitalWrite(LEFT_IN2, LOW);
        digitalWrite(RIGHT_IN1, LOW);
        digitalWrite(RIGHT_IN2, HIGH);
        break;
    case FORWARD:
        digitalWrite(LEFT_ENA, HIGH);
        digitalWrite(RIGHT_ENA, HIGH);
        digitalWrite(LEFT_IN1, HIGH);
        digitalWrite(LEFT_IN2, LOW);
        digitalWrite(RIGHT_IN1, HIGH);
        digitalWrite(RIGHT_IN2, LOW);
        break;
    case BACKWARD:
        digitalWrite(LEFT_ENA, HIGH);
        digitalWrite(RIGHT_ENA, HIGH);
        digitalWrite(LEFT_IN1, LOW);
        digitalWrite(LEFT_IN2, HIGH);
        digitalWrite(RIGHT_IN1, LOW);
        digitalWrite(RIGHT_IN2, HIGH);
        break;
    case STOP:
        digitalWrite(LEFT_ENA, LOW);
        digitalWrite(RIGHT_ENA, LOW);
        digitalWrite(LEFT_IN1, LOW);
        digitalWrite(LEFT_IN2, LOW);
        digitalWrite(RIGHT_IN1, LOW);
        digitalWrite(RIGHT_IN2, LOW);
        break;
    }
}

void setup()
{
    Serial.begin(9600);

    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    pinMode(A4, OUTPUT);
    pinMode(A5, OUTPUT);
}

void loop()
{
    // Read both color sensors
    Colour c_left = (Colour)TCS_LEFT.closestColorIndex(RGBColors, COLOR_COUNT);
    Colour c_right = (Colour)TCS_RIGHT.closestColorIndex(RGBColors, COLOR_COUNT);

    // Print the color values
    Serial.println("Left: " + colourNameFromEnum(c_left) + " Right: " + colourNameFromEnum(c_right));

    // Find direction
    Direction direction = FORWARD;
    if (c_left == BLACK && c_right == WHITE)
    {
        direction = LEFT;
    }
    else if (c_left == WHITE && c_right == BLACK)
    {
        direction = RIGHT;
    }
    else if (c_left == WHITE && c_right == WHITE)
    {
        direction = FORWARD;
    }

    // Move the robot
    move(direction);
}
