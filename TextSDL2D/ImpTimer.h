
#ifndef IMPTIMER__H_
#define IMPTIMER__H_
#include "stdafx.h"
class ImpTimer
{
public:
	ImpTimer();
	~ImpTimer();

	void start();
	void stop();
	void paused();
	void unpaused();

	int get_ticks();
	bool is_paused();
	bool is_started();


private:
	int start_tick_; // bat dau chay ow thoi diem nao
	int paused_tick_; // thoi diem tam dung
	bool is_paused_; // trang thai
	bool is_started_;
};



#endif // !IMPTIMER__H_