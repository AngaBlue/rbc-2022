#include <Arduino.h>
#include "util.h"
#include "header.h"

String colourNameFromEnum(Colour input)
{
    switch (input)
    {
    case Colour::BLACK:
        return "BLACK";
    case Colour::WHITE:
        return "WHITE";
    default:
        return "UNKNOWN";
    }
}

String directionNameFromEnum(Direction direction)
{
    switch (direction)
    {
    case Direction::LEFT:
        return "LEFT";
    case Direction::RIGHT:
        return "RIGHT";
    case Direction::FORWARD:
        return "FORWARD";
    case Direction::BACKWARD:
        return "BACKWARD";
    case Direction::STOP:
        return "STOP";
    default:
        return "UNKNOWN";
    }
}

void move(Direction direction, float left_multiplier, float right_multiplier)
{
#ifdef LOGGING
    Serial.println("Moving " + directionNameFromEnum(direction));
#endif

    if (direction != Direction::BACKWARD)
    {
        // Default drive forward.
        digitalWrite(LEFT_IN1, HIGH);
        digitalWrite(LEFT_IN2, LOW);
        digitalWrite(RIGHT_IN1, HIGH);
        digitalWrite(RIGHT_IN2, LOW);
    }

    switch (direction)
    {
    // No multiplier for the left/right turning radius as if it's the
    // "closest" colour then we should probably drive it full-speed.
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
