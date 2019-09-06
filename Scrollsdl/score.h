
//The score class holds scores; shows scores and health on the screen

#ifndef SCORE_H__
#define SCORE_H__

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern SDL_Window* screen;
extern SDL_Renderer* renderer;
extern SDL_Rect camera;

extern TTF_Font* font;
extern SDL_Color textColor;

extern void apply_surface(int x, int y, SDL_Texture* source, SDL_Renderer* rend, SDL_Rect* clip);

class Score
{
private:

	//score of the hero
	int scoreValue;

public:
	//Constructor
	Score();

	//Increments score by one
	void incScore();

	//Shows score on the screen
	bool showScore();

	//Shows health on the screen
	bool showHealth(double health);

};

Score::Score()
{
	scoreValue = 0;
}

void Score::incScore()
{
	scoreValue++;
}

bool Score::showScore()
{
	std::stringstream scoreString;
	scoreString << "Score: " << scoreValue;
	SDL_Surface* message = TTF_RenderText_Solid(font, scoreString.str().c_str(), textColor);
	SDL_Texture* message_texture = SDL_CreateTextureFromSurface(renderer, message);

	if(message == NULL)
		return false;

	apply_surface(SCREEN_WIDTH - message->w, 10, message_texture, renderer);

	SDL_FreeSurface(message);
	SDL_DestroyTexture(message_texture);

	return true;
}

bool Score::showHealth(double health)
{
	SDL_Rect* filled_healthBox = new SDL_Rect();
	SDL_Rect* empty_healthBox = new SDL_Rect();

	filled_healthBox -> x = 10;
	filled_healthBox -> y = 10;
	filled_healthBox -> w = 10 * health;
	filled_healthBox -> h = 10;

	empty_healthBox -> x = 10 + 10 * health;
	empty_healthBox -> y = 10;
	empty_healthBox -> w = 10 * (10 - health);
	empty_healthBox -> h = 10;

	//renderer draw color rgb
	Uint8* render_draw_color_r = new Uint8();
	Uint8* render_draw_color_g = new Uint8();
	Uint8* render_draw_color_b = new Uint8();
	Uint8* render_draw_color_a = new Uint8();

	//get renderer draw color
	SDL_GetRenderDrawColor(renderer, render_draw_color_r, render_draw_color_g, render_draw_color_b, render_draw_color_a);

	//set renderer draw color to green
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);

	//fill health rectangle
	if(SDL_RenderFillRect(renderer,filled_healthBox) == -1)
		return false;

	//set renderer draw color to red
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);

	//fill empty rectangle
	if(SDL_RenderFillRect(renderer, empty_healthBox) == -1)
		return false;

	//set old drawing color back
	SDL_SetRenderDrawColor(renderer, *render_draw_color_r, *render_draw_color_g, *render_draw_color_b, *render_draw_color_a);

	return true;

}

#endif