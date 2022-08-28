#ifndef TCS3200_h
#define TCS3200_h

#include <Arduino.h>

typedef struct RGB {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} RGB;

class TCS3200
{
public:
	TCS3200(uint8_t S0, uint8_t S1, uint8_t S2, uint8_t S3, uint8_t output);
	int colourRead(char colour = 'c', int scaling = 20);
	int closestColour(uint8_t distinctRGB[][3], int num_of_colours, int scaling);
	RGB colourReadRGB(int scaling);

private:
	uint8_t _S0;
	uint8_t _S1;
	uint8_t _S2;
	uint8_t _S3;
	uint8_t _output;
};

#endif
