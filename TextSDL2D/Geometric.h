// dung de ve hinh hoc

#ifndef GEOMETRIC__H_
#define GEOMETRIC__H_

#include "CommonFunc.h"
typedef struct GeometricFormat  // kich thuoc
{
public:
	GeometricFormat(int left, int top, int width, int height) {left_=left, top_=top, width_=width, height_=height;};
	//~GeometricFormat();

	int left_;
	int top_;
	int width_;
	int height_;
};
typedef struct ColorData // mau sac
{
	ColorData(Uint8 r, Uint8 g, Uint8 b) {red_=r, green_=g, blue_=b;}
public:
	Uint8 red_;
	Uint8 green_;
	Uint8 blue_;
};


class Geometric
{
public:
	static void RenderRectangle(const GeometricFormat& geo_size, const ColorData& color_data, SDL_Renderer* screen );
	// ham the hien duong vien
	static void RenderOutline(const GeometricFormat& geo_size, const ColorData& color_data, SDL_Renderer* screen );
};





#endif