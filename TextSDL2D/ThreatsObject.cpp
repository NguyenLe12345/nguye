#include "stdafx.h"
#include "ThreatsObject.h"
ThreatsObject ::ThreatsObject()
 {
	width_frame_ = 0;
	height_frame_ = 0;
	x_val_ = 0.0;
	y_val_ = 0.0;
	x_pos_ = 0.0; 
	y_pos_ = 0.0;
	map_x_ = 0;
	map_y_ = 0;
	on_ground_ = false;
	come_back_time = 0;
	frame_= 0;

	animation_a_ = 0;
	animation_b_ = 0;
	input_type_.left_ = 1;
	type_move_ = STATIC_THREAT;
}
ThreatsObject :: ~ThreatsObject()
{

}

bool ThreatsObject::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret)
	{
		width_frame_ = rect_.w/THREAT_FRAME_NUM;
		height_frame_ = rect_.h;
	}
	return ret;
}

void ThreatsObject::Set_clips() 
{
	if (width_frame_ > 0 && height_frame_ > 0)
	{
		for (int i = 0; i < THREAT_FRAME_NUM; i++)
		{
			frame_clip_[i].x =i*width_frame_;
			frame_clip_[i].y = 0;
			frame_clip_[i].w = width_frame_;
			frame_clip_[i].h = height_frame_;
		}
	}
}

void ThreatsObject::Show(SDL_Renderer* des)
{
	if (come_back_time == 0)
	{
		rect_.x = x_pos_ - map_x_;
		rect_.y = y_pos_ - map_y_;
		frame_++;
		if (frame_ >= 8)
		{
			frame_ = 0;
		}
	}

	SDL_Rect* currentClip = &frame_clip_[frame_]; // thay duoc cai frame hien tai
	SDL_Rect  rendQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
	SDL_RenderCopy(des, p_object_, currentClip, &rendQuad);
}
void ThreatsObject::Doplayer(Map& gMap)
{
	if (come_back_time == 0)
	{
		x_val_ = 0;
		y_val_ = 10;
		if (y_val_ >= THREAT_MAX_FALL_SPEED)
		{
			y_val_ += THREAT_MAX_FALL_SPEED;
		}

		if (input_type_.left_ == 1)
		{
			x_val_ -= THREAT_SPEED;
		}
		else if (input_type_.right_ == 1)
		{
			x_val_ += THREAT_SPEED;
		}

		CheckToMap(gMap); // kiem tra xem no co va cham voi ban do hay khong
	}
   else if (come_back_time > 0)
	{
		come_back_time--;
		if (come_back_time== 0)
		{
			InitThreats();
		}
	}
}
void ThreatsObject::InitThreats()
{
	x_val_ = 0;
	y_val_ = 0;
	if (x_pos_ > 256)
	{
		x_pos_ -= 256;
		animation_a_ -= 256;
		animation_b_ -= 256;
	}
	else
	{
		x_pos_ = 0;
	}
	y_pos_ = 0;
	come_back_time = 0;
	input_type_.left_ = 1;
}
void ThreatsObject:: RemoveBullet(const int& idx)
{
	int size = bullet_list_.size();
	if (size > 0 && idx < size)
	{
		BulletObject* p_bullet = bullet_list_.at(idx);
		bullet_list_.erase(bullet_list_.begin() + idx);
		if (p_bullet)
		{
			delete p_bullet;
			p_bullet = NULL;
		}
	}
}
 SDL_Rect ThreatsObject::GetRectFrame()
{
	SDL_Rect rect;
	rect.x = rect_.x;
	rect.y = rect_.y;
	rect.w = width_frame_;
	rect.h = height_frame_;

	return rect;
}
void ThreatsObject::CheckToMap(Map& map_data)
{
	int x1 = 0;
	int x2 = 0;

	int y1 = 0;
	int y2 = 0;
	// Check theo chieu ngang truoc
	int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;
	x1 = (x_pos_ + x_val_) / TILE_SIZE;
	x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;

	y1 = (y_pos_) / TILE_SIZE;
	y2 = (y_pos_ + height_min - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if (x_val_ > 0)
		{
			int val1 = map_data.tile[y1][x2];
			int val2 = map_data.tile[y2][x2];
	        if ((val1 != THREAT_BLANK_TILE && val1!=STATE_MONEY) || (val2 != THREAT_BLANK_TILE && val2!=STATE_MONEY))
				{
			        
					x_pos_ = x2 * TILE_SIZE;
					x_pos_ -= width_frame_ + 1;
					x_val_ = 0;
				}
		}
		else if (x_val_ < 0)
		{

			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y2][x1];
	     	if ((val1 != THREAT_BLANK_TILE && val1 != STATE_MONEY )|| (val2 != THREAT_BLANK_TILE && val2!= STATE_MONEY))
				{
				
					x_pos_ = (x1 + 1) * TILE_SIZE;
					x_val_ = 0;
				}
		}
	}
	// di chuyen theo chieu doc
	int width_min = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
	x1 = (x_pos_) / TILE_SIZE;
	x2 = (x_pos_ + width_min) / TILE_SIZE;

	y1 = (y_pos_ + y_val_) / TILE_SIZE;
	y2 = (y_pos_ + height_frame_ + y_val_ - 1) / TILE_SIZE;
	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if (y_val_ > 0)
		{
			int val1 = map_data.tile[y2][x1];
			int val2 = map_data.tile[y2][x2];
			if ((val1 != THREAT_BLANK_TILE && val1!=STATE_MONEY)|| ( val2 != THREAT_BLANK_TILE && val2!=STATE_MONEY))
				{
				  
					y_pos_ = y2 * TILE_SIZE;
					y_pos_ -= (height_frame_ + 1);
					y_val_ = 0;
					on_ground_ = true;
					
				}
			
			//	on_ground_ = true;
		}
		else if (y_val_ < 0)
		{
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y1][x2];
			if ((val1 != THREAT_BLANK_TILE && val1 != STATE_MONEY) || (val2 != THREAT_BLANK_TILE && val2 !=STATE_MONEY))
				{
					y_pos_ = (y1 + 1) * TILE_SIZE;
					y_val_ = 0;
				}
		}
	}
	x_pos_ += x_val_;
	y_pos_ += y_val_;
	if (x_pos_ < 0)
	{
		x_pos_ = 0;
	}
	else if (x_pos_ + width_frame_ > map_data.max_x_)
	{
		x_pos_ = map_data.max_x_ - width_frame_ - 1;
	}
	if (y_pos_ > map_data.max_y_)
	{
		come_back_time = 60;  // thoi gian tro lai la 60;
	}
}


void ThreatsObject::ImpMoveType(SDL_Renderer* screen)
{
	if (type_move_ == STATIC_THREAT)
	{
		;
	}
	else
	{
		if (on_ground_ == true)
		{
			if (x_pos_ > animation_b_)
			{
				input_type_.left_ = 1;
				input_type_.right_ = 0;
				LoadImg("img//threat_left.png", screen);
			}
			else if (x_pos_ < animation_a_)
			{
				input_type_.right_ = 1;
				input_type_.left_ = 0;
				LoadImg("img//threat_right.png", screen);
			}
		}
		else if (on_ground_==false)
		{
			if (input_type_.left_ == 1)
			{
				LoadImg("img//threat_left.png", screen);
				
			}
		}
	}
}
void ThreatsObject::InitBullet(BulletObject* p_bullet, SDL_Renderer* screen)
{
	if (p_bullet != NULL)
	{
		p_bullet->set_bullet_type(BulletObject::SPHERE_BULLET);
		bool ret = p_bullet->LoadImgBullet(screen);// tu dong lay vien dan ra
		if (ret)
		{
			p_bullet->set_is_move(true); // kiem tra vien dan co trong man hinh hay khong
			p_bullet->set_bullet_dir(BulletObject::DIR_LEFT); // xet huong
			p_bullet->SetRect( x_pos_+ 10, y_pos_+ 10); // vi tr xuat phat cua vien dan
			p_bullet->set_x_val(15); // toc do cua vien dan
			bullet_list_.push_back(p_bullet);
		}
	}
}
void ThreatsObject::MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit)
{
	for (int i = 0; i < bullet_list_.size(); i++)
	{
		BulletObject* p_bullets = bullet_list_.at(i);
		if (p_bullets != NULL)
		{
			if (p_bullets->get_is_move())
			{
				int bullet_distance = rect_.x + width_frame_ - p_bullets->GetRect().x; // khoang cach ban 
				if (bullet_distance < 400 && bullet_distance > 0)
				{
					p_bullets->HandleMove(x_limit, y_limit); // huong ban dan va kiem tra coi dan co nam trong man hinh khong
					p_bullets->Render(screen);
				}
				else
				{
					p_bullets->set_is_move(false);
				}
		   }
			else
			{
				p_bullets->set_is_move(true) ;
				p_bullets->SetRect(this->rect_.x+10, this->rect_.y+10);

			}
		}

	}
}
