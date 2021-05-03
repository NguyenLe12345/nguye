
#ifndef THREATSOBJECT__H_
#define THREATSOBJECT__H_
#include "BaseObject.h"
#include "CommonFunc.h"
#include "MainObject.h"
#include "BulletObject.h"
#include "stdafx.h"

#define THREAT_BLANK_TILE 0
#define THREAT_GRAVITY_SPEED 0.8
#define THREAT_FRAME_NUM 8
#define THREAT_MAX_FALL_SPEED 3

#define THREAT_SPEED 3
class ThreatsObject : public BaseObject
{
public:
	enum TyMove
	{
		STATIC_THREAT = 0,
	    MOVE_IN_SPACE_THREAT=1,// trong 1 pham vi
	};
	ThreatsObject();
	~ThreatsObject();
	void set_x_val(const float& x_val) { x_val_ = x_val; }
	//float get_x_val() const { return x_val_; } chua can ham nay vi khong an lay ra
	void set_y_val_(const float& y_val) { y_val_ = y_val; }
	//float get_y_val() const { return y_val_; }
	void set_x_pos(const float& x_pos) { x_pos_ = x_pos; }
	void set_y_pos(const float& y_pos) { y_pos_ = y_pos; }
	float get_x_pos() const { return x_pos_; }
	float get_y_pos() const { return y_pos_; }
	void SetMapXY(const int& map_x, const int& map_y) { map_x_ = map_x; map_y_ = map_y; }
	
	void Set_clips(); // xay dung cac thong so(hay clip cho cac frame;
	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des); // hien thi hinh anh
	int get_width_frame() const { return width_frame_; }
	int get_height_frame() const { return height_frame_; }
	void Doplayer(Map& gMap); // su ly viec di chuyen hay va cham vao ban do
	void CheckToMap(Map& gMap);
	void InitThreats();
	void set_type_move(const int& type_move) { type_move_ = type_move; }
	void SetAnimationPos(const int& pos_a, const int& pos_b) { animation_a_ = pos_a; animation_b_ = pos_b; }
	void set_input_left(const int& ipleft) { input_type_.left_ = ipleft; }
	void ImpMoveType(SDL_Renderer* screen); // su ly di chuyen tu a->b
	
	void set_bullet_list(const std::vector<BulletObject*>& bullet_list) { bullet_list_ = bullet_list; } //su ky va cham
	std::vector<BulletObject*> get_bullet_list() const { return bullet_list_; }
	void InitBullet(BulletObject* p_bullet, SDL_Renderer* screen); // khoi tao cac thong so bullet
	void MakeBullet(SDL_Renderer* screeen, const int& x_limit, const int& y_limit);
	void RemoveBullet(const int& idx); // xoa vien dan thu idx
	 SDL_Rect GetRectFrame();
private:
	int map_x_;
	int map_y_;
	float x_val_;
    float y_val_;
	float x_pos_;
	float y_pos_;
	bool on_ground_;
	int come_back_time;
	SDL_Rect frame_clip_[THREAT_FRAME_NUM];
	int frame_;
	int width_frame_;
	int height_frame_;
	
	int type_move_; // kieu di chuyen
	// gioi han di chuyen a, b
	int animation_a_; 
	int animation_b_;
	Input input_type_; // di chuyen

	std::vector<BulletObject*> bullet_list_;
};


#endif // !THREATSOBJECT__H_
