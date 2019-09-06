
//The brick: building blocks of the walls and obstacles

#ifndef BRICK_H__
#define BRICK_H__

class Brick;

extern const int TILE_WIDTH;
extern const int TILE_HEIGHT;
extern const int BRICK_WIDTH;
extern const int BRICK_HEIGHT;

extern SDL_Rect camera;
extern SDL_Window* screen;
extern SDL_Renderer* renderer;
extern void apply_surface(int x, int y, SDL_Texture* source, SDL_Renderer* rend, SDL_Rect* clip = NULL);

class Brick
{
private:
	//The position and size rectangle of the brick
	SDL_Rect box;

	//The image surface of the brick
	SDL_Texture* brick_image;
public:
	//The constructor of the Brick class
	Brick();

	//The getter and setter methods of private variables
	SDL_Rect get_box();
	SDL_Texture* get_image();

	void set_box(int X, int Y, int W, int H);
	void set_image(SDL_Texture* image);

	void show();
};

Brick::Brick()
{
	box.x = 0;
	box.y = 0;
	box.w = 0;
	box.h = 0;
	brick_image = NULL;
}

SDL_Rect Brick::get_box()
{
	return box;
}

SDL_Texture* Brick::get_image()
{
	return brick_image;
}

void Brick::set_box(int X, int Y, int W, int H)
{
	box.x = X;
	box.y = Y;
	box.w = W;
	box.h = H;
}

void Brick::set_image(SDL_Texture* image)
{
	brick_image = image;
}

void Brick::show()
{
	//Show the brick
	apply_surface(box.x - camera.x, box.y - camera.y, brick_image, renderer);
}

#endif