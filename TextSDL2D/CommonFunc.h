
#ifndef COM_MONFUNC_H_
#define COM_MONFUNC_H_
#include "stdafx.h"
#include<Windows.h>
#include<string>
#include<vector>
#include<SDL_image.h>
#include<SDL.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#undef main

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

static Mix_Chunk* g_sound_bullet[2];
static Mix_Chunk* g_sound_exp[1];
//Screen
const int FRAME_PER_SECOND = 25; // so frame load trong 1s;
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

const int COLER_KEY_R = 167;
const int COLER_KEY_G = 175;
const int COLER_KEY_B = 180;

const int RENDER_DRAW_COLOR = 255;

#define TILE_SIZE 64
#define MAX_MAP_X 400
#define MAX_MAP_Y 10

#define STATE_MONEY 14
 struct Input
{
	    int left_;
		int right_;
		int up_;
		int dow_;
		int jump_; // nhay
};
  struct Map
{
	int stat_x_; 
	int stat_y_;

	int max_x_;
	int max_y_;
	int tile[MAX_MAP_Y][MAX_MAP_X];
	char* file_name_;
};
  // ham check va tram
  namespace SDLCommonFunc1
  {
	  bool CheckCollision(  const SDL_Rect& object1, const SDL_Rect& object2);
  }
 
#endif // !COMMONFUNC__H_