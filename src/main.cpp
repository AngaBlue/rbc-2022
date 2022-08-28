#include <Arduino.h>
#include <stdint.h>

#include "libs/tcs3200.h"
#include "util.h"

// #define LOGGING LOGGING

// Number of colours to detect
#define COLOUR_COUNT 5
#define SPEED 138
#define SENSOR_READOUT_SCALING 100
#define RIGHT_MOTOR_OFFSET * 0.9
#define TURN_OFFSET * 0.48
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
    {66, 76, 90},  // Green
    {142, 111, 100},// Yellow
    {125, 62, 83},  // Red
    {20, 21, 35},   // Black
    {166, 166, 200} // White
};

uint8_t RGBColoursRight[COLOUR_COUNT][3] = {
    {52, 58, 66},    // Green
    {111, 83, 76},   // Yellow
    {100, 47, 62},   // Red
    {42, 35, 46},    // Black
    {125, 125, 142}  // White
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
    #ifdef LOGGING
    Serial.println("Moving " + directionNameFromEnum(direction));
    #endif

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
            analogWrite(LEFT_ENA, SPEED TURN_OFFSET * 0.68);
            digitalWrite(LEFT_IN1, LOW);
            digitalWrite(LEFT_IN2, HIGH);
            analogWrite(RIGHT_ENA, SPEED RIGHT_MOTOR_OFFSET TURN_OFFSET);
            break;
        case Direction::RIGHT:
            analogWrite(LEFT_ENA, SPEED TURN_OFFSET);
            analogWrite(RIGHT_ENA, SPEED TURN_OFFSET * 0.68);
            digitalWrite(RIGHT_IN1, LOW);
            digitalWrite(RIGHT_IN2, HIGH);
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
}

void setup()
{
    // Begin serial communication for logging
    #ifdef LOGGING
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
    int dist_left, dist_right;

    // Read both colour sensors
    Colour c_left = (Colour)TCS_LEFT.closestColour(RGBColoursLeft, COLOUR_COUNT, SENSOR_READOUT_SCALING, &dist_left);
    Colour c_right = (Colour)TCS_RIGHT.closestColour(RGBColoursRight, COLOUR_COUNT, SENSOR_READOUT_SCALING, &dist_right);

    #ifdef LOGGING
    // Print the colour values
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
        delay(40);
    } else {
        delay(50);
        move(Direction::FORWARD, 0.2, 0.2);
        delay(50);
    }
}
