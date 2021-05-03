
#ifndef BULLETOBJECT__H_
#define BULLETOBJECT__H_
#include "BaseObject.h"
#include "CommonFunc.h"
#include "stdafx.h"


class BulletObject : public BaseObject
{

public:
	BulletObject();
	~BulletObject();
	enum BulletDir
	{
		DIR_RIGHT = 20,
		DIR_LEFT = 21,
		DIR_UP = 22,
		DIR_UP_LEFT=23,
		DIR_UP_RIGHT=24,
		DIR_DOW_LEFT=25,
		DIR_DOW_RIGHT=26,
	};
	enum BulletTulye
	{
		SPHERE_BULLET = 50,
		LASER_BULLET = 51,

	};
	void set_x_val(const int& xval) { x_val_ = xval; }
	void set_y_val(const int& yval) { y_val_ = yval; }
	int  get_x_val() const { return x_val_; }
	int get_y_val() const { return y_val_; }
	void set_is_move(const bool& ismove) { is_move_ = ismove; }
	bool get_is_move() const { return is_move_; }
	void HandleMove(const int& x_border, const int& y_border); // chiu su di chuyen cho vien dan
	void set_bullet_dir(const unsigned int& bullet_dir) { bullet_dir_ = bullet_dir; }
	unsigned int get_bullet_dir() const { return bullet_dir_; }
	void set_bullet_type(const unsigned int& bullet_type) { bullet_type_ = bullet_type; }
	unsigned int get_bullet_type() const { return bullet_type_; }
	bool LoadImgBullet(SDL_Renderer* des);
private: 
	int x_val_;
	int y_val_;
	bool is_move_;//neu trong man hinh thi vien dan la true con ra ngoai man hinh thi thoc tinh nay la flase xoa no di
	unsigned int bullet_dir_; // huong cua vien dan
	unsigned int bullet_type_;
};

#endif // !BULLETOBJECT__H_
