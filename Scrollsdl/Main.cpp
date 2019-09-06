
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include "brick.h"
#include "score.h"
#include "enemy.h"
#include "bullet.h"
#include "timer.h"
#include "hero.h"
#include "loaders.h"

//The dimensions of the level
const int LEVEL_WIDTH = 4080;
const int LEVEL_HEIGHT = 3072;

//Dimensions of the tiles
const int TILE_WIDTH = LEVEL_WIDTH / 510;	//Tile width is 8 and there are 510 tiles horizonitically
const int TILE_HEIGHT = LEVEL_HEIGHT / 384; //Tile height is 8 and there are 384 tiles vertically

//The const attributes of the screen
const int SCREEN_WIDTH = TILE_WIDTH * 66;	//There are 66 tiles horizonitically and screen width is 528
const int SCREEN_HEIGHT = TILE_HEIGHT * 48;	//There are 48 tiles vertically and screen height is 384
const int SCREEN_BPP = 32;

//The const attributes of the bricks
const int BRICK_WIDTH = TILE_WIDTH;		//The size of a brick is the same as the size of a tile
const int BRICK_HEIGHT = TILE_HEIGHT;
const int FIRST_BRICK_START_X = TILE_WIDTH * 6;
const int FIRST_BRICK_START_Y = TILE_HEIGHT * 13;

//The const attributes of bullets
const int BULLET_WIDTH = TILE_WIDTH;	//A bullet has the same size of a tile
const int BULLET_HEIGHT = TILE_HEIGHT;
const int BULLET_SPEED = 2 * TILE_WIDTH;	//Bullet will go 2 tiles per frame

//The const attributes of enemies
const int ENEMY_HEIGHT = 4 * TILE_WIDTH;	//The size of an enemy is 4 times size of a tile
const int ENEMY_WIDTH = 4 * TILE_HEIGHT;
const int ENEMY_SPEED = TILE_WIDTH;		//An enemy will go 1 tile per frame

//The const attributes of the hero
const int HERO_WIDTH = 4 * TILE_WIDTH;		//The size of the hero is 4 times of a tile
const int HERO_HEIGHT = 4 * TILE_HEIGHT;
const int HERO_START_X = FIRST_BRICK_START_X;	//Hero starts on the first tile
const int HERO_START_Y = FIRST_BRICK_START_Y - HERO_HEIGHT;
const Hero_direction HERO_START_DIRECTION_X = Right;
const Hero_direction HERO_START_DIRECTION_Y = Down;
const int HERO_SPEED_RUN = TILE_WIDTH;	//Hero runs 1 tile per frame (the size of a brick)
const int HERO_SPEED_FALL = 2 * TILE_HEIGHT;	//speed in the beginning of jumping 

//fps value
const int FRAMES_PER_SECOND = 15;

//The frame rate regulator
Timer fps;

//The renderer and the surface
SDL_Renderer* renderer;
SDL_Window* screen = NULL;
SDL_Texture* background = NULL;
SDL_Texture* bulletimage_right = NULL;
SDL_Texture* bulletimage_left = NULL;
std::vector<SDL_Texture*> brickimages;
std::vector<SDL_Texture*> heroimages;
std::vector<SDL_Texture*> enemyimages;

//The font and color that's going to be used
TTF_Font* font = NULL;
SDL_Color textColor = {0, 0, 0};

//Event structure
SDL_Event event;

//The enemy
std::vector<Enemy*> enemies;

//The camera
SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

//The bricks
std::vector<Brick*> bricks;

//The bullets
std::vector<Bullet*> bullets;

//Score must be global
Score* score;

SDL_Texture* load_image(std::string filename)
{
	//The image that's loaded
	SDL_Surface* loadedImage = NULL;

	//The optimized image that will be used
	SDL_Texture* optimizedImage = NULL;

	//Load image
	loadedImage = IMG_Load(filename.c_str());

	//If the image loaded
	if(loadedImage != NULL)
	{
		//Map the color key
		Uint32 colorkey = SDL_MapRGB(loadedImage->format, 0, 0xFF, 0xFF);

		//Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
		SDL_SetColorKey(loadedImage, SDL_TRUE, colorkey);	

		//Create an optimized image
		optimizedImage = SDL_CreateTextureFromSurface(renderer, loadedImage);

		//Free the old image
		SDL_FreeSurface(loadedImage);

	}

	//Return the optimized image
	return optimizedImage;
}

void apply_surface(int x, int y, SDL_Texture* source, SDL_Renderer* rend, SDL_Rect* clip)
{
	//Temporary rectangle to hold the offsets
	SDL_Rect* offset = new SDL_Rect();

	//Get the offset
	offset -> x = x;
	offset -> y = y;

	//Get the width and height of the source
	SDL_QueryTexture(source, NULL, NULL, &(offset -> w), &(offset -> h));

	if(clip != NULL)
	{
		offset -> w = clip -> w;
		offset -> h = clip -> h;
	}

	//Copy the "clip" portion of the source to the "x,y" position of the window
	SDL_RenderCopy(rend, source, clip, offset);

}

bool init()
{/*
 //Old sdl initialization code

 //Initialize all SDL subsystems
 if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
 return false;

 //Set up the screen
 screen = SDL_CreateWindow("Hero v1.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

 //Create the renderer
 renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);

 //if there was an error in setting up the screen
 if(screen == NULL)
 return false;

 //if there was an error creating the renderer
 if(renderer == NULL)
 return false;

 //Initialize the sdl_ttf
 if(TTF_Init() == -1)
 return false;

 //If everything initialized fine
 return true;*/

	//New SDL initialization code

	//Result variable of initilazition process
	bool success = true;

	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not be initialized! SDL error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Enable VSync
		if(!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1"))
		{
			printf("Warning: VSync is not enabled!");
		}

		//set texture filtering to linear
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering is not enabled!");
		}

		//Create window
		screen = SDL_CreateWindow("Hero v1.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(screen == NULL)
		{
			printf("Window could not be created! SDL error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
			if(renderer == NULL)
			{
				printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize SDL_ttf
				if(TTF_Init() == -1)
				{
					printf("SDL_ttf could not be initialized! SDL_ttf error: %s\n", TTF_GetError());
					success = false;
				}

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not be initialized! SDL_image error: %s\n", IMG_GetError());
					success = false;
				}
			}


		}
	}

	return success;

}

bool load_files()
{
	//Load images
	background = load_image("images/background.png");

	heroimages.push_back(load_image("images/hero_stand_left.png"));				//0
	heroimages.push_back(load_image("images/hero_stand_right.png"));			//1
	heroimages.push_back(load_image("images/hero_run_left_1.png"));				//2
	heroimages.push_back(load_image("images/hero_run_left_2.png"));				//3
	heroimages.push_back(load_image("images/hero_run_left_3.png"));				//4
	heroimages.push_back(load_image("images/hero_run_right_1.png"));			//5
	heroimages.push_back(load_image("images/hero_run_right_2.png"));			//6
	heroimages.push_back(load_image("images/hero_run_right_3.png"));			//7
	heroimages.push_back(load_image("images/hero_air_left.png"));				//8
	heroimages.push_back(load_image("images/hero_air_right.png"));				//9
	heroimages.push_back(load_image("images/hero_fire_left.png"));				//10
	heroimages.push_back(load_image("images/hero_fire_right.png"));				//11

	brickimages.push_back(load_image("images/brick_green_top.png"));				//0
	brickimages.push_back(load_image("images/brick_green_down.png"));				//1
	brickimages.push_back(load_image("images/brick_brown_middle.png"));				//2
	brickimages.push_back(load_image("images/brick_green_left.png"));				//3
	brickimages.push_back(load_image("images/brick_green_right.png"));				//4
	brickimages.push_back(load_image("images/brick_green_corner_leftup.png"));		//5
	brickimages.push_back(load_image("images/brick_green_corner_leftdown.png"));	//6
	brickimages.push_back(load_image("images/brick_green_corner_rightup.png"));		//7
	brickimages.push_back(load_image("images/brick_green_corner_rightdown.png"));	//8

	bulletimage_right = load_image("images/bullet_right.png");
	bulletimage_left = load_image("images/bullet_left.png");

	enemyimages.push_back(load_image("images/enemy_monster_right_0.png"));			//0
	enemyimages.push_back(load_image("images/enemy_monster_right_1.png"));			//1
	enemyimages.push_back(load_image("images/enemy_monster_right_2.png"));			//2
	enemyimages.push_back(load_image("images/enemy_monster_dead_right.png"));		//3
	enemyimages.push_back(load_image("images/enemy_monster_left_0.png"));			//4	
	enemyimages.push_back(load_image("images/enemy_monster_left_1.png"));			//5
	enemyimages.push_back(load_image("images/enemy_monster_left_2.png"));			//6
	enemyimages.push_back(load_image("images/enemy_monster_dead_left.png"));		//7
	enemyimages.push_back(load_image("images/enemy_skeleton_right_0.png"));			//8
	enemyimages.push_back(load_image("images/enemy_skeleton_right_1.png"));			//9
	enemyimages.push_back(load_image("images/enemy_skeleton_right_2.png"));			//10
	enemyimages.push_back(load_image("images/enemy_skeleton_dead_right.png"));		//11
	enemyimages.push_back(load_image("images/enemy_skeleton_left_0.png"));			//12
	enemyimages.push_back(load_image("images/enemy_skeleton_left_1.png"));			//13
	enemyimages.push_back(load_image("images/enemy_skeleton_left_2.png"));			//14
	enemyimages.push_back(load_image("images/enemy_skeleton_dead_left.png"));		//15
	enemyimages.push_back(load_image("images/enemy_zombie_right_0.png"));			//16
	enemyimages.push_back(load_image("images/enemy_zombie_right_1.png"));			//17
	enemyimages.push_back(load_image("images/enemy_zombie_right_2.png"));			//18
	enemyimages.push_back(load_image("images/enemy_zombie_dead_right.png"));		//19
	enemyimages.push_back(load_image("images/enemy_zombie_left_0.png"));			//20
	enemyimages.push_back(load_image("images/enemy_zombie_left_1.png"));			//21
	enemyimages.push_back(load_image("images/enemy_zombie_left_2.png"));			//22
	enemyimages.push_back(load_image("images/enemy_zombie_dead_left.png"));			//23
	enemyimages.push_back(load_image("images/enemy_dragon_right_0.png"));			//24
	enemyimages.push_back(load_image("images/enemy_dragon_right_1.png"));			//25
	enemyimages.push_back(load_image("images/enemy_dragon_right_2.png"));			//26
	enemyimages.push_back(load_image("images/enemy_dragon_dead_right.png"));		//27
	enemyimages.push_back(load_image("images/enemy_dragon_left_0.png"));			//28
	enemyimages.push_back(load_image("images/enemy_dragon_left_1.png"));			//29
	enemyimages.push_back(load_image("images/enemy_dragon_left_2.png"));			//30
	enemyimages.push_back(load_image("images/enemy_dragon_dead_left.png"));			//31


	font = TTF_OpenFont("lazy.ttf", 28);

	//If any load failure happens return false
	if(background == NULL || font == NULL)
		return false;

	for(Uint32 counter = 0; counter < heroimages.size(); counter++)
	{
		if(heroimages[counter] == NULL)
			return false;
	}

	for(Uint32 counter = 0; counter < brickimages.size(); counter++)
	{
		if(brickimages[counter] == NULL)
			return false;
	}

	for(Uint32 counter = 0; counter < enemyimages.size(); counter++)
	{
		if(enemyimages[counter] == NULL)
			return false;
	}

	if(bulletimage_right == NULL || bulletimage_left == NULL)
		return false;

	return true;
}

void cleanup()
{
	SDL_DestroyTexture(background);

	for(Uint32 counter = 0; counter < heroimages.size(); counter++)
	{
		SDL_DestroyTexture(heroimages[counter]);
	}

	for(Uint32 counter = 0; counter < brickimages.size(); counter++)
	{
		SDL_DestroyTexture(brickimages[counter]);
	}

	for(Uint32 counter = 0; counter < enemyimages.size(); counter++)
	{
		SDL_DestroyTexture(enemyimages[counter]);
	}

	SDL_DestroyTexture(bulletimage_right);
	SDL_DestroyTexture(bulletimage_left);	

	TTF_CloseFont(font);
	TTF_Quit();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(screen);

	SDL_Quit();
}


int main(int argc, char* args[])
{
	//Make sure the program waits for a quit
	bool quit = false;

	//Initialize
	if(init() == false)
		return 1;

	//Load files
	load_files();

	//Initialize the hero
	Hero* hero = new Hero();
	hero->set_images_right(heroimages[1], heroimages[5], heroimages[6], heroimages[7], heroimages[9], heroimages[11]);
	hero->set_images_left(heroimages[0], heroimages[2], heroimages[3], heroimages[4], heroimages[8], heroimages[10]);

	//Initialize bricks
	Brickloader brickloader("level1.map", ';', LEVEL_WIDTH / TILE_WIDTH, LEVEL_HEIGHT / TILE_HEIGHT);
	if(brickloader.load_bricks() == false)
	{
		cleanup();
		return 1;	
	}	

	//Initialize enemies
	Enemyloader enemyloader("level1.map", ';', LEVEL_WIDTH / TILE_WIDTH, LEVEL_HEIGHT / TILE_HEIGHT);
	if(enemyloader.load_enemies() == false)
	{
		cleanup();
		return 1;
	}

	//Initialize score
	score = new Score();

	while(quit == false)
	{
		//Start the frame timer
		fps.start();

		while(SDL_PollEvent(&event))
		{
			//If the user has X ed out the window
			if(event.type == SDL_QUIT)
			{
				//Quit program
				quit = true;
			}

			//Handle user events for the hero
			hero->handle_input();
		}

		//Move the hero
		hero->move();

		//if health of an enemy is zero, destroy it
		for(Uint32 counter = 0; counter < enemies.size(); counter++)
		{
			if(enemies[counter] != NULL)
			{
				if(enemies[counter]->get_health() == 0)
				{
					enemies[counter]->set_state(0);		//if enemy's health is 0, set it's state to dead					
				}
			}
		}

		//Move the enemies
		for(Uint32 counter = 0; counter < enemies.size(); counter++)
		{
			if(enemies[counter] != NULL)
			{
				if(enemies[counter]->move() == false)		//if enemy is dead enough destroy it
				{
					enemies[counter] = NULL;
					score->incScore();
				}
			}
		}

		//Set the camera on the hero
		hero->set_camera();

		//Show the background

		//define another camera for rendering the background image
		SDL_Rect background_camera;
		background_camera.x = camera.x;
		background_camera.y = camera.y;
		background_camera.w = camera.w;
		background_camera.h = camera.h;

		//get the width and the height of the background image
		int background_image_width = 0;
		int background_image_height = 0;

		SDL_QueryTexture(background, NULL, NULL, &background_image_width, &background_image_height);

		//if the camera is out of the borders of the background image, return it to the beginning
		if(background_camera.x + background_camera.w > background_image_width)
			background_camera.x = camera.x % (background_image_width - background_camera.w);
		if(background_camera.y + background_camera.h > background_image_height)
			background_camera.y = camera.y % (background_image_height - background_camera.h);

		apply_surface(0, 0, background, renderer, &background_camera);

		//Show the hero
		hero->show();

		//Show the enemies
		for(Uint32 counter = 0; counter < enemies.size(); counter++)
		{
			if(enemies[counter] != NULL)
				enemies[counter]->show();
		}

		//Show the bricks
		for(Uint32 counter = 0; counter < bricks.size(); counter++)
		{
			bricks[counter]->show();
		}

		//Show the score
		if(score->showScore() == false)
			break;

		//Show health
		if(score->showHealth(hero->get_health()) == false)
			break;

		//Update the renderer
		SDL_RenderPresent(renderer);

		//stop the game if the hero dies
		if(hero->get_health() == 0)
			break;

		//Cap the frame rate
		if(fps.get_ticks() < 1000 / FRAMES_PER_SECOND)
		{
			SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
		}

	}

	//Free the surfaces and quit SDL
	cleanup();

	return 0;
}