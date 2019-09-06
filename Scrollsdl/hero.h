
#ifndef HERO_H__
#define HERO_H__

//when status is InAir, the hero can have a speed and direction
//when status is InAir, status cannot be something else until jumping or falling action gets completed
//status can be Firing if it is Standing before, that the hero can fire only while he's standing
enum Hero_state{Standing, Running, InAir, Firing};
enum Hero_direction{Left, Right, Up, Down};

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int SCREEN_BPP;
extern const int HERO_WIDTH;
extern const int HERO_HEIGHT;
extern const int HERO_START_X;
extern const int HERO_START_Y;
extern const Hero_direction HERO_START_DIRECTION_X;
extern const Hero_direction HERO_START_DIRECTION_Y;
extern const int HERO_SPEED_RUN;
extern const int HERO_SPEED_FALL;
extern const int LEVEL_WIDTH;
extern const int LEVEL_HEIGHT;

extern SDL_Window* screen;
extern SDL_Renderer* renderer;
extern SDL_Texture* bulletimage_right;
extern SDL_Texture* bulletimage_left;
extern SDL_Event event;
extern std::vector<Brick*> bricks;
extern std::vector<Bullet*> bullets;
extern std::vector<Enemy*> enemies;
extern SDL_Rect camera;

extern int check_collision(SDL_Rect A, SDL_Rect B);
extern void apply_surface(int x, int y, SDL_Texture* source, SDL_Renderer* rend, SDL_Rect* clip);

class Hero
{
private:
	SDL_Rect box;
	int xSpeed, ySpeed;

	SDL_Texture* image_stand_right;
	SDL_Texture* image_run_right_0;
	SDL_Texture* image_run_right_1;
	SDL_Texture* image_run_right_2;
	SDL_Texture* image_air_right;
	SDL_Texture* image_fire_right;

	SDL_Texture* image_stand_left;
	SDL_Texture* image_run_left_0;
	SDL_Texture* image_run_left_1;
	SDL_Texture* image_run_left_2;
	SDL_Texture* image_air_left;
	SDL_Texture* image_fire_left;

	Hero_state status;
	double health;
	Hero_direction xDirection;
	Hero_direction yDirection;

	//0 if first running image, 2 if third running image is being displayed
	int anim_image_index;

	void show_bullets();

public:
	Hero();
	
	SDL_Rect get_box();
	Hero_state get_status();
	Hero_direction get_direction_x();
	Hero_direction get_direction_y();
	double get_health();

	void set_images_right(SDL_Texture* p_image_stand_right, SDL_Texture* p_image_run_right_0, SDL_Texture* p_image_run_right_1, SDL_Texture* p_image_run_right_2, 
		SDL_Texture* p_image_air_right, SDL_Texture* p_image_fire_right);

	void set_images_left(SDL_Texture* p_image_stand_left, SDL_Texture* p_image_run_left_0, SDL_Texture* p_image_run_left_1, SDL_Texture* p_image_run_left_2, 
		SDL_Texture* p_image_air_left, SDL_Texture* p_image_fire_left);

	void set_status(Hero_state pStatus);	

	void handle_input();
	int check_step(SDL_Rect brick_collisionbox);		//returns y pos of the brick the hero will land (or already landed). Returns 0 if hero won,t land.
	bool check_brick_front(SDL_Rect brick_collisionbox);	//returns true if there is a brick in front of the hero
	bool check_collision_enemy(SDL_Rect enemy_collisionbox);	//returns true if hero collided with enemy
	void move();
	void show();

	void set_camera();
	
};

Hero::Hero()
{
	box.x = HERO_START_X;
	box.y = HERO_START_Y;
	box.w = HERO_WIDTH;
	box.h = HERO_HEIGHT;
	xSpeed = 0;
	ySpeed = 0;
	health = 10;
	xDirection = HERO_START_DIRECTION_X;
	yDirection = HERO_START_DIRECTION_Y;
	status = Standing;
	anim_image_index = 0;	

	//set image surfaces to null
	image_stand_right = NULL;
	image_run_right_0 = NULL;
	image_run_right_1 = NULL;
	image_run_right_2 = NULL;
	image_air_right = NULL;
	image_fire_right = NULL;

	image_stand_left = NULL;
	image_run_left_0 = NULL;
	image_run_left_1 = NULL;
	image_run_left_2 = NULL;
	image_air_left = NULL;
	image_fire_left = NULL;
}

SDL_Rect Hero::get_box()
{
	return box;
}

Hero_direction Hero::get_direction_x()
{
	return xDirection;
}

Hero_direction Hero::get_direction_y()
{
	return yDirection;
}

double Hero::get_health()
{
	return health;
}

void Hero::set_images_right(SDL_Texture* p_image_stand_right, SDL_Texture* p_image_run_right_0, SDL_Texture* p_image_run_right_1, SDL_Texture* p_image_run_right_2, 
		SDL_Texture* p_image_air_right, SDL_Texture* p_image_fire_right)
{
	image_stand_right = p_image_stand_right;
	image_run_right_0 = p_image_run_right_0;
	image_run_right_1 = p_image_run_right_1;
	image_run_right_2 = p_image_run_right_2;
	image_air_right = p_image_air_right;
	image_fire_right = p_image_fire_right;
}

void Hero::set_images_left(SDL_Texture* p_image_stand_left, SDL_Texture* p_image_run_left_0, SDL_Texture* p_image_run_left_1, SDL_Texture* p_image_run_left_2, 
		SDL_Texture* p_image_air_left, SDL_Texture* p_image_fire_left)
{
	image_stand_left = p_image_stand_left;
	image_run_left_0 = p_image_run_left_0;
	image_run_left_1 = p_image_run_left_1;
	image_run_left_2 = p_image_run_left_2;
	image_air_left = p_image_air_left;
	image_fire_left = p_image_fire_left;
}

void Hero::set_status(Hero_state pStatus)
{
	status = pStatus;
}

void Hero::handle_input()
{
	if(event.type == SDL_KEYDOWN)
	{
		switch(event.key.keysym.sym)
		{
		case SDLK_LEFT:
			if(status != InAir)
			{
				status = Running;
				xDirection = Left;
				xSpeed = HERO_SPEED_RUN;
			}
			break;
		case SDLK_RIGHT:
			if(status != InAir)
			{
				status = Running;
				xDirection = Right;
				xSpeed = HERO_SPEED_RUN;
			}
			break;
		case SDLK_UP:
			if(status != InAir)
			{
				status = InAir;
				yDirection = Up;
				ySpeed = HERO_SPEED_FALL;	
			}
			break;
		case SDLK_x:
			if(status == Standing)			
				status = Firing;
			if(status == Firing)
			{
				if(xDirection == Right)
				{
					bullets.push_back(new Bullet(box.x + box.w, box.y + 8 - BULLET_HEIGHT / 2, 0));
					bullets[bullets.size() - 1]->set_image(bulletimage_right);
				}
				else if(xDirection == Left)
				{
					bullets.push_back(new Bullet(box.x, box.y + 8 - BULLET_HEIGHT / 2, 1));
					bullets[bullets.size() - 1]->set_image(bulletimage_left);
				}
			}			
			
			break;
		}
	}
	else if(event.type == SDL_KEYUP)
	{
		switch(event.key.keysym.sym)
		{
		case SDLK_LEFT:
			if(status != InAir)
			{
				status = Standing;			
				xSpeed = 0;
			}
			break;
		case SDLK_RIGHT:
			if(status != InAir)
			{
				status = Standing;			
				xSpeed = 0;
			}
			break;
		case SDLK_UP:
			break;		
		}
	}
}

int Hero::check_step(SDL_Rect brick_collisionbox)
{
	int brick_ypos = 0;	
	
	//if hero is very very close to a brick below or there is a brick between current position and
	//next position (these can be far while falling) of the hero return yPos of the brick in question
	if(xDirection == Right &&(((abs(brick_collisionbox.y - box.y - box.h) <= 10) && abs(brick_collisionbox.x - box.x) <= brick_collisionbox.w / 2) ||			
		(box.y + box.h < brick_collisionbox.y && box.y + box.h + ySpeed > brick_collisionbox.y && abs(brick_collisionbox.x - box.x) <= brick_collisionbox.w / 2)))
		brick_ypos = brick_collisionbox.y;
	if(xDirection == Left &&(((abs(brick_collisionbox.y - box.y - box.h) <= 10) && abs(brick_collisionbox.x - box.x) <= box.w / 2) ||			
		(box.y + box.h < brick_collisionbox.y && box.y + box.h + ySpeed > brick_collisionbox.y && abs(brick_collisionbox.x - box.x) <= box.w / 2)))
		brick_ypos = brick_collisionbox.y;
	
	return brick_ypos;
}

bool Hero::check_brick_front(SDL_Rect brick_collisionbox)
{
	//check if any brick stands in front of the hero		
	if(xDirection == Right && abs(brick_collisionbox.y - box.y) < box.h && abs(brick_collisionbox.x - box.x - box.w) <= 1)
		return true;

	if(xDirection == Left && abs(brick_collisionbox.y - box.y) < box.h && abs(brick_collisionbox.x + brick_collisionbox.w - box.x) <= 1)
		return true;	

	return false;
}

bool Hero::check_collision_enemy(SDL_Rect enemy_collisionbox)
{
	if((xDirection == Right && abs(enemy_collisionbox.x - box.x - box.w) <= 8 && abs(enemy_collisionbox.y - box.y) <= box.h) ||
		(xDirection == Left && abs(enemy_collisionbox.x + enemy_collisionbox.w - box.x) <= 8 && abs(enemy_collisionbox.y - box.y) <= box.h))
		return true;
	return false;
}

void Hero::move()
{
	//Move the hero according to it's status, direction and speed

	//if hero steps on a brick, move it in x pos
	bool heroStepsOnABrick = false;
	for(Uint32 counter = 0; counter < bricks.size(); counter++)
	{
		if(check_step(bricks[counter]->get_box()) != 0)
			heroStepsOnABrick = true;
	}

	bool brickOnTheWay = false;
	for(Uint32 counter = 0; counter < bricks.size(); counter++)
	{
		if(check_brick_front(bricks[counter]->get_box()) == true)
			brickOnTheWay = true;
	}
	
	if(heroStepsOnABrick != 0 || status == InAir)
	{
		if(xDirection == Right && brickOnTheWay == false)
			box.x += xSpeed;
		else if(xDirection == Left && brickOnTheWay == false)
			box.x -= xSpeed;	
	}
	else if(yDirection != Up)	//if hero doesn't step on a brick, make it fall
	{
		status = InAir;
		yDirection = Down;
		xSpeed = 0;
	}
	
	
	if(status == InAir)	//y position of the hero will be changed only when it's in the air
	{
		//Adjust ySpeed according to yDirection and previous value of ySpeed
		if(yDirection == Up)	//if the hero is rising
		{
			if(ySpeed != 0)		//until the ySpeed reaches 1, reduce it to give the illusion of gravity
			{
				ySpeed -= 2;
			}
			else		//when ySpeed reaches 1, change yDirection to Down
			{				
				yDirection = Down;
			}			
		}
		else if(yDirection == Down)	//if the hero is falling, increase it's speed by two
		{
			ySpeed += 2;
		}	

		if(yDirection == Up)
			box.y -= ySpeed;
		else if(yDirection == Down)
			box.y += ySpeed;		

		int brick_land_ypos = 0;
		for(Uint32 counter = 0; counter < bricks.size(); counter++)
		{
			brick_land_ypos = check_step(bricks[counter]->get_box());
			if(brick_land_ypos != 0)
				break;
		}

		if(yDirection == Down && brick_land_ypos != 0)
		{			
			status = Standing;
			box.y = brick_land_ypos - box.h;
			xSpeed = 0;
			ySpeed = 0;
		}	

	}

	//move the bullets and delete the ones that can,t move
	for(Uint32 counter = 0; counter < bullets.size(); counter++)
	{
		if(bullets[counter]->move() == false)
		{
			bullets.erase(bullets.begin() + counter);
		}
	}

	//check if the hero collided with the enemy
	for(Uint32 counter = 0; counter < enemies.size(); counter++)
	{
		if(enemies[counter] != NULL)
		{			
			if(enemies[counter]->get_state() == 1 && check_collision_enemy(enemies[counter]->get_box()) == true)
			{
				health -= 0.5;
			}
		}
	}
	
}

void Hero::show_bullets()
{
	//destroy dead bullets
	for(Uint32 counter = 0; counter < bullets.size(); counter++)
	{
		if(bullets[counter]->is_dead())
			bullets.erase(bullets.begin() + counter);
	}

	//show bullets
	for(Uint32 counter = 0; counter < bullets.size(); counter++)
		bullets[counter]->show();
}

void Hero::show()
{
	if(status == Standing)	
	{
		if(xDirection == Left)
			apply_surface(box.x - camera.x, box.y - camera.y, image_stand_left, renderer);
		else if(xDirection == Right)
			apply_surface(box.x - camera.x, box.y - camera.y, image_stand_right, renderer);
	}
	else if(status == Running)
	{
		if(xDirection == Left)
		{
			if(anim_image_index == 0)
			{
				apply_surface(box.x - camera.x, box.y - camera.y, image_run_left_0, renderer);
			}
			else if(anim_image_index == 1)
			{
				apply_surface(box.x - camera.x, box.y - camera.y, image_run_left_1, renderer);
			}
			else if(anim_image_index == 2)
			{
				apply_surface(box.x - camera.x, box.y - camera.y, image_run_left_2, renderer);
			}
		}
		else if(xDirection == Right)
		{
			if(anim_image_index == 0)
			{
				apply_surface(box.x - camera.x, box.y - camera.y, image_run_right_0, renderer);
			}
			else if(anim_image_index == 1)
			{
				apply_surface(box.x - camera.x, box.y - camera.y, image_run_right_1, renderer);
			}
			else if(anim_image_index == 2)
			{
				apply_surface(box.x - camera.x, box.y - camera.y, image_run_right_2, renderer);
			}
		}

		anim_image_index++;

		if(anim_image_index == 3)	//show the first image again
			anim_image_index = 0;
	}
	else if(status == InAir)
	{
		if(xDirection == Left)
			apply_surface(box.x - camera.x, box.y - camera.y, image_air_left, renderer);
		else if(xDirection == Right)
			apply_surface(box.x - camera.x, box.y - camera.y, image_air_right, renderer);
	}
	else if(status == Firing)
	{
		if(xDirection == Left)
			apply_surface(box.x - camera.x, box.y - camera.y, image_fire_left, renderer);
		else if(xDirection == Right)
			apply_surface(box.x - camera.x, box.y - camera.y, image_fire_right, renderer);
	}

	show_bullets();
}

void Hero::set_camera()
{
	//Center the camera over the hero
	camera.x = (box.x + box.w /2) - SCREEN_WIDTH / 2;
	camera.y = (box.y + box.h / 2) - SCREEN_HEIGHT / 2;

	//Keep the camera in bounds
	if(camera.x < 0)
		camera.x = 0;
	if(camera.y < 0)
		camera.y = 0;
	if(camera.x > LEVEL_WIDTH - camera.w)
		camera.x = LEVEL_WIDTH - camera.w;
	if(camera.y > LEVEL_HEIGHT - camera.h)
		camera.y = LEVEL_HEIGHT - camera.h;
}

#endif