#pragma once
#include <Arduino.h>

/**
 * @brief A class to represent an 8-bit RGB colour.
 */
typedef struct RGB {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} RGB;

/**
 * @brief A class to represent a TCS3200 colour sensor.
 */
class TCS3200
{
public:
	TCS3200(uint8_t S0, uint8_t S1, uint8_t S2, uint8_t S3, uint8_t output);

	/**
	 * @brief Reads the colour from the sensor.
	 * 
	 * @param colour The colour to read.
	 * @param scaling The scaling to use.
	 * @return The colour value.
	 */
	int colourRead(char colour = 'c', int scaling = 20);

	/**
	 * @brief Reads the colour from the sensor and returns the closest colour.
	 *
	 * @param distinctRGB The array of RGB values to compare against.
	 * @param num_of_colours The number of colours to detect.
	 * @param scaling The scaling to use for the sensor.
	 * @return RGB The RGB colour read from the sensor.
	 */
	int closestColour(uint8_t distinctRGB[][3], int num_of_colours, int scaling);

	/**
	 * @brief Reads the RGB values from the sensor.
	 *
	 * @param scaling The scaling to use for the sensor.
	 * @return The RGB values read from the sensor.
	 */
	RGB colourReadRGB(int scaling);

private:
	/**
	 * @brief Sensor pins.
	 */
	uint8_t _S0;
	uint8_t _S1;
	uint8_t _S2;
	uint8_t _S3;
	uint8_t _output;
};
