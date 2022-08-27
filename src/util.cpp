#include <Arduino.h>
#include "util.h"

String colourNameFromEnum(Colour input)
{
    switch (input)
    {
    case RED:
        return "RED";
    case GREEN:
        return "GREEN";
    case BLACK:
        return "BLACK";
    case YELLOW:
        return "YELLOW";
    case WHITE:
        return "WHITE";
    default:
        return "UNKNOWN";
    }
}

String directionNameFromEnum(Direction direction)
{
    switch (direction)
    {
    case LEFT:
        return "LEFT";
    case RIGHT:
        return "RIGHT";
    case FORWARD:
        return "FORWARD";
    case BACKWARD:
        return "BACKWARD";
    case STOP:
        return "STOP";
    default:
        return "UNKNOWN";
    }
}
