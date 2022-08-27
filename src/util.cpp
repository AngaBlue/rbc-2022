#include <Arduino.h>
#include "util.h"

String colourNameFromEnum(Colour input)
{
    switch (input)
    {
    case Colour::RED:
        return "RED";
    case Colour::GREEN:
        return "GREEN";
    case Colour::BLACK:
        return "BLACK";
    case Colour::YELLOW:
        return "YELLOW";
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
