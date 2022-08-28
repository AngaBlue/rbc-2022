#ifndef util_h
#define util_h

#include <Arduino.h>

/**
 * @brief The direction in which the Robot can move.
 */
enum class Direction
{
    LEFT,
    RIGHT,
    FORWARD,
    BACKWARD,
    STOP
};

/**
 * @brief The colour of the track.
 */
enum class Colour
{
    BLACK,
    WHITE
};

/**
 * @brief Converts a direction enum value to a string.
 * 
 * @param direction The direction value.
 * @return The direction as a string.
 */
String directionNameFromEnum(Direction direction);

/**
 * @brief Converts a colour enum value to a string.
 * 
 * @param colour The colour value.
 * @return The colour as a string.
 */
String colourNameFromEnum(Colour colour);

#endif
