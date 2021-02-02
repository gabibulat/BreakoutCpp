#include "GameLogic.h"

#pragma region VectorClass
Vec2::Vec2(): x(0.0f), y(0.0f){}

Vec2::Vec2(float x, float y): x(x), y(y){}
#pragma endregion VectorClass

#pragma region BallClass
Ball::Ball(Vec2 position, Vec2 velocity) : position(position), velocity(velocity)
{
	ballPosition.x = static_cast<int>(position.x);
	ballPosition.y = static_cast<int>(position.y);
	ballPosition.w = ballSize ;
	ballPosition.h = ballSize ;

}

void Ball::UpdatePosition(float dt)
{
	position.x += velocity.x * dt;
	position.y += velocity.y * dt;
	ballPosition.x = static_cast<int>(position.x);
	ballPosition.y = static_cast<int>(position.y);

}

void Ball::CollideWith(Contact  contact)
{
	// ball bouncing off  
	if (contact.type == CollisionType::BouncePaddleMiddle1)
	{
		velocity.x = -velocity.x;
		velocity.y = -bounceAngle1 * ballSpeed;

	}

	else if (contact.type == CollisionType::BouncePaddleCorner)
	{
		velocity.x = -velocity.x;
		velocity.y = -bounceAngle2 * ballSpeed; 

	}

	else if (contact.type == CollisionType::BouncePaddleMiddle2 || contact.type == CollisionType::TopSideBrick) velocity.y = -bounceAngle1 * ballSpeed;

	else if (contact.type == CollisionType::TopWall || contact.type == CollisionType::BottomSideBrick) velocity.y = bounceAngle1 * ballSpeed;

	else if (contact.type == CollisionType::LSideWall || contact.type == CollisionType::RightSideBrick) velocity.x = bounceAngle1 * ballSpeed;

	else if (contact.type == CollisionType::LeftSideBrick || contact.type == CollisionType::RSideWall) velocity.x = -bounceAngle1 * ballSpeed;

	else if (contact.type == CollisionType::ExitedScreen) {

		position.y = screenHeight / 2.5;
		position.x = rand() % (screenWidth - ballSize) + ballSize;

		velocity.x = rand() % 1 - 1 * velocity.x;;
		velocity.y = 0.55 * ballSpeed;

	}


}
#pragma endregion BallClass


Contact GameLogic::CheckifCollisionWall(float ball_x, float ball_y)
{
	Contact contact{};
	//top wall
	if (ball_y <= ballSize /2) contact.type = CollisionType::TopWall;

	//left wall
	else if (ball_x <= ballSize /2 && ball_y > ballSize /2 && (ball_y + ballSize ) <= screenHeight) contact.type = CollisionType::LSideWall;

	//right wall
	else if (ball_x + ballSize  >= (screenWidth- ballSize  / 2) && ball_y <= screenHeight) contact.type = CollisionType::RSideWall;

	//bottom wall
	else if (ball_y > screenHeight && ball_y < (screenHeight+ballSize *2)) {contact.type = CollisionType::ExitedScreen;	}

	return contact;

}

Contact GameLogic::CheckPaddleCollision(SDL_Rect ballPosition, float paddlePositionX, float ballDirection)
{	
	Contact contact{};
	float ballMiddleX = ballPosition.x + ballSize / 2;

	//ball bounces of the paddle depending on which part of the paddle it hits

	float part1 = paddlePositionX + paddleWidth/4, part2 = paddlePositionX + paddleWidth/4 * 2, part3 = paddlePositionX + paddleWidth/4 * 3;


	if ((ballPosition.y + ballSize) >= 655 && (ballPosition.y + ballSize) <= 695) {
		if ((ballPosition.x + ballSize+24) >= paddlePositionX && ballMiddleX< part1)
		{
			if (ballDirection >= 0)contact.type = CollisionType::BouncePaddleCorner;
			else if (ballDirection < 0)contact.type = CollisionType::BouncePaddleMiddle2;

		}
		else if (ballMiddleX>= part1 && ballMiddleX< part2)
		{
			if (ballDirection >= 0)contact.type = CollisionType::BouncePaddleMiddle1;
			else if (ballDirection < 0)contact.type = CollisionType::BouncePaddleMiddle2;

		}
		else if (ballMiddleX>= part2 && ballMiddleX< part3)
		{
			if (ballDirection >= 0)contact.type = CollisionType::BouncePaddleMiddle2;
			else if (ballDirection < 0)contact.type = CollisionType::BouncePaddleMiddle1;

		}
		else if (ballMiddleX>= part3 && ballPosition.x <= (paddlePositionX + 128))
		{
			if (ballDirection >= 0)contact.type = CollisionType::BouncePaddleMiddle2;
			else if (ballDirection < 0)contact.type = CollisionType::BouncePaddleCorner;

		}
	}
	return contact;
}


GameLogic::GameLogic()
{
	window = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	if (window == NULL) printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
	renderer= SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	Scene s;
	mouse.x = 448;
	level= 1;
	running = true;

	SDL_ShowCursor(0);

	s.SetNewLevel(level, renderer);
	
	srand((unsigned)time(NULL));


	Ball ball(Vec2((rand() % (screenWidth-ballSize) + ballSize), screenHeight/2.5), Vec2(ballSpeed, ballSpeed)); //POPRAVIT


	while (running) {
		
		auto startTime = high_resolution_clock::now();
		s.Update(mouse.x, ball.ballPosition);

		//exit when X is pressed
		while (SDL_PollEvent(&event)) if(event.type== SDL_QUIT)running = false;
		
		//paddle following cursor
		SDL_GetMouseState(&mouse.x, &mouse.y);
		if (mouse.x < 1) mouse.x = 1; //left wall
		else if (mouse.x > screenWidth - paddleWidth) mouse.x = screenWidth - paddleWidth; //right wall

		ball.UpdatePosition(dt);
	
	
#pragma region Collisions

		Contact contact2 = CheckifCollisionWall(ball.ballPosition.x, ball.ballPosition.y);
		if (contact2.type == CollisionType::ExitedScreen) { 
			s.numberOfLives--; 
			if (s.numberOfLives == 0) {
				s.numberOfLives = 4;
				level = 1;
				s.score = 0;
				s.SetNewLevel(level, renderer);
			}
		
		}
		ball.CollideWith(contact2);

		Contact contact1 = CheckPaddleCollision(ball.ballPosition, mouse.x, ball.velocity.x);
		ball.CollideWith(contact1);

#pragma region BrickCollisions
		Contact contact{};

		for(auto i = 0U; i <= s.brickObjects.size() - 1;i++) {
			if (s.brickObjects.at(i).exists) {
		
			w = ballSpeed * (ballSize  + s.brickWidth);
			h = ballSpeed * (ballSize  + s.brickHeight);
			dx = (ball.ballPosition.x + ballSpeed * 24) - (s.brickObjects.at(i).brickPosition.x + ballSpeed * s.brickWidth);
			dy = (ball.ballPosition.y + ballSpeed * 24) - (s.brickObjects.at(i).brickPosition.y + ballSpeed * 24);

				if (fabs(dx) <= w && fabs(dy) <= h) { // Ball and brick collided
					Mix_PlayChannel(-1, s.brickObjects.at(i).hitSound, 0);
			
					wy = w * dy;
					hx = h * dx;
		
					if (wy > hx) {
						if (wy > -hx) contact.type = CollisionType::BottomSideBrick; 
						else contact.type = CollisionType::LeftSideBrick; 
					}
					else {
						if (wy > -hx) contact.type = CollisionType::RightSideBrick; 
 						else contact.type = CollisionType::TopSideBrick; 
					}
					
					ball.CollideWith(contact);

					if (s.brickObjects.at(i).id != 'I') {
						s.brickObjects.at(i).hitPoints--;
						if (s.brickObjects.at(i).hitPoints == 0) {
							Mix_PlayChannel(-1, s.brickObjects.at(i).breakSound, 0);
						
							s.brickObjects.at(i).exists = false;
							s.score+=s.brickObjects.at(i).breakScore;
							s.brickCount--;

							if (s.brickCount == 0) {
								if (level == 3) {
								 //won
									level = 1;
							
								}
								else {
									//new level
									level++;
									Contact contact3;
									contact3.type = CollisionType::ExitedScreen;
									ball.CollideWith(contact3);
									s.SetNewLevel(level,renderer);
									
								}
							}
						}
					}
				}
			}
		}
#pragma endregion BrickCollisions	

#pragma endregion Collisions
		auto stopTime = high_resolution_clock::now();
		dt = duration<float, milliseconds::period>(stopTime - startTime).count();
	
	}


}

