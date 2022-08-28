#include <Arduino.h>
#include <stdint.h>

#include "libs/tcs3200.h"
#include "util.h"

// Number of colours to detect
#define COLOUR_COUNT 5
#define SPEED 160
#define SENSOR_READOUT_SCALING 100
#define RIGHT_MOTOR_OFFSET * 0.9
#define TURN_OFFSET * 0.75
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
uint8_t RGBColoursLeft[COLOUR_COUNT][3] = {
    {47, 41, 55},   // Black
    {166, 166, 200} // White
};

uint8_t RGBColoursRight[COLOUR_COUNT][3] = {
    {62, 55, 66},   // Black
    {125, 125, 142} // White
};

enum class Side
{
    LEFT,
    RIGHT,
    UNBIASED
};

// Sensors
tcs3200 TCS_LEFT(S0, S1, S2, S3, C_OUT_LEFT);
tcs3200 TCS_RIGHT(S0, S1, S2, S3, C_OUT_RIGHT);

/**
 * @brief Moves the robot in the direction specified.
 *
 * @param direction The direction to move the robot in.
 * @param left_multiplier The speed multiplier to use for the left motor.
 * @param right_multiplier The speed multiplier to use for the right motor.
 */
void move(Direction direction, float left_multiplier = 1, float right_multiplier = 1)
{
    Serial.println("Moving " + directionNameFromEnum(direction));

    if (direction != Direction::BACKWARD)
    {
        digitalWrite(LEFT_IN1, HIGH);
        digitalWrite(LEFT_IN2, LOW);
        digitalWrite(RIGHT_IN1, HIGH);
        digitalWrite(RIGHT_IN2, LOW);
    }

    switch (direction)
    {
        // No multiplier for the left/right turning radius as if it's the 
        // "closest" colour then we should probably drive it full-speed
        case Direction::LEFT:
            analogWrite(LEFT_ENA, LOW);
            analogWrite(RIGHT_ENA, SPEED RIGHT_MOTOR_OFFSET TURN_OFFSET);
            break;
        case Direction::RIGHT:
            analogWrite(LEFT_ENA, SPEED TURN_OFFSET);
            analogWrite(RIGHT_ENA, LOW);
            break;
        case Direction::FORWARD:
            analogWrite(LEFT_ENA, SPEED * left_multiplier);
            analogWrite(RIGHT_ENA, SPEED RIGHT_MOTOR_OFFSET * right_multiplier);
            break;
        case Direction::BACKWARD:
            analogWrite(LEFT_ENA, SPEED * left_multiplier);
            analogWrite(RIGHT_ENA, SPEED RIGHT_MOTOR_OFFSET * right_multiplier);
            digitalWrite(LEFT_IN1, LOW);
            digitalWrite(LEFT_IN2, HIGH);
            digitalWrite(RIGHT_IN1, LOW);
            digitalWrite(RIGHT_IN2, HIGH);
            break;
        case Direction::STOP:
            analogWrite(LEFT_ENA, LOW);
            analogWrite(RIGHT_ENA, LOW);
            break;
    }
}

float getSpeedFactorFromDistance(Colour newColour, int distance, Side sensorLocation)
{
    return 1;
    /*
     * General idea is that we can figure out how much of the line the sensor
     * is reading based on the colour's "distance" from the full-patch readout
     * (see RGBColoursLeft, RGBColoursRight). Using this distance, we can scale
     * the motor's power output to turn a more appropriate amount instead of
     * always going to 11.
     *
     * Maximum distance between white and an arbitrary colour is 53.2 (W->Y,
     * left sensor) or 81.8 (W-Y, right sensor). I'm using 65 as a fallback
     * value in case something in the code is broken.
     *
     * If the value is larger than that... the good way would be to calculate
     * maximums for all possible colour transitions. I'll do this each call
     * instead of as a LUT because I'm lazy - could definitely use a macro or
     * something to do it at compile-time but this is hopefully fast enough.
     *
     * Does the Arduino have accelerated division..?
     */

    const int colourIndex = (int)newColour;
    Serial.println("Colour index: " + String(colourIndex));

    // Max speed for the motors to turn
    const float MAX_SPEED_SCALE = 1;
    // Set min speed to half - will have to tweak later
    const float MIN_SPEED_SCALE = 0.75;

    // Assume always transitioning from WHITE
    // const int WHITE_INDEX = (int)Colour::WHITE;

    // // Find the maximum value differences for each colour (assumed white start)
    // int diffR, diffG, diffB;
    // switch (sensorLocation)
    // {
    // case Side::LEFT:
    //     diffR = RGBColoursLeft[colourIndex][0] - RGBColoursLeft[WHITE_INDEX][0];
    //     diffG = RGBColoursLeft[colourIndex][1] - RGBColoursLeft[WHITE_INDEX][1];
    //     diffB = RGBColoursLeft[colourIndex][2] - RGBColoursLeft[WHITE_INDEX][2];
    //     break;

    // case Side::RIGHT:
    //     diffR = RGBColoursRight[colourIndex][0] - RGBColoursRight[WHITE_INDEX][0];
    //     diffG = RGBColoursRight[colourIndex][1] - RGBColoursRight[WHITE_INDEX][1];
    //     diffB = RGBColoursRight[colourIndex][2] - RGBColoursRight[WHITE_INDEX][2];
    //     break;

    // case Side::UNBIASED:
    //     // This shouldn't happen.
    //     diffR = diffG = diffB = 65;
    //     Serial.println("Invalid side input to turn power calculator.");
    //     break;
    // }

    // Same algorithm used in the library
    // float maxDistance = sqrt(pow(diffR, 2) + pow(diffG, 2) + pow(diffB, 2));
    // The closer the distance is to 0, the less we want to turn

    const int maxDistance = 65;

    float turn_power_reduction;
    if (maxDistance != 0) {
        // Scale the turn power to the max/min speed scale
        turn_power_reduction = (float)distance / maxDistance;
    } else {
        // If the max distance is 0, we're probably on the line
        turn_power_reduction = MAX_SPEED_SCALE;
    }

    Serial.println("Max distance: " + String(maxDistance) + " Distance: " + String(distance));

    // Scale the speed based on the turn power
    float multiplier = MAX_SPEED_SCALE - (MAX_SPEED_SCALE - MIN_SPEED_SCALE) * turn_power_reduction;
    Serial.println("Turn power: " + String(turn_power_reduction) + " Multiplier: " + String(multiplier));
    return multiplier;
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
    int dist_left, dist_right;

    // Read both colour sensors
    Colour c_left = (Colour)TCS_LEFT.closestColour(RGBColoursLeft, COLOUR_COUNT, SENSOR_READOUT_SCALING, &dist_left);
    Colour c_right = (Colour)TCS_RIGHT.closestColour(RGBColoursRight, COLOUR_COUNT, SENSOR_READOUT_SCALING, &dist_right);

    // Print the colour values
    Serial.println("Left: " + colourNameFromEnum(c_left) + " Right: " + colourNameFromEnum(c_right));

    auto rgb_left = TCS_LEFT.colourReadRGB(SENSOR_READOUT_SCALING);
    auto rgb_right = TCS_RIGHT.colourReadRGB(SENSOR_READOUT_SCALING);

    Serial.println("Left: " + String(rgb_left.r) + " " + String(rgb_left.g) + " " + String(rgb_left.b));
    Serial.println("Right: " + String(rgb_right.r) + " " + String(rgb_right.g) + " " + String(rgb_right.b));

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

    // Find speed multipliers
    // If the LEFT sensor is detecting a colour, drive the RIGHT one faster (likewise for flip)
    // At the moment, the sensor on WHITE will probably return a low multiplier (can fix but cbf right now)
    float left_motor_multiplier = getSpeedFactorFromDistance(c_left, dist_left, Side::LEFT);
    float right_motor_multiplier = getSpeedFactorFromDistance(c_right, dist_right, Side::RIGHT);

    // Move the robot
    move(direction, left_motor_multiplier, right_motor_multiplier);

    if (direction != Direction::FORWARD)
    {
        // If we're turning, we need to wait for the robot to turn before we can read the colour again
        delay(80);
    } else {
        delay(50);
        move(Direction::FORWARD, 0.2, 0.2);
        delay(50);
    }
}
