#ifndef TEXTOBJECT__H_
#define TEXTOBJECT__H_

#include "CommonFunc.h"
class TextObject
{
public:
	TextObject();
	~TextObject();

	enum TextColor
	{
		RED_TEXT = 0, // mau do
		WHITE_TEXT = 1, // mau trang
		BLANK_TEXT = 2, // mau den
	};
	
	bool LoadFromRenderText (TTF_Font* font, SDL_Renderer* screen); // kieu chu, phong chu
	void Free();

	void SetColor(Uint8 red, Uint8 green, Uint8 blue); // truyen truc tiep mau vao
	void SetColor(int type);
	void RenderText(SDL_Renderer* screen, 
		             int xp, int yp,
					 SDL_Rect* clip=NULL, 
					 double angle = 0.0, 
					 SDL_Point* center= NULL, 
					 SDL_RendererFlip flip=SDL_FLIP_NONE);
	
	int GetWidth() const {return width_;}
	int  GetHeight() const { return height_; }
	void SetRect(const int& x, const int& y) { rect_.x = x, rect_.y = y;}
	SDL_Rect GetRect() const { return rect_;} // lay ra rect_ cua tam anh;
	void SetText(const std::string& text) {str_val_=text;}
	std::string GetText() const {return str_val_;}
private:
	std::string str_val_; // luu noi  dung cua tex
	SDL_Color text_color_; // luu mau sac
	SDL_Texture* texture_; // dung quan ly tex
	int width_;
	int height_;
	SDL_Rect rect_;

};
#endif 