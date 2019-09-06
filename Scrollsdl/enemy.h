
#ifndef ENEMY_H__
#define ENEMY_H__

class Enemy;

enum Enemy_type{Monster, Skeleton, Zombie, Dragon};

extern const int ENEMY_WIDTH;
extern const int ENEMY_HEIGHT;
extern const int ENEMY_SPEED;

extern SDL_Window* screen;
extern SDL_Renderer* renderer;
extern SDL_Rect camera;

extern std::vector<Brick*> bricks;

extern void apply_surface(int x, int y, SDL_Texture* source, SDL_Renderer* rend, SDL_Rect* clip);

class Enemy
{
private:
	SDL_Rect box;	
	SDL_Texture* image_right_0;
	SDL_Texture* image_right_1;
	SDL_Texture* image_right_2;
	SDL_Texture* image_dead_right;
	SDL_Texture* image_left_0;
	SDL_Texture* image_left_1;
	SDL_Texture* image_left_2;
	SDL_Texture* image_dead_left;

	int speed;
	int direction;	//0 right 1 left
	double health;
	Enemy_type enemy_type;

	int enemy_state;	//1 alive 0 dead
	int frame_dead;		//increase this while enemy_state is dead

	int anim_image_index;
	
public:
	Enemy(int X, int Y, Enemy_type p_enemy_type);

	SDL_Rect get_box();
	double get_health();
	int get_state();

	void set_state(int p_enemy_state);

	void set_images_right(SDL_Texture* p_image_right_0, SDL_Texture* p_image_right_1, SDL_Texture* p_image_right_2, SDL_Texture* p_image_dead_right);
	void set_images_left(SDL_Texture* p_image_left_0, SDL_Texture* p_image_left_1, SDL_Texture* p_image_left_2, SDL_Texture* p_image_dead_left);

	void decHealth();
	bool check_step(SDL_Rect brick_collisionbox);
	bool check_brick_front(SDL_Rect brick_collisionbox);
	bool move();
	void show();

};

Enemy::Enemy(int X, int Y, Enemy_type p_enemy_type)
{
	box.x = X;
	box.y = Y;
	box.w = ENEMY_WIDTH;
	box.h = ENEMY_HEIGHT;
	speed = ENEMY_SPEED;
	direction = 0;		//startup direction is right
	health = 10;	
	anim_image_index = 0;
	enemy_type = p_enemy_type;
	enemy_state = 1;
	frame_dead = 0;

	image_right_0 = NULL;
	image_right_1 = NULL;
	image_right_2 = NULL;
	image_left_0 = NULL;
	image_left_1 = NULL;
	image_left_2 = NULL;
}

SDL_Rect Enemy::get_box()
{
	return box;
}

double Enemy::get_health()
{
	return health;
}

int Enemy::get_state()
{
	return enemy_state;
}

void Enemy::set_state(int p_enemy_state)
{
	enemy_state = p_enemy_state;
}

void Enemy::set_images_right(SDL_Texture* p_image_right_0, SDL_Texture* p_image_right_1, SDL_Texture* p_image_right_2, SDL_Texture* p_image_dead_right)
{
	image_right_0 = p_image_right_0;
	image_right_1 = p_image_right_1;
	image_right_2 = p_image_right_2;
	image_dead_right = p_image_dead_right;
}

void Enemy::set_images_left(SDL_Texture* p_image_left_0, SDL_Texture* p_image_left_1, SDL_Texture* p_image_left_2, SDL_Texture* p_image_dead_left)
{
	image_left_0 = p_image_left_0;
	image_left_1 = p_image_left_1;
	image_left_2 = p_image_left_2;
	image_dead_left = p_image_dead_left;
}

void Enemy::decHealth()
{
	health = health - 0.5;
}

bool Enemy::check_step(SDL_Rect brick_collisionbox)
{		
	if(abs(brick_collisionbox.x + brick_collisionbox.w - box.x) <= brick_collisionbox.w / 2 && abs(box.y - brick_collisionbox.y) <= box.h)
		return true;		

	return false;
}

bool Enemy::check_brick_front(SDL_Rect brick_collisionbox)
{	
	if(direction == 0 && abs(brick_collisionbox.x - box.x - box.w) <= 1 && abs(box.y - brick_collisionbox.y) < box.h)
		return true;
	if(direction == 1 && abs(brick_collisionbox.x + brick_collisionbox.w - box.x) <= 1 && abs(box.y - brick_collisionbox.y) < box.h)
		return true;

	return false;
}

bool Enemy::move()
{
	if(enemy_state == 1)
	{
		if(direction == 0)
			box.x += speed;
		else if(direction == 1)
			box.x -= speed;

		//if enemy collided with a brick, reverse it's direction
		bool brickFront = false;
		for(Uint32 counter = 0; counter < bricks.size(); counter++)
		{
			if(check_brick_front(bricks[counter]->get_box()) == true)
				brickFront = true;
		}

		if(brickFront == true && direction == 0)
		{
			direction = 1;		
		}
		else if(brickFront == true && direction == 1)
		{
			direction = 0;		
		}	

		//if enemy doesn't step on a brick, reverse it's direction
		bool enemy_step = false;
		for(Uint32 counter = 0; counter < bricks.size(); counter++)
		{
			if(check_step(bricks[counter] ->get_box()) == true)
				enemy_step = true;
		}

		if(enemy_step == false && direction == 0)
		{
			direction = 1;		
		}
		else if(enemy_step == false && direction == 1)
		{
			direction = 0;		
		}
	}
	else
	{
		if(frame_dead < 20)
			frame_dead++;
		else
			return false;
	}

	return true;

}

void Enemy::show()
{
	if(direction == 0)
	{
		if(enemy_state == 1)
		{
			if(anim_image_index == 0)
			{
				apply_surface(box.x - camera.x, box.y - camera.y, image_right_0, renderer);
				anim_image_index = 1;
			}
			else if(anim_image_index == 1)
			{
				apply_surface(box.x - camera.x, box.y - camera.y, image_right_1, renderer);
				anim_image_index = 2;
			}
			else if(anim_image_index == 2)
			{
				apply_surface(box.x - camera.x, box.y - camera.y, image_right_2, renderer);
				anim_image_index = 0;
			}
		}
		else
			apply_surface(box.x - camera.x, box.y - camera.y, image_dead_right, renderer);
	}
	else if(direction == 1)
	{
		if(enemy_state == 1)
		{
			if(anim_image_index == 0)
			{
				apply_surface(box.x - camera.x, box.y - camera.y, image_left_0, renderer);
				anim_image_index = 1;
			}
			else if(anim_image_index == 1)
			{
				apply_surface(box.x - camera.x, box.y - camera.y, image_left_1, renderer);
				anim_image_index = 2;
			}
			else if(anim_image_index == 2)
			{
				apply_surface(box.x - camera.x, box.y - camera.y, image_left_2, renderer);
				anim_image_index = 0;
			}
		}
		else
			apply_surface(box.x - camera.x, box.y - camera.y, image_dead_left, renderer);
	}	
}

#endif