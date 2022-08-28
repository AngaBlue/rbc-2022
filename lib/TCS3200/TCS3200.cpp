#include <Arduino.h>
#include "TCS3200.h"

TCS3200::TCS3200(uint8_t S0, uint8_t S1, uint8_t S2, uint8_t S3, uint8_t output)
{
	pinMode(S0, OUTPUT);
	pinMode(S1, OUTPUT);
	pinMode(S2, OUTPUT);
	pinMode(S3, OUTPUT);
	pinMode(output, INPUT);

	_S0 = S0;
	_S1 = S1;
	_S2 = S2;
	_S3 = S3;
	_output = output;
}

int TCS3200::colourRead(char colour, int scaling)
{

	switch (scaling)
	{
	case 0: // Set scaling to 0% (scaling is turned OFF).
		digitalWrite(_S0, LOW);
		digitalWrite(_S1, LOW);
		break;

	case 2: // Set scaling to 2%.
		digitalWrite(_S0, LOW); 
		digitalWrite(_S1, HIGH);
		break;

	case 20: // Set scaling to 20%.
		digitalWrite(_S0, HIGH);
		digitalWrite(_S1, LOW);
		break;

	case 100: // Set scaling to 100%.
		digitalWrite(_S0, HIGH);
		digitalWrite(_S1, HIGH);
		break;

	default: // Set default scaling (default scaling is 20%).
		digitalWrite(_S0, HIGH);
		digitalWrite(_S1, LOW);
		break;
	}

	switch (colour)
	{
	case 'r': // Setting red filtered photodiodes to be read.
		digitalWrite(_S2, LOW);
		digitalWrite(_S3, LOW);
		break;

	case 'b': // Setting blue filtered photodiodes to be read.
		digitalWrite(_S2, LOW);
		digitalWrite(_S3, HIGH);
		break;

	case 'c': // Setting clear photodiodes (no filters on diodes) to be read.
		digitalWrite(_S2, HIGH);
		digitalWrite(_S3, LOW);
		break;

	case 'g': // Setting green filtered photodiodes to be read.
		digitalWrite(_S2, HIGH);
		digitalWrite(_S3, HIGH);
		break;

	default:
		digitalWrite(_S2, HIGH);
		digitalWrite(_S3, LOW);
		break;
	}

	unsigned long duration;

	duration = pulseIn(_output, LOW);
	
	if (duration != 0)
	{
		return 1000 / duration; // Reads and returns the frequency of selected colour.
	}
	else
	{
		return 0;
	}
}

int TCS3200::closestColour(uint8_t distinctRGB[][3], int num_of_colours, int scaling)
{
	int index = -1; // return -1 if no declared colour matches colour sensor is reading.
	int smallestDistance = 765;
	int r, g, b;

	r = colourRead('r', scaling);
	g = colourRead('g', scaling);
	b = colourRead('b', scaling);

	for (int i = 0; i < num_of_colours; i++)
	{
		int distance = sqrt(pow(r - distinctRGB[i][0], 2) + pow(g - distinctRGB[i][1], 2) + pow(b - distinctRGB[i][2], 2));
		if (distance < smallestDistance)
		{
			index = i;
			smallestDistance = distance;
		}
	}

	return index;
}

RGB TCS3200::colourReadRGB(int scaling) 
{
	RGB rgb;

	rgb.r = colourRead('r', scaling);
	rgb.g = colourRead('g', scaling);
	rgb.b = colourRead('b', scaling);
	
	return rgb;
}
