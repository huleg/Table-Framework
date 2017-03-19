#include "Draw.h"
Pixel Pixels[WIDTH * HEIGHT];
// Pixels are stored to be optimal for sending to ws2812b leds from the microprocess

// Retrieve pixel, and return first pixel for invalid inputs
Pixel getPixel(int x, int y) {
	if (x > WIDTH || y > HEIGHT || x < 0 || y < 0) {
		return Pixels[0];
	}
	if(y % 2 == 0)
	{
	    return Pixels[y * WIDTH + WIDTH - x - 1];
	}
	return Pixels[y * WIDTH + x];
}

// Only update pixel if a valid position is given
void setPixel(int x, int y, Pixel pixel) {
	if (x > WIDTH) {
		return;
	}
	if (y > HEIGHT) {
		return;
	}
	if (x < 0) {
		return;
	}
	if (y < 0) {
		return;
	}
    if(y % 2 == 0)
    {
        Pixels[y * WIDTH + WIDTH - x - 1] = pixel;
    }
    else
    {
        Pixels[y * WIDTH + x] = pixel;
    }
}

void drawRect(int x, int y, int x2, int y2, Pixel pixel) {
	int y1;
	if (x2 > WIDTH) {
		x2 = WIDTH;
	}
	if (y2 > HEIGHT) {
		y2 = HEIGHT;
	}
	if (x < 0) {
		x = 0;
	}
	if (y < 0) {
		y = 0;
	}
	for (; x < x2; x++) {
		for (y1 = y; y1 < y2; y1++) {
			setPixel(x, y1, pixel);
		}
	}
}

void clearDisplay(void) {
	Pixel blank = { 0,0,0 };
	drawRect(0, 0, WIDTH, HEIGHT, blank);
}
