#ifndef PLAYERPOWER__H_
#define PLAYERPOWER__H_

#include "CommonFunc.h"
#include "BaseObject.h"

class PlayerPower : public BaseObject
{
public : 
	PlayerPower();
	~PlayerPower();

	void SetNum(const int& number) { number_=number; }
	void AddPos(const int& xPos);

	void Show(SDL_Renderer* screen);
	void Init(SDL_Renderer* screen);

	void InitCrease(); // ham tang mang
	void Decrerase(); // ham giam mang
private:

	int number_; // so mang
	std::vector<int> pos_list_; // moi o 1 mang
};


class PlayerMoney : public BaseObject
{
public:

	PlayerMoney();
	~PlayerMoney();

	void Init(SDL_Renderer* screen);
	void Show(SDL_Renderer* screen);
	void SetPos(const int& x, const int& y) {x_pos_=x, y_pos_=y;}
private:
	int x_pos_;
	int y_pos_;
};



#endif