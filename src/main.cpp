#include <Arduino.h>
#include <TCS3200.h>
#include <stdint.h>
#include "util.h"
#include "header.h"

// Define RGB values for colours, these must match the same order as the enum.
static uint8_t RGBColoursLeft[COLOUR_COUNT][3] = {
    {47, 41, 55},   // Black
    {166, 166, 200} // White
};

static uint8_t RGBColoursRight[COLOUR_COUNT][3] = {
    {62, 55, 66},   // Black
    {125, 125, 142} // White
};

// Sensors
TCS3200 TCS_LEFT(S0, S1, S2, S3, C_OUT_LEFT);
TCS3200 TCS_RIGHT(S0, S1, S2, S3, C_OUT_RIGHT);

void setup()
{
#ifdef LOGGING
    // Begin serial communication for logging.
    Serial.begin(9600);
#endif

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
    // Read both colour sensors.
    Colour c_left = (Colour)TCS_LEFT.closestColour(RGBColoursLeft, COLOUR_COUNT, SENSOR_READOUT_SCALING);
    Colour c_right = (Colour)TCS_RIGHT.closestColour(RGBColoursRight, COLOUR_COUNT, SENSOR_READOUT_SCALING);

#ifdef LOGGING
    // Print the colour values.
    Serial.println("Left: " + colourNameFromEnum(c_left) + " Right: " + colourNameFromEnum(c_right));

    auto rgb_left = TCS_LEFT.colourReadRGB(SENSOR_READOUT_SCALING);
    auto rgb_right = TCS_RIGHT.colourReadRGB(SENSOR_READOUT_SCALING);

    Serial.println("Left: " + String(rgb_left.r) + " " + String(rgb_left.g) + " " + String(rgb_left.b));
    Serial.println("Right: " + String(rgb_right.r) + " " + String(rgb_right.g) + " " + String(rgb_right.b));
#endif

    // Find direction
    Direction direction = Direction::FORWARD;
    if (c_left == Colour::BLACK)
    {
        direction = Direction::LEFT;
    }
    else if (c_right == Colour::BLACK)
    {
        direction = Direction::RIGHT;
    }

    // Move the robot.
    move(direction);

    if (direction == Direction::FORWARD)
    {
        // Poor man's PWM to slow the robot's speed.
        delay(40);
        move(Direction::FORWARD, 0.2, 0.2);
        delay(40);
    }
    else
    {
        // If we're turning, we need to wait for the robot to turn before we can read the colour again.
        delay(20);
    }
}
