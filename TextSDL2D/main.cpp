
#include "stdafx.h"
#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatsObject.h"
#include "BulletObject.h"
#include "ExplosionObject.h"
#include "TextObject.h"
#include "PlayerPower.h"
#include "Geometric.h"
#include "BossObject.h"

#undef main

BaseObject g_Background;
TTF_Font* font_time=NULL;
TTF_Font* g_font_text = NULL;
bool InitData() // khoi tao cac thong so cho moi truong
{
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);// khoi tao moi truong man hinh  SDL
	if (ret< 0)
		return false;

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");// bat tinh nang bat ket cau tuyen tinh
	g_window = SDL_CreateWindow("Gmaes2.0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); // khoi tao cua so window
	if (g_window == NULL)
	{
		success = false;
	}
	else
	{
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED); // tao render cho g_screen lay tu g_window
		if (g_screen == NULL)
			success = false;
		else
		{
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR); // tao mau sac render man hinh thuong dung 255
			// khoi tao tai PNG
			int imgFlags = IMG_INIT_JPG;
			if (!(IMG_Init(imgFlags) && imgFlags))
				success = false;
		}
		if (TTF_Init() == -1)
		{
			success = false;
		}
		font_time = TTF_OpenFont("front//dlxfont_.ttf", 15);
		if (font_time ==NULL)
		{
			success= false;
		}
		g_font_text = TTF_OpenFont("front//dlxfont_.ttf", 50);
		if (g_font_text ==NULL)
		{
			success= false;
		}
	}

	
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096)== -1)
	    return false;

		// Doc file  audio
		g_sound_bullet[0] = Mix_LoadWAV("amthanh//Laser.wav");
		g_sound_bullet[1] = Mix_LoadWAV("amthanh//threat.wav");
		g_sound_exp[0] = Mix_LoadWAV("amthanh//no.wav");

		if (g_sound_bullet[0] == NULL || g_sound_bullet[1] ==NULL || g_sound_exp[0] == NULL)
		{
			return false;

		}

	return success;
}

bool LoadBackground()
{
	bool set = g_Background.LoadImg("img//background.png", g_screen);
	if (set == false)
		return false;

	return true;
}
void close()
{
	g_Background.Free();
	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;
	SDL_DestroyWindow(g_window);
		g_window = NULL;
		IMG_Quit();
		SDL_Quit();
}
std::vector<ThreatsObject*> MakeThreatList()
{
	std::vector<ThreatsObject*> list_threats;
	
	ThreatsObject* threats_objs = new ThreatsObject[20];
	for (int i = 0; i < 20; i++)
	{
		ThreatsObject* p_threat = (threats_objs + i);
		if (p_threat != NULL)
		{
			p_threat->LoadImg("img//threat_level.png", g_screen);
			p_threat->Set_clips();
			p_threat->set_x_pos(i*1200+650);
			p_threat->set_y_pos(250);
			p_threat->set_type_move(ThreatsObject::STATIC_THREAT);                 
			p_threat->set_input_left(0);


			BulletObject* p_bullet = new BulletObject(); // tao ra  mot vien dan
			p_threat->InitBullet(p_bullet, g_screen);

			list_threats.push_back(p_threat);
		}
	}

	ThreatsObject* dynamic_threats= new ThreatsObject[20];
	for (int i = 0; i < 20; i++)
	{
		ThreatsObject* p_threat = (dynamic_threats + i);
		if (p_threat != NULL)
		{
			p_threat->LoadImg("img//threat_left.png", g_screen);
			p_threat->Set_clips();
			p_threat->set_x_pos(i*1000+300);
			p_threat->set_y_pos(200);
			p_threat->set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT);
			
		    int pos1 = p_threat->get_x_pos() - 100;
		    int pos2 = p_threat->get_x_pos() + 100;
		    p_threat->SetAnimationPos(pos1, pos2);
		    list_threats.push_back(p_threat);
		}
	}


	return list_threats;
}


int main(int argc, char* argv[])
{

	ImpTimer fps_timer;

	if (InitData() == false)
		return -1;

	if (LoadBackground() == false)
		return -1;
	GameMap game_map;
	game_map.LoadMap((char*)"map//map01.dat");
	game_map.LoadTiles(g_screen);

	MainObject p_player;
	p_player.LoadImg("img//player_right.png", g_screen);
	p_player.set_clips();


	PlayerPower player_power;
	player_power.Init(g_screen);

	PlayerMoney player_money;
	player_money.Init(g_screen);
	player_money.SetPos(SCREEN_WIDTH*0.5-300, 8);

	std::vector<ThreatsObject*> Threats_list = MakeThreatList();
	
	// Boss Threats
	BossObject bossObject;
	bool ret = bossObject.LoadImg("img//boss_object.png", g_screen);
	if (ret == true)
	{
	bossObject.Set_Clips();
	int xPosBoss = MAX_MAP_X*TILE_SIZE-SCREEN_WIDTH*0.6;
	bossObject.set_xpos(xPosBoss);
	bossObject.set_ypos(200);
	}

	ExplosionObject exp_threat;
	bool tRect = exp_threat.LoadImg("img//exp3.png", g_screen);
	if (!tRect) return -1;
	exp_threat.set_clip();

	ExplosionObject exp_main;
	bool mRect = exp_main.LoadImg("img//exp3.png", g_screen);
	if (!mRect) return -1;
	exp_main.set_clip();

	ExplosionObject img_boss_no;
	bool hRect = img_boss_no.LoadImg("img//exp3.png", g_screen);
	if (!hRect) return -1;
	img_boss_no.set_clip();

	int num_die=0; // so lan chet;

	// Timetext
	TextObject time_game;
	time_game.SetColor(TextObject :: WHITE_TEXT);

	TextObject mark_game;
	mark_game.SetColor(TextObject ::WHITE_TEXT);
	UINT mark_value = 0;
	


	TextObject money_game;
	money_game.SetColor(TextObject :: WHITE_TEXT);

	int boss_am=0;

	bool is_quit = false;
	
	BaseObject ME_NU;
	int ret_menu = ME_NU.ShowMenu(g_screen, g_font_text);
    if (ret_menu == 1)
	{
		is_quit = true;
	}


	while (!is_quit)
	{
		fps_timer.start();
		while (SDL_PollEvent(&g_event) != 0)
		{
			if (g_event.type == SDL_QUIT)
			{
				is_quit = true;
			}
			p_player.HandelInputAction(g_event, g_screen, g_sound_bullet);
		}
		SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(g_screen);

		g_Background.Render(g_screen, NULL);
		//game_map.DrawMap(g_screen);
		Map map_data = game_map.getMap();

		p_player.HandleBullet(g_screen);
		p_player.SetMapXY(map_data.stat_x_, map_data.stat_y_);
		p_player.DoPlayer(map_data);
		p_player.Show(g_screen);

		game_map.SetMap(map_data);
		game_map.DrawMap(g_screen);
		

		//DrawGemitric
		GeometricFormat rectangle_size( 0, 0, SCREEN_WIDTH, 40);
		ColorData color_data1(36, 36 ,36);
		Geometric :: RenderRectangle(rectangle_size, color_data1, g_screen);

		GeometricFormat outlineSize(1, 1, SCREEN_WIDTH -1 , 38);
		ColorData color_data2(255, 255, 255);

		Geometric :: RenderOutline(outlineSize, color_data2, g_screen);

		player_power.Show(g_screen);
		player_money.Show(g_screen);


		
				            bool bCol3= false;
				            SDL_Rect rect2_player = p_player.GetRectFrame();
				            
				            std::vector<BulletObject*> boss_bullet_list = bossObject.get_bullet_list();
				            for (int k=0; k < boss_bullet_list.size() ; k++)
				                {
					                BulletObject* boss_bullet1= boss_bullet_list.at(k);
				                   	SDL_Rect rect_bullet_boss = boss_bullet1->GetRect();
					                bCol3 = SDLCommonFunc1 :: CheckCollision(rect2_player, rect_bullet_boss);
				                 	if (bCol3 == true)
					                     {
											 bossObject.RemoveBullet(k);
											 break;
									     }

			                	}


							if ( bCol3==true)
				{
					int width_exp_frame = exp_main.get_frame_width();
					int height_exp_frame = exp_main.get_frame_height();
					for (int ex2=0; ex2 < 4; ex2 ++)
					      {
								int x_pos = (p_player.GetRect().x+p_player.get_frame_width()*0.5)-width_exp_frame*0.5;
								int y_pos = (p_player.GetRect().y+p_player.get_frame_height()*0.5)-height_exp_frame*0.5;

								exp_main.set_frame(ex2);
								exp_main.SetRect(x_pos, y_pos);
								exp_main.Show(g_screen);
								SDL_RenderPresent(g_screen);
								Mix_PlayChannel(-1, g_sound_exp[0], 0);
						 }
					   num_die++;
					   if (num_die<=3)
					   {

						   p_player.SetRect(0, 0);
						   p_player.set_comeback_time(60);
						   SDL_Delay(1000);
						   player_power.Decrerase();
						   player_power.Render(g_screen);
						   continue;
					   }
					   else 
					   {
						   	if (MessageBox(NULL, L"GAMES OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
				        	{
					        	
					        	close();
						        SDL_Quit();
						        return 0;
					
				        	}
					   }
				}





		for ( int i = 0; i < Threats_list.size(); i++)
		{
			ThreatsObject* p_threat = Threats_list.at(i);
			if (p_threat != NULL)
			{
				p_threat->SetMapXY(map_data.stat_x_, map_data.stat_y_);
				p_threat->ImpMoveType(g_screen);
				p_threat->Doplayer(map_data);
				p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
				p_threat->Show(g_screen);
				
				// va cham cua dan threat voi nhan vat
			   SDL_Rect rect1_player = p_player.GetRectFrame();
		       bool bCol1 = false;
 				std::vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();
				for (int jj = 0; jj < tBullet_list.size(); jj++)
				{
					BulletObject* pt_bullet = tBullet_list.at(jj);
					if (pt_bullet!=NULL)
					{
						SDL_Rect rect_bullet =  pt_bullet->GetRect();
						 bCol1 = SDLCommonFunc1::CheckCollision(rect_bullet,rect1_player);
						 if (bCol1==true)
						{

							p_threat->RemoveBullet(jj);
							break;
							
						}
					}

				}


				 

				// vs cham cua nhan vat voi threat
				SDL_Rect rect_threat = p_threat->GetRectFrame();
				bool bCol2 = false;
				bCol2 = SDLCommonFunc1::CheckCollision(rect1_player, rect_threat);
			
				if ( bCol2 ==true || bCol1==true )
				{
					int width_exp_frame = exp_main.get_frame_width();
					int height_exp_frame = exp_main.get_frame_height();
					for (int ex2=0; ex2 < 4; ex2 ++)
					      {
								int x_pos = (p_player.GetRect().x+p_player.get_frame_width()*0.5)-width_exp_frame*0.5;
								int y_pos = (p_player.GetRect().y+p_player.get_frame_height()*0.5)-height_exp_frame*0.5;

								exp_main.set_frame(ex2);
								exp_main.SetRect(x_pos, y_pos);
								exp_main.Show(g_screen);
								SDL_RenderPresent(g_screen);
								Mix_PlayChannel(-1, g_sound_exp[0], 0);
						 }
					   num_die++;
					   if (num_die<=3)
					   {

						   p_player.SetRect(0, 0);
						   p_player.set_comeback_time(60);
						   SDL_Delay(1000);
						   player_power.Decrerase();
						   player_power.Render(g_screen);
						   continue;
					   }
					   else 
					   {
						   	if (MessageBox(NULL, L"GAMES OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
				        	{
					        	p_threat->Free();
					        	close();
						        SDL_Quit();
						        return 0;
					
				        	}
					   }

				}
			}
		}
		

		// va cham dan nhan vat voi threat
		int frame_exp_width = exp_threat.get_frame_width();
		int frame_exp_height= exp_threat.get_frame_height();
				std::vector<BulletObject*> bullet_arr = p_player.get_bullet_list_();
				for (int r = 0; r < bullet_arr.size(); ++r)
				{
					BulletObject* p_bullet = bullet_arr.at(r);
					if (p_bullet != NULL)
					{
						// kiem tra tung vien dan voi tung threat(quan dich)
						for (int t = 0; t < Threats_list.size(); ++t)
						{
							ThreatsObject* obj_threat = Threats_list.at(t);
							if (obj_threat != NULL)
							{
								SDL_Rect tRect;

								// lay thong so cua cac obj_othreat;
								tRect.x = obj_threat->GetRect().x;
								tRect.y = obj_threat->GetRect().y;
								tRect.w = obj_threat->get_width_frame();
								tRect.h = obj_threat->get_height_frame();

								SDL_Rect bRect = p_bullet->GetRect(); // vi vien dan khong co thong so gi
								bool bCol = SDLCommonFunc1::CheckCollision(tRect, bRect);
								if (bCol)
								{
									mark_value ++;
									for (int ex1=0; ex1 < NUM_FRAME_EXP; ex1++) // lay ra vi tri cac tam anh
									{
										// vi tri cua vu no
										int x_pos = p_bullet->GetRect().x-frame_exp_width*0.5; // dan cham vao dau thi no tai do
										int y_pos = p_bullet->GetRect().y-frame_exp_height*0.5;
										
										exp_threat.set_frame(ex1);
										exp_threat.SetRect(x_pos, y_pos);
										exp_threat.Show(g_screen);
										SDL_RenderPresent(g_screen);
										Mix_PlayChannel(-1, g_sound_exp[0], 0);
									}

									p_player.RemoveBullet(r);
									obj_threat->Free();
									Threats_list.erase(Threats_list.begin() + t);
									
								}
							}
						}
					}
				}


				// Su ly thoi gian
				std::string str_time = "Time: ";
				Uint32 time_val = SDL_GetTicks()/1000; // thoi diem hien tai
				Uint32 val_time=400-time_val; // thoi gian con lai
				if (val_time<=0)
				{
					 	if (MessageBox(NULL, L"GAMES OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
				        	{
					        	is_quit = true;
								break;
				        	}
				}
				else 
				{
					std::string str_val = std::to_string(val_time);
					str_time+=str_val;
					time_game.SetText(str_time);
					time_game.LoadFromRenderText(font_time, g_screen);
					time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15); // bao gom toa do
				}

				std::string val_str_mark = std :: to_string(mark_value);
				std :: string strMark("Mark: ");
				strMark += val_str_mark;

				mark_game.SetText(strMark);
				mark_game.LoadFromRenderText(font_time, g_screen);
				mark_game.RenderText(g_screen, SCREEN_WIDTH*0.5-50, 15);

				int money_count = p_player.GetMoneyCount();
				std::string money_str = std ::to_string(money_count);
				
				money_game.SetText(money_str);
				money_game.LoadFromRenderText(font_time, g_screen);
				money_game.RenderText(g_screen, SCREEN_WIDTH*0.5-250, 15);

				// Show Boss
				
				int x_val = MAX_MAP_X*TILE_SIZE - (map_data.stat_x_+p_player.GetRect().x);
				
			    if (x_val <= SCREEN_WIDTH-200)
				{
				bossObject.SetMapXY(map_data.stat_x_, map_data.stat_y_);
				bossObject.DoPlayer(map_data);
				bossObject.MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
				bossObject.Show(g_screen);
			
				}

			
				SDL_Rect Boss = bossObject.GetRectFrame();

				int frame_boss_width = bossObject.get_width_frame();
				int frame_boss_height = bossObject.get_height_frame();
					std::vector<BulletObject*> p_player_boss_arr = p_player.get_bullet_list_();
					for (int am=0; am < p_player_boss_arr.size(); am++)
					{
						BulletObject* p_player_boss = p_player_boss_arr.at(am);
						if (p_player_boss!=NULL)
						{
							SDL_Rect cRect = p_player_boss->GetRect();
							bool cCol = SDLCommonFunc1 :: CheckCollision (Boss, cRect);
							if (cCol)

							{      
								   
								     boss_am++;
									 if (boss_am>=5)
									 {
						           	   bossObject.Free();
									   
								    	for (int ex2=0; ex2 < 8; ex2++) // lay ra vi tri cac tam anh
									    {
										// vi tri cua vu no
									

										int x_pos1 = p_player_boss->GetRect().x;//+frame_exp_width*0.5; // dan cham vao dau thi no tai do
										int y_pos1 = p_player_boss->GetRect().y;//+frame_exp_height*0.5;
										

										img_boss_no.set_frame(ex2);
										img_boss_no.SetRect(x_pos1, y_pos1);
										img_boss_no.Show(g_screen); 
										SDL_RenderPresent(g_screen);
										Mix_PlayChannel(-1, g_sound_exp[0], 0);

									    }
										
									   
									    if (MessageBox(NULL, L"WIN", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
				        	              {
										 bossObject.RemoveBullet(am);
					                 	 close();
						                 SDL_Quit();
						                 return 0;
					
									     }
										
								      }
									 p_player.RemoveBullet(am);
									  bossObject.RemoveBullet(am);
							}

						}

					}
					


				SDL_RenderPresent(g_screen);

				int real_imp_time = fps_timer.get_ticks();
				int time_one_frame = 1000 / FRAME_PER_SECOND;
				if (real_imp_time < time_one_frame)
				{
					int delay_time = time_one_frame - real_imp_time;
					if (delay_time)
					{
						SDL_Delay(delay_time);
					}
				}
	    }
			for (int i = 0; i < Threats_list.size(); i++)
			{
				ThreatsObject* p_threat = Threats_list.at(i);
				if (p_threat != NULL)
				{
					p_threat->Free();
					p_threat = NULL;
				}

			}
			Threats_list.clear();
			
			close();
			return 0;
    }