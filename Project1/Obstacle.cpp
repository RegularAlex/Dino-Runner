#include "Obstacle.h"
#include <cstdlib>
#include <sstream>
#include <iostream>
using namespace std;


int obstacle::obstacle_x(int last_x)
{
	//X Coordinate
    //Created from the last used x corrdinate
	int obstacle_x = 0;
	obstacle_x = (last_x / 1.05); //last_x - 100
	return obstacle_x;
}

int obstacle::obstacle_y(int type)
{
	//Y Coordinate creation
	int obstacle_y = 0;
	if (type == 1)
	{
		//Ground Obstacle Code
		//Needs to be on the height of the ground which is 200
		obstacle_y = 550;
		return obstacle_y;
	}

	else if (type == 2)
	{
		//Flying Obstacle Code
		//Needs to be above the ground, so a greater y value. 
		obstacle_y = 450;
		return obstacle_y;
	}


}

int obstacle::obstacle_width(int type)
{
	//Width Creation
	int obstacle_width = 0;
	if (type == 1)
	{
		//Ground Obstacle Code
		obstacle_width = 50;
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
	//Height Creation
	int obstacle_height = 0;
	if (type == 1)
	{
		//Ground Obstacle Code
		obstacle_height = 150;
		return obstacle_height;
	}

	else if (type == 2)
	{
		//Flying Obstacle Code
		obstacle_height = 125;
		return obstacle_height;
	}
}

