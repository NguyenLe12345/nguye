
#ifndef BASEOBJECT__H_
#define BASEOBJECT__H_

#include "CommonFunc.h"
#include "TextObject.h"
#include "stdafx.h"

class BaseObject {

public:
	BaseObject();
    ~BaseObject();
	void SetRect(const int& x, const int& y) { rect_.x = x, rect_.y = y; }
	SDL_Rect GetRect() const { return rect_;} // lay ra rect_ cua tam anh;
	SDL_Texture* GetObject() const { return p_object_; }
	virtual bool LoadImg(std::string path, SDL_Renderer* screen);
	void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
	void Free();
	int ShowMenu(SDL_Renderer* des, TTF_Font* font);

	bool CheckFocusWidthRect(const int& x, const int& y, const SDL_Rect& rect);  // kiem tra toa do cua con chuot voi hinh anh menu
protected: 
	SDL_Texture* p_object_; // luu tru cac hinh anh
	SDL_Rect rect_;
};

#endif // !BASEOBJECT__H_
