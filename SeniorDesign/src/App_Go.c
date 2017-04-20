#include "App_Go.h"
#include "Draw.h"
#include "Input.h"
#include <string.h>

#define MAXBOARDSIDE 16
#define MAXBOARDSIZE MAXBOARDSIDE * MAXBOARDSIDE

struct playerData
{
	char x;
	char y;
	Pixel color;
};

struct appData
{
   int frame;
   int size;
   struct playerData player1;
   struct playerData player2;
   struct playerData* currentPlayer;
   unsigned char board[MAXBOARDSIZE];
}
typedef appData;

appData *Go_Data;

void Demo_Go_Init(void);
void Demo_Go_Tick(void);
void Demo_Go_Deinit(void);
void App_Go_Init(void);
void App_Go_Tick(void);
void App_Go_Deinit(void);

void Demo_Go_Init(void)
{
}


void Demo_Go_Tick(void)
{
	drawBackground(PIXEL_BLACK);
	char string[5] = { 'G', 'O', ' ', ' ', ' ' };
	toString(string, 0, PIXEL_LIGHT_GREEN);
}


void Demo_Go_Deinit(void)
{

}


void App_Go_Init(void)
{
	App_Go_Deinit();
	Go_Data = AppStorage;
	
	Go_Data->player1.color = PIXEL_RED;
	Go_Data->player2.color = PIXEL_WHITE;
}


void App_Go_Tick(void)
{
	int P1FrameAt = 15, P2FrameAt = 15;

	if (Input_Status & DOWN_INPUT)
	{
		P1FrameAt = 3;
	}

	// Draw boarder
	// Draw active user board
	// Draw a Red 1 and a Blue 2 to represent players
	// Use a pulsating purple to player color as selector
	// A to place Flash Red base board for invalid?

	//Tetris_Drop(&(Tetris_Data->Player1), 0, Input_Tap);
	//TODO: add user tick handler

	Input_Tap &= ~(RIGHT_INPUT | LEFT_INPUT | A_INPUT | B_INPUT);
	User2_Input_Tap &= ~(RIGHT_INPUT | LEFT_INPUT | A_INPUT | B_INPUT);
}


void App_Go_Deinit(void)
{
   int i;

   for (i = 0; i < sizeof(appData); i++)
   {
       _AppStorage[i] = 0;
   }

   Go_Data = 0;
}


void App_Go_New(App *app)
{
   app->Demo_Init   = &Demo_Go_Init;
   app->Demo_Tick   = &Demo_Go_Tick;
   app->Demo_Deinit = &Demo_Go_Deinit;
   app->App_Init    = &App_Go_Init;
   app->App_Tick    = &App_Go_Tick;
   app->App_Deinit  = &App_Go_Deinit;
}
