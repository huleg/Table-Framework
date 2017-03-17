#include "App_Basic.h"
#include "Draw.h"
#include "Input.h"
#include "Random.h"

#define DIRECTION 0xF
#define DIRECTION_UP 1
#define DIRECTION_DOWN 2
#define DIRECTION_LEFT 4
#define DIRECTION_RIGHT 8
#define SEGMENT_ACTIVE 16
#define GAME_OVER 32

struct appData {	
	unsigned char headX;
	unsigned char headY;
	unsigned char foodX;
	unsigned char foodY;
	unsigned short headIdx;
	unsigned short length;
	char frame;
	unsigned char Direction;
	unsigned char pastKeys;
	unsigned char snake[WIDTH*HEIGHT];
} typedef appData;
appData* Snake_Data;

void App_Snake_Init(void);
void App_Snake_Deinit(void);
void App_Snake_Place_Food(void);

void Demo_Snake_Init(void) {
	App_Snake_Init();
}

void Demo_Snake_Tick(void) {
}

void Demo_Snake_Deinit(void) {
	App_Snake_Deinit();
}

void App_Snake_Init(void) {
	Snake_Data = &AppStorage;
	Snake_Data->headX = getRandom() % WIDTH;
	Snake_Data->headY = getRandom() % HEIGHT;
	Snake_Data->headIdx = 0;
	Snake_Data->length = 1;
	Snake_Data->frame = 0;

	Snake_Data->snake[0] = DIRECTION_RIGHT | SEGMENT_ACTIVE;
	App_Snake_Place_Food();
}

void App_Snake_Remove_Tail() {
	unsigned char x, y;
	unsigned short idx, offset;
	x = Snake_Data->headX;
	y = Snake_Data->headY;
	offset = Snake_Data->headIdx;

	// Consider storing the tail end, and detecting what the next peice is by scanning the 4 neighbors.

	for (idx = 0; idx < Snake_Data->length; idx++) {
		if (offset - idx == 0 ) {
			offset += WIDTH * HEIGHT;
		}
		switch (Snake_Data->snake[offset - idx] & ~SEGMENT_ACTIVE)
		{
		case DIRECTION_UP:
			y++;
			break;
		case DIRECTION_DOWN:
			y--;
			break;
		case DIRECTION_LEFT:
			x++;
			break;
		case DIRECTION_RIGHT:
			x--;
			break;
		default:
			break;
		}
	}
	idx--;
	Snake_Data->snake[offset - idx] = 0;
	setPixel(x, y, PIXEL_BLACK);
}

void App_Snake_Place_Food() {
	Pixel tmp;
	while(1) {
		Snake_Data->foodX = getRandom() % WIDTH;
		Snake_Data->foodY = getRandom() % HEIGHT;
		tmp = getPixel(Snake_Data->foodX, Snake_Data->foodY);
		if (tmp.R == tmp.G == tmp.B == 0) {
			break;
		}
	}
	setPixel(Snake_Data->foodX, Snake_Data->foodY, PIXEL_GREEN);
}

void App_Snake_Game_Over_Tick(){
	if (Snake_Data->frame % 4 == 3) {
		setPixel(getRandom() % WIDTH, getRandom() % HEIGHT, PIXEL_RED);
		if (Input_Status & ~Snake_Data->pastKeys) {
			clearDisplay();
			App_Snake_Deinit();
			App_Snake_Init();
		}
		Snake_Data->pastKeys = Input_Status & 0xFF;
	}
}

void App_Snake_Tick(void) {
	unsigned char Input;
	Pixel tmp;
	if (Snake_Data->Direction & GAME_OVER) {
		App_Snake_Game_Over_Tick();
	}
	else if (Snake_Data->frame == 6) {
		Snake_Data->frame = 0;
		
		if (Snake_Data->Direction & SEGMENT_ACTIVE) {
			Snake_Data->Direction ^= SEGMENT_ACTIVE;
			Snake_Data->length++;
		}
		else {
			App_Snake_Remove_Tail();
		}
		Input = Input_Status & ~Snake_Data->pastKeys;
		Snake_Data->pastKeys = Input_Status & 0xFF;
		if (Input) {
			if (Input & UP_INPUT && Snake_Data->Direction != DIRECTION_DOWN) {
				Snake_Data->Direction = DIRECTION_UP;
			} else if (Input & DOWN_INPUT && Snake_Data->Direction != DIRECTION_UP) {
				Snake_Data->Direction = DIRECTION_DOWN;
			} else if (Input & LEFT_INPUT && Snake_Data->Direction != DIRECTION_RIGHT) {
				Snake_Data->Direction = LEFT_INPUT;
			} else if (Input & RIGHT_INPUT && Snake_Data->Direction != DIRECTION_LEFT) {
				Snake_Data->Direction = DIRECTION_RIGHT;
			}
			if (!Snake_Data->Direction) {
				return;
			}

			if (Input & A_INPUT) {
				Snake_Data->Direction |= SEGMENT_ACTIVE;
			}

			if (Snake_Data->headX < 0|| Snake_Data->headX > WIDTH) {
				Snake_Data->Direction |= SEGMENT_ACTIVE;
				return;
			}

			if (Snake_Data->headY < 0 || Snake_Data->headY > HEIGHT) {
				Snake_Data->Direction |= SEGMENT_ACTIVE;
				return;
			}
		}
		switch (Snake_Data->Direction & ~SEGMENT_ACTIVE)
		{
		case DIRECTION_UP:
			Snake_Data->headY--;
			break;
		case DIRECTION_DOWN:
			Snake_Data->headY++;
			break;
		case DIRECTION_LEFT:
			Snake_Data->headX--;
			break;
		case DIRECTION_RIGHT:
			Snake_Data->headX++;
			break;
		default:
			break;
		}
		tmp = getPixel(Snake_Data->headX, Snake_Data->headY);
		if (Snake_Data->foodX == Snake_Data->headX && Snake_Data->foodY == Snake_Data->headY) {
			Snake_Data->Direction |= SEGMENT_ACTIVE;
			App_Snake_Place_Food();
		} else if (tmp.R || tmp.G || tmp.B || Snake_Data->headX < 0 ||
			Snake_Data->headX >= WIDTH || Snake_Data->headY < 0 || Snake_Data->headY >= HEIGHT) {
			Snake_Data->Direction = GAME_OVER;
		}
		setPixel(Snake_Data->headX, Snake_Data->headY, PIXEL_CYAN);
		Snake_Data->headIdx = (Snake_Data->headIdx + 1) % (WIDTH * HEIGHT);
		Snake_Data->snake[Snake_Data->headIdx] = Snake_Data->Direction & DIRECTION;
	}
	Snake_Data->frame++;
}

void App_Snake_Deinit(void) {
	int i;
	for (i = 0; i < sizeof(appData); i++) {
		AppStorage[i] = 0;
	}

	Snake_Data = 0;
}

void App_Snake_New(App* app) {
	app->Demo_Init = &Demo_Snake_Init;
	app->Demo_Tick = &Demo_Snake_Tick;
	app->Demo_Deinit = &Demo_Snake_Deinit;
	app->App_Init = &App_Snake_Init;
	app->App_Tick = &App_Snake_Tick;
	app->App_Deinit = &App_Snake_Deinit;
}