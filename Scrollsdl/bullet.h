
#ifndef BULLET_H__
#define BULLET_H__

class Bullet;

extern const int BULLET_WIDTH;
extern const int BULLET_HEIGHT;
extern const int BULLET_SPEED;

extern SDL_Window* screen;
extern SDL_Renderer* renderer;
extern SDL_Rect camera;

extern std::vector<Enemy*> enemies;

extern std::vector<Brick*> bricks;
extern std::vector<Bullet*> bullets;

extern Score* score;

extern void apply_surface(int x, int y, SDL_Texture* source, SDL_Renderer* rend, SDL_Rect* clip);

class Bullet
{
private:

	SDL_Rect box;
	SDL_Texture* image;
	int speed;
	int direction;
	int frame;

public:

	Bullet(int X, int Y, int Direction);

	SDL_Rect get_box();
	int get_direction();

	void set_image(SDL_Texture* pImage);

	bool check_collision_brick(SDL_Rect brick_collisionbox);	//checks collision if the bullet collided with any of the bricks (returns true if any collision happened)
	bool check_collision_enemy(SDL_Rect enemy_collisionbox);	//checks collision if the bullet collided with the enemy
	bool move();
	void show();

	bool is_dead();

};

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

void Bullet::set_image(SDL_Texture* pImage)
{
	image = pImage;
}

bool Bullet::check_collision_brick(SDL_Rect brick_collisionbox)
{		
	if(direction == 0)	//if bullet go right
	{
		if(abs(brick_collisionbox.x - box.x - box.w) <= box.w && abs(brick_collisionbox.y - box.y) < brick_collisionbox.h)
			return true;
	}
	else if(direction == 1)	//if bullet go left		
	{
		if(abs(box.x - brick_collisionbox.x - brick_collisionbox.w) <= box.w && abs(brick_collisionbox.y - box.y) < brick_collisionbox.h)
			return true;
	}
	
	return false;
}

bool Bullet::check_collision_enemy(SDL_Rect enemy_collisionbox)
{	
	
	if(direction == 0)	//if bullet go right
	{
		if(abs(enemy_collisionbox.x - box.x - box.w) <= box.w && abs(enemy_collisionbox.y - box.y) < box.h)
			return true;
	}
	else if(direction == 1)	//if bullet go left		
	{
		if(abs(box.x - enemy_collisionbox.x - enemy_collisionbox.w) <= box.w && abs(enemy_collisionbox.y - box.y) < box.h)
			return true;
	}
	
	return false;
}

bool Bullet::move()
{
	//check if bullet collided with any of the bricks
	for(Uint32 counter = 0; counter < bricks.size(); counter++)
	{
		SDL_Rect brick_collisionbox = bricks[counter]->get_box();
		if(check_collision_brick(brick_collisionbox) == true)
			return false;
	}

	//check if bullet collided with the enemy
	for(Uint32 counter = 0; counter < enemies.size(); counter++)
	{
		if(enemies[counter] != NULL)
		{
			if(enemies[counter]->get_state() == 1 && check_collision_enemy(enemies[counter]->get_box()) == true)
			{
				enemies[counter]->decHealth();
				return false;
			}
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
	apply_surface(box.x - camera.x, box.y - camera.y, image, renderer);
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