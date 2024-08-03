#include "Obstacle.h"
#include <cstdlib>
#include <sstream>
#include <iostream>
using namespace std;


int obstacle::obstacle_x(int last_x)
{
	//X Coordinate
    //Random X Coordinate between something and 1250
	int obstacle_x = 0;
	//srand((unsigned)time(NULL));
	//obstacle_x = 750 + (rand() % 500);
	obstacle_x = (last_x / 1.05); //last_x - 100
	cout << "The obstacle x is" << obstacle_x << endl;
	return obstacle_x;
}

int obstacle::obstacle_y(int type)
{
	//Y Coordinate, Width and Height Sorting Out
	int obstacle_y = 0;
	if (type == 1)
	{
		//Ground Obstacle Code
		//Needs to be on the height of the ground which is 200
		obstacle_y = 550;
		cout << "Obstacle y is" << obstacle_y << endl;
		return obstacle_y;
	}

	else if (type == 2)
	{
		//Flying Obstacle Code
		//Needs to be above the ground, so a greater y value. 
		obstacle_y = 450;
		cout << "Obstacle y is" << obstacle_y << endl;
		return obstacle_y;
	}


}

int obstacle::obstacle_width(int type)
{
	//Y Coordinate, Width and Height Sorting Out
	int obstacle_width = 0;
	if (type == 1)
	{
		//Ground Obstacle Code
		obstacle_width = 25;
		cout << "Obstacle Width is" << obstacle_width << endl;
		return obstacle_width;
	}

	else if (type == 2)
	{
		//Flying Obstacle Code
		obstacle_width = 100;
		cout << "Obstacle Width is" << obstacle_width << endl;
		return obstacle_width;
	}
}

int obstacle::obstacle_height(int type)
{
	//Y Coordinate, Width and Height Sorting Out
	int obstacle_height = 0;
	if (type == 1)
	{
		//Ground Obstacle Code
		obstacle_height = 100;
		cout << "Obstacle Height is" << obstacle_height << endl; 
		return obstacle_height;
	}

	else if (type == 2)
	{
		//Flying Obstacle Code
		obstacle_height = 50;
		cout << "Obstacle Height is" << obstacle_height << endl;
		return obstacle_height;
	}
}

//void obstacle::create(int type)
//{
//	//X Coordinate
//	//Random X Coordinate between something and 1250
//	cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
//	srand((unsigned)time(NULL));
//	obstacle_x = 750 + (rand() % 500);
//
//	//Y Coordinate, Width and Height Sorting Out
//	if (type == 1)
//	{
//		//Ground Obstacle Code
//		//Needs to be on the height of the ground which is 200
//		obstacle_y = 200;
//		obstacle_height = 100;
//		obstacle_width = 25;
//	}
//
//	else if (type == 2)
//	{
//		//Flying Obstacle Code
//		//Needs to be above the ground, so a greater y value. 
//		obstacle_y = 350;
//		obstacle_height = 50;
//		obstacle_width = 100;
//	}
//}
//
//void obstacle::draw(int type, SDL_Renderer* render)
//{
//	cout << "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
//	SDL_Rect Obs;
//	Obs.x = obstacle_x;
//	Obs.y = obstacle_y;
//	Obs.w = obstacle_width;
//	Obs.h = obstacle_height;
//
//	//Drawing it on screen
//	if (type == 1)
//	{
//		//Ground Obstacle
//		SDL_Surface* groundimage = SDL_LoadBMP("cactus.jpg");
//		SDL_Texture* groundtexture = SDL_CreateTextureFromSurface(render, groundimage);
//		SDL_RenderCopy(render, groundtexture, NULL, &Obs);
//		SDL_RenderPresent(render);
//	}
//
//	else if (type == 2)
//	{
//		//Flying Obstacle
//		SDL_Surface* flyingimage = SDL_LoadBMP("bird.jpg");
//		SDL_Texture* flyingtexture = SDL_CreateTextureFromSurface(render, flyingimage);
//		SDL_RenderCopy(render, flyingtexture, NULL, &Obs);
//		SDL_RenderPresent(render);
//	}
//
//}
//
//void obstacle::move()
//{
//	while (obstacle_x != -5)
//	{
//		obstacle_x -= 1; 
//	}
//}