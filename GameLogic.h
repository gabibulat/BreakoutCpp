#pragma once
#include "Scene.h"
#include <stdlib.h>
#include <iostream>
#include <vector> 
#include <chrono> //?
#include <random>
#include <string>
#include <stdio.h>

using namespace std;
using namespace chrono;

const float ballSpeed= 0.5f;
const int screenWidth = 1024;
const int screenHeight = 768;
const int ballSize = 24;
const int paddleWidth = 128;
const int fixedPaddleY = 663;

enum class CollisionType
{ 
	None,
	BouncePaddleCorner, BouncePaddleMiddle1, BouncePaddleMiddle2,
	TopWall, RSideWall, LSideWall, ExitedScreen,
	RightSideBrick,	LeftSideBrick, TopSideBrick, BottomSideBrick
};

struct Contact{	CollisionType type; };

class Vec2
{
public:
	Vec2();
	Vec2(float x, float y);
	float x, y;

};

class Ball
{
public:

	Ball(Vec2 position, Vec2 velocity);
	void UpdatePosition(float dt);
	void CollideWith(Contact contact);
	Vec2 position, velocity;
	SDL_Rect ballPosition;
private:
	const float bounceAngle1 = 0.75f, bounceAngle2 = 0.45f;
};

class GameLogic
{
	
public:
	GameLogic();

private:

	Contact CheckifCollisionWall(float ball_x, float ball_y);
	Contact CheckPaddleCollision(SDL_Rect ballPosition, float px, float ballDirection);
	SDL_Rect mouse;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
	int level;
	bool running;
	float w, h, dy, dx, wy, hx, dt;
};
