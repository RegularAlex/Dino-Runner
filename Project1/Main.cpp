#include <SDL.h>
#include <SDL_ttf.h> 
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include "Player.h"
#include "Obstacle.h"

using namespace std;

//Variables
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

//Function Definitions
void main_menu();
void PlayGame(SDL_Renderer* render, SDL_Window* window);
void text(SDL_Color Colour, SDL_Renderer* text_render, int x, int y, string text);
bool checkCollision(SDL_Rect a, SDL_Rect b);

int main(int argc, char* argv[])
{
	main_menu();
	return 0;
}

void main_menu()
{
	//Initalisation of SDL and SDL_TTF
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	IMG_Init;

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
	Start.x = 550;
	Start.y = 600;
	Start.w = 200;
	Start.h = 100;

	//Putting the Start Button on Screen 
	SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
	SDL_RenderFillRect(render, &Start);
	SDL_RenderPresent(render);

	//Colours
	SDL_Color Black{ 0,0,0 };
	SDL_Color Gold{212,175,55};

	//Text Creation
	text(Black, render, 575, 10, "Dino Runner");
	text(Black, render, 615, 625, "START");
	text(Gold, render, 400, 300, "HIGH SCORE:");

	//Accessing external text file to display text
	string highscore; 
	int displayhighscore = 0; 
	ifstream displayscore("HighScore.txt");
	while (getline(displayscore,highscore)) //Going through the file line by line
	{
		int namehere = stoi(highscore); //Convert string from textfile into a int.
		if (namehere > displayhighscore) //If the new value is higher than the stored value
		{
			displayhighscore = namehere; //Make the new value the high score
		}
	}
	displayscore.close(); //Close the access to the external file
	//Display the high score value on screen
	stringstream highscoretext; 
	highscoretext << displayhighscore; 
	text(Gold, render, 625, 300, highscoretext.str().c_str());

	if (played_game == true)
	{
		//Code to display recently achieved score here
		text(Black, render, 400, 400, "SCORE:");
		stringstream endscore;
		endscore << score;
		text(Black, render, 500, 400, endscore.str().c_str());
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
				SDL_GetMouseState(&MousePosition.x, &MousePosition.y);

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
	//Obstacles being active/inactive
	bool obstacle1_active = false;
	bool obstacle2_active = false;
	bool obstacle3_active = false;
	bool obstacle4_active = false;
	//Obstacles being on/off the screen. 
	bool obstacle1_clear = true;
	bool obstacle2_clear = true;
	bool obstacle3_clear = true;
	bool obstacle4_clear = true;
	//Limit Jump Height 
	int limitjump = 475;
	bool jump = false;
	//Limit Duck Height
	int limitduck = 625;
	bool duck = false;
	//Player Phase
	int phase = 1; 
	int speedchange = 2100;

	//Images
	//Background 
	SDL_Surface* backgrounds = IMG_Load("Background.png");
	SDL_Texture* backgroundt = SDL_CreateTextureFromSurface(render, backgrounds);
	//Background2
	SDL_Surface* background2s = IMG_Load("Background2.png");
	SDL_Texture* background2t = SDL_CreateTextureFromSurface(render, background2s);

	//Player
	SDL_Surface* dinoruns = IMG_Load("DinoRunning1.1.png");
	SDL_Texture* dinorunt = SDL_CreateTextureFromSurface(render,dinoruns);

	SDL_Surface* dinoruns2 = IMG_Load("DinoRunning2.1.png");
	SDL_Texture* dinorunt2 = SDL_CreateTextureFromSurface(render, dinoruns2);

	//Obstacles 
	SDL_Surface* groundobss = IMG_Load("cactus1.1.png"); //Ground
	SDL_Texture* groundobst = SDL_CreateTextureFromSurface(render, groundobss);

	SDL_Surface* airobss = IMG_Load("bunchbirds1.1.png"); //Flying
	SDL_Texture* airobst = SDL_CreateTextureFromSurface(render, airobss);

	SDL_Surface* groundobss2 = IMG_Load("cactus2.1.png"); //Ground
	SDL_Texture* groundobst2 = SDL_CreateTextureFromSurface(render, groundobss2);

	SDL_Surface* airobss2 = IMG_Load("bunchbirds2.1.png"); //Flying
	SDL_Texture* airobst2 = SDL_CreateTextureFromSurface(render, airobss2);

	//Creation of Background Rectangle so an image can be assigned to it. 
	SDL_Rect Background; 
	Background.x = 0; 
	Background.y = 0; 
	Background.w = Screen_Width;
	Background.h = Screen_Height;
	SDL_RenderCopy(render, backgroundt, NULL, &Background);
	SDL_FreeSurface(backgrounds);

	//Creation of Second Background Rectangle. 
	SDL_Rect Background2; 
	Background2.x = 1249;
	Background2.y = 0; 
	Background2.w = Screen_Width;
	Background2.h = Screen_Height;
	SDL_RenderCopy(render, background2t, NULL, &Background2);
	SDL_FreeSurface(background2s);

	//Creation of Floor Rectangle/Surface(Positioned on screen) 
	SDL_Rect Floor;
	Floor.x = 0;
	Floor.y = 600;
	Floor.w = 1250;
	Floor.h = 200;

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
	int type = 1; //Type for the obstacle (There is two of each obstacle type, but they all have different images). 
	obstacle ground;
	SDL_Rect obs;
	obs.x = 1700; //When altering score this value needs to be altered to (e.g., 1700 (being the initial obstacle spawn x) and 500 (being the initial score number for obstacles to spawn) / 1800 (being the initial obstacle spawn x) and 600 (being the initial score number for obstacles to spawn) ) by the same amount which the score is changed by.
	obs.y = ground.obstacle_y(type);
	obs.w = ground.obstacle_width(type);
	obs.h = ground.obstacle_height(type);
	//Assign Image to Obstacle
	SDL_RenderCopy(render, groundobst, NULL, &obs);
	SDL_FreeSurface(groundobss);

	//Obstacle 2
	type = 2;
	obstacle air;
	SDL_Rect obs2;
	obs2.x = obs.x + 600;
	obs2.y = air.obstacle_y(type);
	obs2.w = air.obstacle_width(type);
	obs2.h = air.obstacle_height(type);
	//Assign Image to Obstacle
	SDL_RenderCopy(render, airobst, NULL, &obs2);
	SDL_FreeSurface(airobss);

	//Obstacle 3
	type = 1;
	obstacle ground1;
	SDL_Rect obs3;
	obs3.x = obs2.x + 600;
	obs3.y = ground1.obstacle_y(type);
	obs3.w = ground1.obstacle_width(type);
	obs3.h = ground1.obstacle_height(type);
	SDL_RenderCopy(render, groundobst2, NULL, &obs3);
	SDL_FreeSurface(groundobss2);

	//Obstacle 4 
	type = 2;
	obstacle air1;
	SDL_Rect obs4;
	obs4.x = obs3.x + 600;
	obs4.y = air1.obstacle_y(type);
	obs4.w = air1.obstacle_width(type);
	obs4.h = air1.obstacle_height(type);
	SDL_RenderCopy(render, airobst2, NULL, &obs4);
	SDL_FreeSurface(airobss2);

	int last_x = 2700; //When altering score this value needs to be altered to (e.g., 2700 (for the last x value) and 500 (initial score needed for obstacles to spawn) / 2800 and 600) by the same amount.

	SDL_SetRenderDrawColor(render, 250, 213, 165, 255);
	SDL_RenderFillRect(render, &Floor);
	SDL_SetRenderDrawColor(render, 250, 213, 165, 255);
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
			Background.x -= Player_xvel;
			Background2.x -= Player_xvel;
		}

		//Allowing for the floor to be endless, as when one floor ends it is brought to the other side. 
		if (Floor.x < -1249)
		{
			Floor.x = 1249;
		}
		if (Floor2.x < -1249)
		{
			Floor2.x = 1249;
		}
		//Allow for the background to be on loop as well, when one background ends it is brought to the other side. 
		if (Background.x < -1249)
		{
			Background.x = 1249;
		}
		if (Background2.x < -1249)
		{
			Background2.x = 1249;
		}

		//Using Score to Start Spawning Obstacles, allowing the player character to get into the centre of the screen
		if ((score >= 500) && (obstacle1_clear == true)) //600 (previous value)
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
			}
		}

		if ((score >= 1100) && (obstacle2_clear == true)) //1200 (previous value)
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
			}
		}

		if ((score >= 1700) && (obstacle3_clear == true)) //1800 (previous value)
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
			}
		}

		if ((score >= 2000) && (obstacle4_clear == true)) //2100 (previous value)
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
			}
		}

		//First Obstacle 
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
		}

		//Second Obstacle
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
		}

		//Third Obstacle
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
		}

		//Fourth Obstacle
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
		}

		//Player Character Image being updated between the two images (makes the character look like they are running). 
		if (phase == 1)
		{
			//Having to redraw everything. 
			SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
			SDL_RenderClear(render);
			SDL_RenderPresent(render);
			//Background
			SDL_RenderCopy(render, backgroundt, NULL, &Background);
			SDL_RenderCopy(render, background2t, NULL, &Background2);
			//Floor
			SDL_SetRenderDrawColor(render, 250, 213, 165, 255);
			SDL_RenderFillRect(render, &Floor);
			SDL_SetRenderDrawColor(render, 250, 213, 165, 255);
			SDL_RenderFillRect(render, &Floor2);
			//Player
			SDL_RenderCopy(render, dinorunt, NULL, &Player);

			SDL_RenderPresent(render);
			phase += 1;
			//Obstacles 
			if (obstacle1_active == true)
			{
				SDL_RenderCopy(render, groundobst, NULL, &obs);
				SDL_RenderPresent(render);
			}
			if (obstacle2_active == true)
			{
				SDL_RenderCopy(render, airobst, NULL, &obs2);
				SDL_RenderPresent(render);
			}
			if (obstacle3_active == true)
			{
				SDL_RenderCopy(render, groundobst2, NULL, &obs3);
				SDL_RenderPresent(render);
			}
			if (obstacle4_active == true)
			{
				SDL_RenderCopy(render, airobst2, NULL, &obs4);
				SDL_RenderPresent(render);
			}
			//Score Text
			text(Black, render, 0, 0, "Score:");
			//Score Value
			stringstream updatedscore;
			updatedscore << score;
			text(Black, render, 100, 0, updatedscore.str().c_str());

			SDL_RenderPresent(render);
			SDL_RenderClear(render);
			
		}
		else if (phase == 2)
		{
			SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
			SDL_RenderClear(render);
			SDL_RenderPresent(render);
			SDL_RenderCopy(render, backgroundt, NULL, &Background);
			SDL_RenderCopy(render, background2t, NULL, &Background2);
			SDL_SetRenderDrawColor(render, 250, 213, 165, 255);
			SDL_RenderFillRect(render, &Floor);
			SDL_SetRenderDrawColor(render, 250, 213, 165, 255);
			SDL_RenderFillRect(render, &Floor2);
			SDL_RenderCopy(render, dinorunt2, NULL, &Player);
			SDL_RenderPresent(render);
			phase -= 1;
			if (obstacle1_active == true)
			{
				SDL_RenderCopy(render, groundobst, NULL, &obs);
				SDL_RenderPresent(render);
			}
			if (obstacle2_active == true)
			{
				SDL_RenderCopy(render, airobst, NULL, &obs2);
				SDL_RenderPresent(render);
			}
			if (obstacle3_active == true)
			{
				SDL_RenderCopy(render, groundobst2, NULL, &obs3);
				SDL_RenderPresent(render);
			}
			if (obstacle4_active == true)
			{
				SDL_RenderCopy(render, airobst2, NULL, &obs4);
				SDL_RenderPresent(render);
			}
			text(Black, render, 0, 0, "Score:");
			stringstream updatedscore;
			updatedscore << score;
			text(Black, render, 100, 0, updatedscore.str().c_str());
			SDL_RenderPresent(render);
			SDL_RenderClear(render);
		}

		//Making sure that speed is limited to 1 (most likely to change)
		if (Player_xvel >= 1) //Basic but works, may change this so it uses time
		{
			Player_xvel = 1; //Keep x velocity at 1
			//Below Code of Changing speed does work, but need to change the == to =. As it causes a problem with the obstacles.
			if (score > 1400)
			{
				Player_xvel = 2;
			}
			if (score > 2100)
			{
				Player_xvel = 3;
			}
			if (score > speedchange + 700) //For continous speed increases. 
			{
				//Player_xvel += 1; //Increments player speed by 1
				Player_xvel = Player_xvel + 1; 
				speedchange = speedchange + 700; //Increaes the boundary by which the score needs to pass for the speed to increase
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
			SDL_RenderCopy(render, backgroundt, NULL, &Background);
			SDL_RenderCopy(render, background2t, NULL, &Background2);
			SDL_SetRenderDrawColor(render, 250, 213, 165, 255);
			SDL_RenderFillRect(render, &Floor);
			SDL_SetRenderDrawColor(render, 250, 213, 165, 255);
			SDL_RenderFillRect(render, &Floor2);
			SDL_RenderPresent(render);

			if (obstacle1_active == true) //seems to be false
			{
				SDL_RenderCopy(render, groundobst, NULL, &obs);
				SDL_RenderPresent(render);
			}
			if (obstacle2_active == true)
			{
				SDL_RenderCopy(render, airobst, NULL, &obs2);
				SDL_RenderPresent(render);
			}
			if (obstacle3_active == true)
			{
				SDL_RenderCopy(render, groundobst2, NULL, &obs3);
				SDL_RenderPresent(render);
			}
			if (obstacle4_active == true)
			{
				SDL_RenderCopy(render, airobst2, NULL, &obs4);
				SDL_RenderPresent(render);
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
		if ((collision1 == true) || (collision2 == true) || (collision3 == true) || (collision4 == true)) //If any collision is true
		{
			//Game Over condition (when the player makes contact with ANY obstacle)
			played_game = true;
			int count = 1;
			while (count != 0) //Needed or the user is stuck in a main menu cycle which the score being updated. 
			{
				//Save score to external text file. 
				ofstream accessfile("HighScore.txt", ios::app);
				accessfile << endl; 
				accessfile << score;
				accessfile.close();

				//Destroys the current screen which the game is being played on.
				SDL_DestroyWindow(window);
				TTF_Quit();
				SDL_Quit();
				active = false; 
				//Creates a new one with the main menu being presented with the players score being presented. 
				main_menu();
				//Reduces count by 1, so the loop doesnt occur again.
				count = count - 1;
				break;
			}
		}

		while (SDL_PollEvent(&event))
		{
			//Allows the game to be exited 
			if (event.type == SDL_QUIT)
			{
				active = false;
				break;
			}

			//Moving up and down / jumping and ducking 
			else if (event.type == SDL_KEYDOWN) //Checking for the key being pressed down
			{

				switch (event.key.keysym.sym)
				{
				case SDLK_UP: //UP Arrow Key
					//up/jump action 
					Player_yvel -= 100;
					if (Player.y < limitjump) //Limits the players jump height to the value specified in the variable of limitjump
					{
						Player_yvel = 0;
						jump = true;
					}
					break;

				case SDLK_DOWN: //DOWN Arrow Key
					//down/duck action
					Player_yvel += 100;
					if (Player.y > limitduck) //Same as above but for ducking to thr value specified in limitduck
					{
						Player_yvel = 0;
						duck = true;
					}
					break;

				}
				//Update Player X and Y position by the corrosponding X and Y velocities. 
				Player.y += Player_yvel;
				Player.x += Player_xvel;
				//Re-Drawing everything again here, for jumps and ducking
				SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
				SDL_RenderClear(render);
				SDL_RenderPresent(render);
				SDL_RenderCopy(render, backgroundt, NULL, &Background);
				SDL_RenderCopy(render, background2t, NULL, &Background2);
				SDL_SetRenderDrawColor(render, 250, 213, 165, 255);
				SDL_RenderFillRect(render, &Floor);
				SDL_SetRenderDrawColor(render, 250, 213, 165, 255);
				SDL_RenderFillRect(render, &Floor2);
				SDL_RenderPresent(render);
				if (obstacle1_active == true)
				{
					SDL_RenderCopy(render, groundobst, NULL, &obs);
					SDL_RenderPresent(render);
				}
				if (obstacle2_active == true)
				{
					SDL_RenderCopy(render, airobst, NULL, &obs2);
					SDL_RenderPresent(render);
				}
				if (obstacle3_active == true)
				{
					SDL_RenderCopy(render, groundobst2, NULL, &obs3);
					SDL_RenderPresent(render);
				}
				if (obstacle4_active == true)
				{
					SDL_RenderCopy(render, airobst2, NULL, &obs4);
					SDL_RenderPresent(render);
				}
				text(Black, render, 0, 0, "Score:");
				stringstream updatedscore;
				updatedscore << score;
				text(Black, render, 100, 0, updatedscore.str().c_str());
				SDL_RenderPresent(render);
				SDL_RenderClear(render);

			}

			else if (event.type == SDL_KEYUP)//Checking for the key being released
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_UP: //UP Arrow Key
					//up/jump action; 
					if (jump == false)
					{
						Player_yvel += 100;
					}
					Player.y = 550; //Brings Player Back Down to the floor
					jump = false;
					break;

				case SDLK_DOWN: //DOWN Arrow Key
					//down/duck action
					if (duck == false)
					{
						Player_yvel -= 100;
					}
					Player.y = 550; //Brings Player Back Down to the floor
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


//Text Creation Function 
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
