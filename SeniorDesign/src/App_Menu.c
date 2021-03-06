#include "App_Menu.h"
#include "Input.h"
#include "Draw.h"

int idx;
int frame;
int x, y;

void App_Menu_Init(void)
{
   clearDisplay();
   idx   = 0;
   frame = 0;
   apps[idx].Demo_Init();
   setPixel(0, idx, PIXEL_GREEN);

   Input_Tap = 0;
}


void App_Menu_Tick(void)
{
   if (frame == 2)
   {
      frame = 0;
         if (Input_Tap & UP_INPUT || P1_Long_Hold(UP_INPUT, 8))
         {
            setPixel(0, idx, PIXEL_BLACK);
            apps[idx].Demo_Deinit();
            idx--;
            if (idx < 0)
            {
               idx = 0;
            }
			drawRect(0, 0, WIDTH, HEIGHT, PIXEL_BLACK);
			apps[idx].Demo_Init();
         }
         else if (Input_Tap & DOWN_INPUT || P1_Long_Hold(DOWN_INPUT, 8))
         {
            setPixel(0, idx, PIXEL_BLACK);
            apps[idx].Demo_Deinit();
            idx++;
            if (idx >= APP_COUNT)
            {
               idx = APP_COUNT - 1;
            }
			drawRect(0, 0, WIDTH, HEIGHT, PIXEL_BLACK);
			apps[idx].Demo_Init();
         }
         if (Input_Tap & A_INPUT)
         {
            apps[idx].Demo_Deinit();
            currentApp = &apps[idx];
            clearDisplay();
            currentApp->App_Init();
            idx = -1;
         }

         Input_Tap &= ~(UP_INPUT | DOWN_INPUT | A_INPUT);
   }
   if (idx != -1)
   {
      apps[idx].Demo_Tick();
      frame++;
	  drawRect(0, 0, 1, HEIGHT, PIXEL_BLACK);
	  setPixel(0, idx, PIXEL_GREEN);
   }
}


void App_Menu_Deinit(void)
{
}


void App_Menu_New(App *app)
{
   app->Demo_Init   = 0;
   app->Demo_Tick   = 0;
   app->Demo_Deinit = 0;
   app->App_Init    = &App_Menu_Init;
   app->App_Tick    = &App_Menu_Tick;
   app->App_Deinit  = &App_Menu_Deinit;
   AppStorage = &_AppStorage;
}


void App_Menu_Poll()
{
   if (Input_Tap & START_INPUT)
   {
      if (currentApp != &homeApp)
      {
         currentApp->App_Deinit();
         currentApp = &homeApp;
         currentApp->App_Init();
      }
      idx = 0;
      drawRect(0, 0, WIDTH, HEIGHT, PIXEL_BLACK);
      apps[idx].Demo_Init();
      setPixel(0, idx, PIXEL_GREEN);
   }else if (Input_Tap & SELECT_INPUT)
   {
      currentApp->App_Deinit();
      drawRect(0, 0, WIDTH, HEIGHT, PIXEL_BLACK);
      currentApp->App_Init();
   }
   Input_Tap &= ~(START_INPUT | SELECT_INPUT);
}
