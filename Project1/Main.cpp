#include <SDL.h>
#include <SDL_ttf.h> 
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include<fstream>
#include "Player.h"
#include "Obstacle.h"

using namespace std; 

int score = 0; 
double Player_xvel = 0; 
double Player_yvel = 0;
int player_x = 100;
int player_y = 550;
bool played_game = false; 
int Screen_Height = 750; 
int Screen_Width = 1250;
//Game Related
bool obstacle1_active = false;
bool obstacle2_active = false;
bool obstacle1_clear = true; 
bool obstacle2_clear = true;

//Obstacle Variables 



void main_menu();
void PlayGame(SDL_Renderer* render, SDL_Window* window);
string highscore();
void text(SDL_Color Colour, SDL_Renderer* text_render, int x, int y, string text);
bool checkCollision(SDL_Rect a, SDL_Rect b);
void compare(int score);

int main(int argc, char* argv[])
{
	ofstream MyFile("HighScore.txt");
	MyFile.close();
	main_menu();
	return 0;
}

void main_menu()
{
	//Initalisation of SDL and SDL_TTF
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	//Creation of a Window 
	SDL_Window* window;
	window = SDL_CreateWindow("Dino Runner", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Screen_Width, Screen_Height, SDL_WINDOW_SHOWN);

	//Creation of Renderer 
	SDL_Renderer* render = NULL; 
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//Draw Background Colour
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
	SDL_RenderClear(render);

	//Creation of Start Button
	SDL_Rect Start; 
	Start.x = 550 ; 
	Start.y = 600 ; 
	Start.w = 200 ; 
	Start.h = 100 ;

	//Putting the Start Button on Screen 
	SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
	SDL_RenderFillRect(render, &Start);
	SDL_RenderPresent(render);

	//Colours
	SDL_Color Black{ 0,0,0 };

	//Text Creation
	text(Black, render, 575, 10, "Dino Runner");
	text(Black, render, 615, 625, "START");
	text(Black, render, 400, 300, "HIGH SCORE:");
	//Need to access external text file and access the high score and then display it next to the above text
	string high_score = highscore();
	stringstream hs; 
	hs << high_score;
	text(Black, render, 500, 300, hs.str().c_str());

	if (played_game == true)
	{
		text(Black, render, 400, 400, "SCORE:");
		stringstream endscore;
		endscore << score;
		text(Black, render, 500, 400, endscore.str().c_str());
		//Code to display recently achieved score here
	}

	//Text Display 
	SDL_RenderPresent(render);
	SDL_RenderClear(render);

	//Main Game Loop 
	SDL_Event event; 
	bool active = true; 
	while (active)
	{
		while (SDL_PollEvent(&event))
		{
			//Allows the game to be exited 
			if (event.type == SDL_QUIT)
			{
				active = false; 
				break;
			}

			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				SDL_Point MousePosition = { 0,0 };
				SDL_GetMouseState(& MousePosition.x, & MousePosition.y);

				if (SDL_PointInRect(&MousePosition, &Start))
				{
					SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
					SDL_RenderClear(render);
					SDL_RenderPresent(render);
					PlayGame(render, window);
				}
			}
		}
	}

	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();

}

void PlayGame(SDL_Renderer* render, SDL_Window* window)
{
	int obstacle_count = 0;
	score = 0; 
	double Player_xvel = 0;
	double Player_yvel = 0;
	int player_x = 100;
	int player_y = 550;
	bool obstacle1_active = false;
	bool obstacle2_active = false;
	bool obstacle3_active = false;
	bool obstacle4_active = false;
	bool obstacle1_clear = true;
	bool obstacle2_clear = true;
	bool obstacle3_clear = true;
	bool obstacle4_clear = true;
	//Limit Jump Height 
	int limitjump = 475 ; 
	bool jump = false;
	//Limit Duck Height
	int limitduck = 625 ;
	bool duck = false;

	//Creation of Floor Rectangle/Surface (Positioned on screen)
	SDL_Rect Floor; 
	Floor.x = 0 ; 
	Floor.y = 600 ; 
	Floor.w = 1250 ; 
	Floor.h = 200 ;
	//Creation of Second Floor Surface (Positioned off screen, ready to move on screen)
	SDL_Rect Floor2;
	Floor2.x = 1249;
	Floor2.y = 600;
	Floor2.w = 1250;
	Floor2.h = 200;
	//Player
	SDL_Rect Player; 
	Player.x = player_x; 
	Player.y = player_y; 
	Player.w = 50; 
	Player.h = 50;
	//Obstacle 1
	int type = 1; //Need to improve this, so a random value between 1 and 2 is called
	obstacle ground; 
	SDL_Rect obs; 
	obs.x = 1800;
	obs.y = ground.obstacle_y(type);
	obs.w = ground.obstacle_width(type);
	obs.h = ground.obstacle_height(type);

	//Obstacle 2
	type = 2;
	obstacle air; 
	SDL_Rect obs2; 
	obs2.x = obs.x + 600;
	obs2.y = air.obstacle_y(type);
	obs2.w = air.obstacle_width(type);
	obs2.h = air.obstacle_height(type);

	//Obstacle 3
	type = 1; 
	obstacle ground1; 
	SDL_Rect obs3; 
	obs3.x = obs2.x + 600; 
	obs3.y = ground1.obstacle_y(type);
	obs3.w = ground1.obstacle_width(type);
	obs3.h = ground1.obstacle_height(type);

	//Obstacle 4 
	type = 2; 
	obstacle air1; 
	SDL_Rect obs4; 
	obs4.x = obs3.x + 600; 
	obs4.y = air1.obstacle_y(type);
	obs4.w = air1.obstacle_width(type);
	obs4.h = air1.obstacle_height(type);

	int last_x = 2800; 

	SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
	SDL_RenderFillRect(render,&Floor);
	SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
	SDL_RenderFillRect(render, &Floor2);
	SDL_SetRenderDrawColor(render, 255, 255, 0, 255);
	SDL_RenderFillRect(render, &Player);
	SDL_RenderPresent(render);
	
	SDL_Color Black{ 0,0,0 };
	text(Black, render, 0, 0, "Score:");
	stringstream updatedscore;
	updatedscore << score;
	text(Black, render, 100, 0, updatedscore.str().c_str());

	SDL_RenderPresent(render);
	SDL_RenderClear(render);

	Player_xvel += 1;
	SDL_Event event;
	bool active = true;
	while (active)
	{
		//Making sure that the player doesnt get to close to the right hand side of the screen
		if (Player.x > Screen_Width - 650)
		{
			Player.x -= Player_xvel; //Player Stops Moving but the ground doesnt, so it gives the impression that the character is moving
			Floor.x -= Player_xvel; 
			Floor2.x -= Player_xvel;
			cout << "Point Reached";
		}

		//if ((Player.y > limitjump) && (jump == true)) 
		//{
		//	Player.y = limitjump;
		//}
		//if ((Player.y < limitduck) && (duck == true)) 
		//{
		//	Player.y = limitduck;
		//}
		//Move the floor rect which isnt on screen to the left of the screen 
		if (Floor.x < -1249)
		{
			Floor.x = 1249;
		}
		if (Floor2.x < -1249)
		{
			Floor2.x = 1249;
		}

		//Using Score to Start Spawning Obstacles, allowing the player character to get into the centre of the screen
		if ((score >= 600) && (obstacle1_clear == true))
		{
			if (obstacle_count <= 4)
			{
				//Start Spawning Ground Obstacles 
				obstacle1_active = true;
				obstacle_count += 1;
				obstacle1_clear = false; 
				type = 1;
				SDL_Rect obs;
				obs.x = ground.obstacle_x(last_x); 
				obs.y = ground.obstacle_y(type);
				obs.w = ground.obstacle_width(type);
				obs.h = ground.obstacle_height(type);
				//last_x = obs.x;
				cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << obs.x;
			}
		}

		if ((score >= 1200) && (obstacle2_clear == true))
		{
			if (obstacle_count <= 4)
			{
				//Start Spawning Flying Obstacles 
				obstacle2_active = true;
				obstacle_count += 1;
				obstacle2_clear = false; 
				type = 2;
				SDL_Rect obs2;
				obs2.x = air.obstacle_x(last_x);
				obs2.y = air.obstacle_y(type);
				obs2.w = air.obstacle_width(type);
				obs2.h = air.obstacle_height(type);
				//last_x = obs2.x;
				cout << "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB" << obs2.x;
			}
		}

		if ((score >= 1800) && (obstacle3_clear == true))
		{
			if (obstacle_count <= 4)
			{
				//Start Spawning Flying Obstacles 
				obstacle3_active = true;
				obstacle_count += 1;
				obstacle3_clear = false;
				type = 1;
				SDL_Rect obs3;
				obs3.x = ground1.obstacle_x(last_x);
				obs3.y = ground1.obstacle_y(type);
				obs3.w = ground1.obstacle_width(type);
				obs3.h = ground1.obstacle_height(type);
				cout << "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" << obs3.x;
			}
		}

		if ((score >= 2100) && (obstacle4_clear == true))
		{
			if (obstacle_count <= 4)
			{
				//Start Spawning Flying Obstacles 
				obstacle4_active = true;
				obstacle_count += 1;
				obstacle4_clear = false;
				type = 2;
				SDL_Rect obs4;
				obs4.x = air1.obstacle_x(last_x);
				obs4.y = air1.obstacle_y(type);
				obs4.w = air1.obstacle_width(type);
				obs4.h = air1.obstacle_height(type);
				cout << "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" << obs4.x;
			}
		}

		if (obs.x > -1) //Should move the rect/obstacle whilsts its x position is greater than -1 
		{
			obs.x -= Player_xvel;
		}
		else if (obs.x = -1) //If the rect which is obs has moved off screen then. 
		{
			obstacle1_clear = true; 
			obstacle_count -= 1;
			obstacle1_active = false; 
			obs.x = ground.obstacle_x(last_x);
			last_x = last_x + 150;
			//cout << "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC";
		}
		if (obs2.x > -1)
		{
			obs2.x -= Player_xvel;
		}
		else if (obs2.x = -1)
		{
			obstacle2_clear = true;
			obstacle_count -= 1;
			obstacle2_active = false;
			obs2.x = air.obstacle_x(last_x);
			last_x = last_x + 150;
			//cout << "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";
		}

		if (obs3.x > -1)
		{
			obs3.x -= Player_xvel;
		}
		else if (obs3.x = -1)
		{
			obstacle3_clear = true;
			obstacle_count -= 1;
			obstacle3_active = false;
			obs3.x = ground1.obstacle_x(last_x);
			last_x = last_x + 150;
			//cout << "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ";
		}

		if (obs4.x > -1)
		{
			obs4.x -= Player_xvel;
		}
		else if (obs4.x = -1)
		{
			obstacle4_clear = true;
			obstacle_count -= 1;
			obstacle4_active = false;
			obs4.x = air1.obstacle_x(last_x);
			last_x = last_x + 150;
			//cout << "ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ";
		}

		//Making sure that speed is limited to 1 (most likely to change)
		if (Player_xvel >= 1) //Basic but works, may change this so it uses time
		{
			Player_xvel == 1; //Keep x velocity at 1 
			if (score > 700)
			{
				Player_xvel == 2; 
			}
			if (score > 1400)
			{
				Player_xvel == 3; 
			}
			int count = 1000; 
			for (int i = 0; i < count; i++) 
			{
				cout << "i num" << i << endl;//Keep this, makes the movement slow...
			}
			Player.x += Player_xvel; //Move Player along
			score += 1;//Award Points for movement
			cout << "X Position" << Player.x << endl;
			//Should move the character along every x ticks. 
			//Allows for everything to be redrawn 
			SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
			SDL_RenderClear(render);
			SDL_RenderPresent(render);
			SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
			SDL_RenderFillRect(render, &Floor);
			SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
			SDL_RenderFillRect(render, &Floor2);
			SDL_SetRenderDrawColor(render, 255, 255, 0, 255);
			SDL_RenderFillRect(render, &Player);
			if (obstacle1_active == true) //seems to be false
			{
				SDL_SetRenderDrawColor(render, 111, 255, 111, 255);
				SDL_RenderFillRect(render, &obs);
				cout << "111111111111111111111111111111111111111111111111111111" << endl;
			}
			if (obstacle2_active == true)
			{
				SDL_SetRenderDrawColor(render, 111, 255, 111, 255);
				SDL_RenderFillRect(render, &obs2);
				cout << "222222222222222222222222222222222222222222222222222222" << endl;
			}
			if (obstacle3_active == true)
			{
				SDL_SetRenderDrawColor(render, 111, 255, 111, 255);
				SDL_RenderFillRect(render, &obs3);
				cout << "555555555555555555555555555555555555555555555555555555" << endl;
			}
			if (obstacle4_active == true)
			{
				SDL_SetRenderDrawColor(render, 111, 255, 111, 255);
				SDL_RenderFillRect(render, &obs4);
				cout << "666666666666666666666666666666666666666666666666666666" << endl;
			}
			SDL_RenderPresent(render);
			text(Black, render, 0, 0, "Score:");
			stringstream updatedscore; 
			updatedscore << score; 
			text(Black, render, 100, 0, updatedscore.str().c_str());
			SDL_RenderPresent(render);
			SDL_RenderClear(render);
		}

		//Collision Dectection
		bool collision1 = checkCollision(Player, obs); //Obstacle 1
		bool collision2 = checkCollision(Player, obs2); //Obstacle 2
		bool collision3 = checkCollision(Player, obs3); //Obstacle 3
		bool collision4 = checkCollision(Player, obs4); //Obstacle 4
		if ((collision1 == true) || (collision2 == true) || (collision3 == true) || (collision4 == true))
		{
			//cout << "GAME OVER" << endl; 
			played_game = true; 
			main_menu();
			//break;
			SDL_DestroyWindow(window);
			TTF_Quit();
			SDL_Quit();
		}


		while (SDL_PollEvent(&event))
		{
			//Allows the game to be exited 
			if (event.type == SDL_QUIT)
			{
				active = false;
				break;
			}


			//Moving up and down (for the time being), this will become jumping and ducking in the future
			else if (event.type == SDL_KEYDOWN)
			{
				
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					//cout << "up/jump" << endl; 
					Player_yvel -= 100;
					//cout << Player_yvel << endl;
					//cout << Player.y;
					//jump = true;
					if (Player.y < limitjump)
					{
						Player_yvel = 0;
						jump = true;
					}

					break;

				case SDLK_DOWN:
					//cout << "down/duck" << endl;
					Player_yvel += 100;
					//cout << Player.y;
					if (Player.y > limitduck)
					{
						Player_yvel = 0; 
						duck = true;
					}
					break;

				}
				Player.y += Player_yvel;
				Player.x += Player_xvel;
				//Re-Drawing everything again here, for jumps and ducking
				SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
				SDL_RenderClear(render);
				SDL_RenderPresent(render);
				SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
				SDL_RenderFillRect(render, &Floor);
				SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
				SDL_RenderFillRect(render, &Floor2);
				SDL_SetRenderDrawColor(render, 255, 255, 0, 255);
				SDL_RenderFillRect(render, &Player);
				SDL_RenderPresent(render);
				if (obstacle1_active == true)
				{
					SDL_SetRenderDrawColor(render, 111, 255, 111, 255);
					SDL_RenderFillRect(render, &obs);
					cout << "333333333333333333333333333333333333333333333333333333" << endl;
				}
				if (obstacle2_active == true)
				{
					SDL_SetRenderDrawColor(render, 111, 255, 111, 255);
					SDL_RenderFillRect(render, &obs2);
					cout << "44444444444444444444444444444444444444444444444444444" << endl;
				}
				if (obstacle3_active == true)
				{
					SDL_SetRenderDrawColor(render, 111, 255, 111, 255);
					SDL_RenderFillRect(render, &obs3);
					cout << "77777777777777777777777777777777777777777777777777777" << endl;
				}
				if (obstacle4_active == true)
				{
					SDL_SetRenderDrawColor(render, 111, 255, 111, 255);
					SDL_RenderFillRect(render, &obs4);
					cout << "88888888888888888888888888888888888888888888888888888" << endl;
				}
				text(Black, render, 0, 0, "Score:");
				stringstream updatedscore;
				updatedscore << score;
				text(Black, render, 100, 0, updatedscore.str().c_str());
				SDL_RenderPresent(render);
				SDL_RenderClear(render);
				
			}

			else if (event.type == SDL_KEYUP)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					//cout << "up/jump" << endl; 
					if (jump == false)
					{
						Player_yvel += 100;
					}
					//cout << Player_yvel << endl; 
					//cout << "PLayer Y" << Player.y << endl;
					Player.y = 550; //Brings Player Back Down
					jump = false;
					break;

				case SDLK_DOWN:
					//cout << "down/duck" << endl;
					if (duck == false)
					{
						Player_yvel -= 100;
					}
					//cout << Player.y << endl;
					Player.y = 550; //Brings Player Back Down 
					duck = false; 
					break;
				}
			}
			if (Player.x > Screen_Width - 650)
			{
				Player.x = Screen_Width - 650;
			}

		}

	}

	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();

}


//Text Creation Function (Borrowed from my C++ Coursework)
void text(SDL_Color Colour, SDL_Renderer* text_render, int x, int y, string text)
{
	//Creation of Font
	TTF_Font* font = TTF_OpenFont("../Project1/Resources/Arial/arial.ttf", 25);
	//Location of Font
	SDL_Rect fontrect = { x,y };
	stringstream text_creation; 
	text_creation << text;
	SDL_Surface* surface = TTF_RenderText_Blended(font, text_creation.str().c_str(), Colour);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(text_render, surface);
	SDL_QueryTexture(texture, NULL, NULL, &fontrect.w, &fontrect.h);
	SDL_RenderCopy(text_render, texture, NULL, &fontrect);
}

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
	//Sides of the rectangles 
	int leftA, leftB; 
	int rightA, rightB; 
	int topA, topB; 
	int bottomA, bottomB;

	//Calculating the sides of Rectangle A (Which will most likely be the player)
	leftA = a.x; 
	rightA = a.x + a.w; 
	topA = a.y; 
	bottomA = a.y + a.h; 

	//Calculating the sides of Rectangle B (Which will most likely be the obstacle)
	leftB = b.x; 
	rightB = b.x + b.w; 
	topB = b.y; 
	bottomB = b.y + b.h;

	//Checking to see if there has been a collision. 
	//If the two rects arent inside of each other, so no collision has occured
	if (bottomA <= topB)
	{
		return false;
	}
	if (topA > +bottomB)
	{
		return false; 
	}
	if (rightA <= leftB)
	{
		return false;
	}
	if (leftA >= rightB)
	{
		return false;
	}

	//If there has been a collision 
	return true; 


}

string highscore()
{
	fstream MyFile("HighScore.txt");
	string highscore;
	getline(MyFile, highscore);
	MyFile.close();
	return highscore;
}

void compare(int score)
{
	fstream MyFile("HighScore.txt");
	string highscore;
	getline(MyFile, highscore);
	int hs = stoi(highscore);
	if (score > hs)
	{
		hs = score; 
		MyFile << hs; 

	}



}
