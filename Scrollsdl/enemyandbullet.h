
#ifndef BULLET_H__
#define BULLET_H__

extern const int ENEMY_WIDTH;
extern const int ENEMY_HEIGHT;
extern const int ENEMY_SPEED;
extern const int BULLET_WIDTH;
extern const int BULLET_HEIGHT;
extern const int BULLET_SPEED;

extern SDL_Surface* screen;
extern SDL_Surface* enemyimage_right;
extern SDL_Surface* enemyimage_left;
extern SDL_Rect camera;

extern Enemy* enemy;

extern std::vector<Brick*> bricks;
extern std::vector<Bullet*> bullets;

extern Score* score;

extern void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
extern int check_collision(SDL_Rect A, SDL_Rect B);



class Bullet
{
private:

	SDL_Rect box;
	SDL_Surface* image;
	int speed;
	int direction;
	int frame;

public:

	Bullet(int X, int Y, int Direction);

	SDL_Rect get_box();
	int get_direction();

	void set_image(SDL_Surface* pImage);

	bool check_collision_brick();	//checks collision if the bullet collided with any of the bricks (returns true if any collision happened)
	bool check_collision_enemy();	//checks collision if the bullet collided with the enemy
	bool move();
	void show();

	bool is_dead();

};

void Enemy::show()
{	
	apply_surface(box.x - camera.x, box.y - camera.y, image, screen);
}

//Direction 0 right, 1 left
Bullet::Bullet(int X, int Y, int Direction)
{
	box.x = X;
	box.y = Y;
	box.w = BULLET_WIDTH;
	box.h = BULLET_HEIGHT;
	speed = BULLET_SPEED;
	direction = Direction;
}

SDL_Rect Bullet::get_box()
{
	return box;
}

int Bullet::get_direction()
{
	return direction;
}

void Bullet::set_image(SDL_Surface* pImage)
{
	image = pImage;
}

bool Bullet::check_collision_brick()
{
	SDL_Rect brick_collisionbox;
	for(Uint32 counter = 0; counter < bricks.size(); counter++)
	{
		brick_collisionbox = bricks[counter]->get_box();
		if(direction == 0)	//if bullet go right
		{
			if((box.x + box.w == brick_collisionbox.x || box.x == brick_collisionbox.x) && box.y >= brick_collisionbox.y 
				&& box.y + box.h <= brick_collisionbox.y + brick_collisionbox.h)
				return true;
		}
		else if(direction == 1)	//if bullet go left		
		{
			if((box.x == brick_collisionbox.x + brick_collisionbox.w || box.x + box.w == brick_collisionbox.x + brick_collisionbox.w)
				&& box.y >= brick_collisionbox.y && box.y + box.h <= brick_collisionbox.y + brick_collisionbox.h )
				return true;
		}
	}

	return false;
}

bool Bullet::check_collision_enemy()
{
	
	SDL_Rect enemy_collisionbox = enemy->get_box();
	if(direction == 0)	//if bullet go right
	{
		if((box.x + box.w == enemy_collisionbox.x || box.x == enemy_collisionbox.x) && box.y >= enemy_collisionbox.y 
			&& box.y + box.h <= enemy_collisionbox.y + enemy_collisionbox.h)
			return true;
	}
	else if(direction == 1)	//if bullet go left		
	{
		if((box.x == enemy_collisionbox.x + enemy_collisionbox.w || box.x + box.w == enemy_collisionbox.x + enemy_collisionbox.w)
			&& box.y >= enemy_collisionbox.y && box.y + box.h <= enemy_collisionbox.y + enemy_collisionbox.h )
			return true;
	}
	
	return false;
}

bool Bullet::move()
{
	//check if bullet collided with any of the bricks
	if(check_collision_brick() == true)
		return false;

	//check if bullet collided with the enemy
	if(enemy != NULL)
	{
		if(check_collision_enemy() == false)
		{
			enemy->decHealth();
			return false;
		}
	}	

	if(direction == 0)
		box.x += speed;
	else if(direction == 1)
		box.x -= speed;

	return true;
}

void Bullet::show()
{
	apply_surface(box.x - camera.x, box.y - camera.y, image, screen);
	frame++;
}

bool Bullet::is_dead()
{
	bool dead = false;
	if(frame >= 20)
		dead = true;
	return dead;
}

#endif