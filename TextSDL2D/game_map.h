
#ifndef GAME_MAP__H_
#define GAME_MAP__H_
#include "CommonFunc.h"
#include "BaseObject.h"
#include "stdafx.h"
#define MAX_TILES 20
class TileMat : public BaseObject
{
public:
	TileMat() { ; }
	~TileMat() { ; }
};

class GameMap
{
public:
	GameMap();
	~GameMap();
	void LoadMap(char* name);
	void LoadTiles(SDL_Renderer* screen);
	void DrawMap(SDL_Renderer* creen);
	Map getMap() const { return game_map_; };
	void SetMap(Map& map_data) { game_map_ = map_data; };
private:
	Map game_map_;
	TileMat tile_mat[MAX_TILES];
};
#endif // !GAME_MAP__H_
