
#include "stdafx.h"
#include "BaseObject.h"



BaseObject::BaseObject()
{
	p_object_ = NULL;
	rect_.x = 0;
	rect_.y = 0;
	rect_.w = 0;
	rect_.h = 0;
}
BaseObject:: ~BaseObject()
{
	Free();
}
bool BaseObject :: CheckFocusWidthRect(const int& x , const int& y, const SDL_Rect& rect)
{
	if (x>= rect.x && x<=rect.x + rect.w &&
		y>=rect.y && y<=rect.y+rect.h)
	{
		return true;
	}
	return false;
}

BaseObject d_menu;
int BaseObject :: ShowMenu(SDL_Renderer* des, TTF_Font* font)

{   
	
	bool g_imt_menu = false;

	g_imt_menu = d_menu.LoadImg("img//menu.png", des);
	if (g_imt_menu == false)
	{
		return 1;
	}
	const int kMenuItemNum = 2;
	SDL_Rect pos_arr[kMenuItemNum];
	pos_arr[0].x = 200;
	pos_arr[0].y = 400;
	

	pos_arr[1].x = 200;
	pos_arr[1].y = 500;
	

	TextObject text_menu[kMenuItemNum];

	text_menu[0].SetText("play Game");
	text_menu[0].SetColor(TextObject :: BLANK_TEXT);
	text_menu[0].SetRect(pos_arr[0].x, pos_arr[0].y);
	


	text_menu[1].SetText("Exit");
	text_menu[1].SetColor(TextObject :: BLANK_TEXT);
	text_menu[1].SetRect(pos_arr[1].x, pos_arr[1].y);
	

	bool selected[kMenuItemNum] = {0, 0};

	int xm=0;
	int ym=0;

	SDL_Event m_event;
	while(true)
	{
		SDL_SetRenderDrawColor(des, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(des);
		d_menu.Render(des, NULL); // luu y
		 for (int i=0; i< kMenuItemNum; i++)
		{
			text_menu[i].LoadFromRenderText(font, des);
			text_menu[i].RenderText(des, pos_arr[i].x, pos_arr[i].y);
		}
		while(SDL_PollEvent(&m_event))
		{
			switch (m_event.type)
			{
			case SDL_QUIT:
				return 1;
			case SDL_MOUSEMOTION:
				{
					 
					// lay toa do cua con chuot
					xm = m_event.motion.x; 
					ym = m_event.motion.y;

					for (int i=0; i< kMenuItemNum; i++)
					{
						 if (CheckFocusWidthRect(xm, ym, text_menu[i].GetRect()))
						 {
						     if (selected[i]==false)
							{
								selected[i] = 1;
								text_menu[i].SetColor(TextObject :: RED_TEXT);
							}
						 }
						 else
						{
							if (selected[i]==true)
							{
								selected[i] = 0;
								text_menu[i].SetColor(TextObject :: BLANK_TEXT);
							}
						}


					}

				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				{
					xm = m_event.button.x;
					ym = m_event.button.y;
					for (int i=0; i< kMenuItemNum; i++)
					{
					if (CheckFocusWidthRect(xm, ym, text_menu[i].GetRect()))
						 {
						     return i;
						 }
					}
				}
				break;
			case SDL_KEYDOWN:
				if (m_event.key.keysym.sym==SDLK_ESCAPE)
				{
					return 1;
				}
			default:
				break;
			}
		}
		SDL_RenderPresent(des);
	}
    
	return 1;
}



bool BaseObject::LoadImg(std::string path, SDL_Renderer* screen)
{
	Free();
	SDL_Texture* new_texture=NULL;
	SDL_Surface* load_Suaface = IMG_Load(path.c_str());
	if (load_Suaface!=NULL)
	{
		SDL_SetColorKey(load_Suaface, SDL_TRUE, SDL_MapRGB(load_Suaface->format, COLER_KEY_R, COLER_KEY_G, COLER_KEY_B)); // xoa anh nen
		new_texture = SDL_CreateTextureFromSurface(screen, load_Suaface); //giup nhan thong tin gio rang va chinh xac hon
		if (new_texture != NULL)
		{
			rect_.w = load_Suaface->w;
			rect_.h = load_Suaface->h;
		}
		SDL_FreeSurface(load_Suaface);
	}
	p_object_ = new_texture;
	return p_object_ != NULL;
}
void BaseObject::Render(SDL_Renderer* des, const SDL_Rect* clip/*=NULL*/) // LOAD anh len man hinh des
{
	SDL_Rect rederquad = { rect_.x, rect_.y, rect_.w, rect_.h };
	SDL_RenderCopy (des, p_object_, clip, &rederquad);

}

void BaseObject::Free()
{
	if (p_object_ != NULL)
	{
		SDL_DestroyTexture(p_object_);
		p_object_ = NULL; 
		rect_.w = 0;
		rect_.h = 0;
	}
}
