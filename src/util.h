#include <Arduino.h>

#ifndef UTIL_H
#define UTIL_H

enum Direction
{
    LEFT,
    RIGHT,
    FORWARD,
    BACKWARD,
    STOP
};

enum Colour
{
    GREEN,
    YELLOW,
    RED,
    BLACK,
    WHITE
};

String directionNameFromEnum(Direction direction);
String colourNameFromEnum(Colour colour);

#endif
