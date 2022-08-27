#include <Arduino.h>
#include <stdint.h>

#include "libs/tcs3200.h"
#include "util.h"

// Number of colours to detect
#define COLOUR_COUNT 5
#define SPEED 120
#define SCALING 100
#define RIGHT_MOTOR_OFFSET * 0.85
#define MOVEMENT_CHECK_DELAY 0

// Shared colour sensor pins
#define S0 11
#define S1 8
#define S2 7
#define S3 6

// Colour sensor output pins
#define C_OUT_LEFT 13
#define C_OUT_RIGHT 12

// Ultrasonic Sensor Control
#define UT 2

// Ultrasonic Output
#define U_OUT 3

// Motor Control
#define LEFT_ENA 5
#define LEFT_IN1 A4
#define LEFT_IN2 A5

#define RIGHT_ENA 3
#define RIGHT_IN1 A1
#define RIGHT_IN2 A2

// Define RGB values for colours, these must match the same order as the enum
int RGBColorsLeft[COLOUR_COUNT][3] = {
    {76, 76, 100},  // Green
    {125, 100, 90}, // Yellow
    {111, 58, 76},  // Red
    {47, 41, 55},   // Black
    {125, 111, 142} // White
};

int RGBColorsRight[COLOUR_COUNT][3] = {
    {55, 62, 71},    // Green
    {125, 100, 90},  // Yellow
    {111, 58, 76},   // Red
    {62, 55, 66},    // Black
    {142, 125, 166}  // White
};

// Sensors
tcs3200 TCS_LEFT(S0, S1, S2, S3, C_OUT_LEFT);
tcs3200 TCS_RIGHT(S0, S1, S2, S3, C_OUT_RIGHT);

/**
 * @brief Moves the robot in the direction specified.
 *
 * @param direction The direction to move the robot in.
 */
void move(Direction direction)
{
    Serial.println("Moving " + directionNameFromEnum(direction));

    switch (direction)
    {
    case LEFT:
        analogWrite(LEFT_ENA, 255);
        analogWrite(RIGHT_ENA, 255 RIGHT_MOTOR_OFFSET);
        digitalWrite(LEFT_IN1, LOW);
        digitalWrite(LEFT_IN2, HIGH);
        digitalWrite(RIGHT_IN1, HIGH);
        digitalWrite(RIGHT_IN2, LOW);
        break;
    case RIGHT:
        analogWrite(LEFT_ENA, 255);
        analogWrite(RIGHT_ENA, 255 RIGHT_MOTOR_OFFSET);
        digitalWrite(LEFT_IN1, HIGH);
        digitalWrite(LEFT_IN2, LOW);
        digitalWrite(RIGHT_IN1, LOW);
        digitalWrite(RIGHT_IN2, HIGH);
        break;
    case FORWARD:
        analogWrite(LEFT_ENA, SPEED);
        analogWrite(RIGHT_ENA, SPEED RIGHT_MOTOR_OFFSET);
        digitalWrite(LEFT_IN1, HIGH);
        digitalWrite(LEFT_IN2, LOW);
        digitalWrite(RIGHT_IN1, HIGH);
        digitalWrite(RIGHT_IN2, LOW);
        break;
    case BACKWARD:
        analogWrite(LEFT_ENA, SPEED);
        analogWrite(RIGHT_ENA, SPEED RIGHT_MOTOR_OFFSET);
        digitalWrite(LEFT_IN1, LOW);
        digitalWrite(LEFT_IN2, HIGH);
        digitalWrite(RIGHT_IN1, LOW);
        digitalWrite(RIGHT_IN2, HIGH);
        break;
    case STOP:
        analogWrite(LEFT_ENA, SPEED);
        analogWrite(RIGHT_ENA, SPEED RIGHT_MOTOR_OFFSET);
        digitalWrite(LEFT_IN1, LOW);
        digitalWrite(LEFT_IN2, LOW);
        digitalWrite(RIGHT_IN1, LOW);
        digitalWrite(RIGHT_IN2, LOW);
        break;
    }
}

void setup()
{
    // Begin serial communication for logging
    Serial.begin(9600);

    // Initialise all motor pins.  The colour sensors are initialised in the sensor constructor.
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
    Colour c_left = (Colour)TCS_LEFT.closestColorIndex(RGBColorsLeft, COLOUR_COUNT, SCALING);
    Colour c_right = (Colour)TCS_RIGHT.closestColorIndex(RGBColorsRight, COLOUR_COUNT, SCALING);

    // Print the color values
    Serial.println("Left: " + colourNameFromEnum(c_left) + " Right: " + colourNameFromEnum(c_right));

    auto rgb_left = TCS_LEFT.colorReadRGB(SCALING);
    auto rgb_right = TCS_RIGHT.colorReadRGB(SCALING);

    Serial.println("Left: " + String(rgb_left.r) + " " + String(rgb_left.g) + " " + String(rgb_left.b));
    Serial.println("Right: " + String(rgb_right.r) + " " + String(rgb_right.g) + " " + String(rgb_right.b));

    // Find direction
    Direction direction = FORWARD;
    if (c_left == BLACK)
    {
        direction = LEFT;
    }
    else if (c_right == BLACK)
    {
        direction = RIGHT;
    }

    // Move the robot
    move(direction);
    if (direction != FORWARD) delay(MOVEMENT_CHECK_DELAY);
}
