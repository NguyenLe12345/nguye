#ifndef BOSS_OBJECT_H_
#define BOSS_OBJECT_H_

#include "BaseObject.h"
#include "CommonFunc.h"
#include "BulletObject.h"

#define GAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10

#define PLAYER_SPEED 2
#define PLAYER_HIGHT_VAL 18;

#define FRAME_NUM_32 32
#define BLANK_TILE 0
class BossObject : public BaseObject
{
public:
	BossObject();
	~BossObject();
	void set_x_val(int xp) {x_val_ = xp;}
	void set_y_val(int yp) {y_val_ = yp;}

	void set_xpos(const int& xpos) { x_pos_ = xpos;}
	void set_ypos(const int& ypos) { y_pos_ = ypos;}
	
	int get_x_pos() {return x_pos_;}
	int get_y_pos() {return y_pos_;}
	void set_frame(const int& fra) {frame_=fra;}

	void Show(SDL_Renderer* des);
	
	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Set_Clips();
	void set_clip(); // anh cua hinh vu no
	int get_width_frame() const {return width_frame_;}
	int get_height_frame() const {return height_frame_;}
	bool LoadImg_no(std::string path, SDL_Renderer* screen);
	void SetMapXY(const int& map_x, const int& map_y) {map_x_=map_x, map_y_=map_y;};

	void CheckToMap(Map& g_map);
	void CenterEntityOnMap(Map& g_map);
	void DoPlayer(Map& g_map);
	void InitPlayer();
	std::vector<BulletObject*> get_bullet_list() const {return bullet_list_;}
	void set_bullet_list(const std::vector<BulletObject*>& am_list) {bullet_list_ = am_list;}

    void InitBullet(SDL_Renderer* screen);
	void MakeBullet(SDL_Renderer* des, const int& x_limit, const int& y_limit);
	SDL_Rect GetRectFrame();
	SDL_Rect GetRect() const {return rect_;}
    void Free();
	void RemoveBullet(const int& idx); // xoa vien dan thu idx
	void Clear_boss();
private:
	int map_x_;
	int map_y_;
	int on_ground_;
	int think_time_;
	Input input_type_;
	int frame_;
	
	SDL_Rect frame_clip_[FRAME_NUM_32];

	int x_pos_;
	int y_pos_;
	float x_val_;
	float y_val_;
	int width_frame_;
	int height_frame_;

	std::vector<BulletObject*> bullet_list_;
};

#endif