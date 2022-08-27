#include <Arduino.h>
#include <stdint.h>

#include "libs/tcs3200.h"
#include "util.h"

// Number of colours to detect
#define COLOUR_COUNT 5
#define SPEED 100
#define SCALING 100
#define RIGHT_MOTOR_OFFSET *0.9
#define TURN_OFFSET *1
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
    {55, 62, 71},   // Green
    {125, 100, 90}, // Yellow
    {111, 58, 76},  // Red
    {62, 55, 66},   // Black
    {142, 125, 166} // White
};

enum class Side
{
    LEFT,
    RIGHT,
    UNBIASED
};

typedef struct State
{
    Side side;
    Colour colour;
} State;

// Sensors
tcs3200 TCS_LEFT(S0, S1, S2, S3, C_OUT_LEFT);
tcs3200 TCS_RIGHT(S0, S1, S2, S3, C_OUT_RIGHT);

State state{.side = Side::UNBIASED, .colour = Colour::BLACK};

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
    Serial.println("Path: " + colourNameFromEnum(state.colour) + " Bias: " + (state.side == Side::LEFT ? "Left" : "Right"));

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
    /*
     * General idea is that we can figure out how much of the line the sensor
     * is reading based on the colour's "distance" from the full-patch readout
     * (see RGBColorsLeft, RGBColorsRight). Using this distance, we can scale
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

    // Max speed for the motors to turn
    const int MAX_SPEED_SCALE = 1;
    // Set min speed to half - will have to tweak later
    const int MIN_SPEED_SCALE = 0.5;

    // Assume always transitioning from WHITE
    const int WHITE_INDEX = (int)Colour::WHITE;

    // Find the maximum value differences for each colour (assumed white start)
    int diffR, diffG, diffB;
    switch (sensorLocation)
    {
    case Side::LEFT:
        diffR = RGBColorsLeft[colourIndex][0] - RGBColorsLeft[WHITE_INDEX][0];
        diffG = RGBColorsLeft[colourIndex][1] - RGBColorsLeft[WHITE_INDEX][1];
        diffB = RGBColorsLeft[colourIndex][2] - RGBColorsLeft[WHITE_INDEX][2];
        break;

    case Side::RIGHT:
        diffR = RGBColorsRight[colourIndex][0] - RGBColorsRight[WHITE_INDEX][0];
        diffG = RGBColorsRight[colourIndex][1] - RGBColorsRight[WHITE_INDEX][1];
        diffB = RGBColorsRight[colourIndex][2] - RGBColorsRight[WHITE_INDEX][2];
        break;

    case Side::UNBIASED:
        // This shouldn't happen.
        diffR = diffG = diffB = 65;
        Serial.println("Invalid side input to turn power calculator.");
        break;
    }

    // Same algorithm used in the library
    float maxDistance = sqrt(pow(diffR, 2) + pow(diffG, 2) + pow(diffB, 2));
    // The closer the distance is to 0, the less we want to turn
    float turn_power = (float)distance / maxDistance;

    // Scale the speed based on the turn power
    float multiplier = MIN_SPEED_SCALE + ((MAX_SPEED_SCALE - MIN_SPEED_SCALE) * turn_power);
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

    // Read both color sensors
    Colour c_left = (Colour)TCS_LEFT.closestColorIndex(RGBColorsLeft, COLOUR_COUNT, SENSOR_READOUT_SCALING, &dist_left);
    Colour c_right = (Colour)TCS_RIGHT.closestColorIndex(RGBColorsRight, COLOUR_COUNT, SENSOR_READOUT_SCALING, &dist_right);

    // Print the color values
    Serial.println("Left: " + colourNameFromEnum(c_left) + " Right: " + colourNameFromEnum(c_right));

    auto rgb_left = TCS_LEFT.colorReadRGB(SENSOR_READOUT_SCALING);
    auto rgb_right = TCS_RIGHT.colorReadRGB(SENSOR_READOUT_SCALING);

    Serial.println("Left: " + String(rgb_left.r) + " " + String(rgb_left.g) + " " + String(rgb_left.b));
    Serial.println("Right: " + String(rgb_right.r) + " " + String(rgb_right.g) + " " + String(rgb_right.b));

    // If unbiased or left side, and non-white colour, track that path
    if (c_left != Colour::WHITE && state.side != Side::RIGHT)
    {
        state.side = c_right == Colour::BLACK ? Side::LEFT : Side::UNBIASED;
        state.colour = c_left;
    }

    // If unbiased or right side, and non-white colour, track that path
    if (c_right != Colour::WHITE && state.side != Side::LEFT)
    {
        state.side = c_right == Colour::BLACK ? Side::RIGHT : Side::UNBIASED;
        state.colour = c_right;
    }

    // Find direction
    Direction direction = Direction::FORWARD;
    if (c_left == state.colour)
    {
        direction = Direction::LEFT;
    }
    else if (c_right == state.colour)
    {
        direction = Direction::RIGHT;
    }

    // Find speed multipliers
    // If the LEFT sensor is detecting a colour, drive the RIGHT one faster (likewise for flip)
    // At the moment, the sensor on WHITE will probably return a low multiplier (can fix but cbf right now)
    float right_motor_multiplier = getSpeedFactorFromDistance(c_left, dist_left, Side::LEFT);
    float left_motor_multiplier = getSpeedFactorFromDistance(c_right, dist_right, Side::RIGHT);

    // Move the robot
    move(direction, left_motor_multiplier, right_motor_multiplier);
}
