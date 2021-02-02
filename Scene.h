#pragma once
#define SDL_MAIN_HANDLED 
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <vector> 
#include "GameLogic.h"
#include "LevelsXML.h"


struct Brick {
	char id;
	SDL_Rect brickPosition;
	int hitPoints; 
	int breakScore;
	Mix_Chunk* hitSound;
	Mix_Chunk* breakSound;
	bool exists;
};

class Scene
{
public:
	void SetNewLevel(int lvl, SDL_Renderer *r);
	void Update(int paddleXposition, SDL_Rect ballPosition);
	const int brickHeight = 24, brickWidth = 64;
	vector<Brick> brickObjects;
	int numberOfLives = 4, score = 0, brickCount;
private:
	void DrawOnScreen(SDL_Texture* texture, int x, int y);
	
	TTF_Font* font;
	SDL_Renderer* renderer;
	SDL_Rect sourceRect, destinationRect;
	SDL_Texture *backgroundTexture, *paddleTexture, *ballTexture, *lifeTexture, *brickTexture, *textLevel, *textScore;;
	SDL_Surface *emptySurface, *brickField, *brickSurfaceS;
	
	map <char, SDL_Surface*> brickSurface;

	const int fixedPaddleHeight = 663, screenWidth = 1024;
	
	int level = 1, lifeXposition = 140;

};