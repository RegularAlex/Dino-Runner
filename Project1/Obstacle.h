#pragma once
#include <SDL.h>

class obstacle
{
public: 
	int obstacle_x(int last_x);
	int obstacle_y(int type);
	int obstacle_width(int type);
	int obstacle_height(int type);
	//void create(int type);
	//void draw(int type, SDL_Renderer* render);
	//void move(); 

};
