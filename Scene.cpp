#include "Scene.h"

void Scene::DrawOnScreen(SDL_Texture* texture, int x, int y)
{
	SDL_QueryTexture(texture, NULL, NULL, &sourceRect.w, &sourceRect.h);
	destinationRect.x = x;	destinationRect.y = y;
	sourceRect.x = 0; sourceRect.y = 0;
	destinationRect.w = sourceRect.w;	destinationRect.h = sourceRect.h;
	SDL_RenderCopy(renderer, texture, &sourceRect, &destinationRect);
	
}

void Scene::Update(int paddleXposition, SDL_Rect ballPosition)
{
	//Background/Paddle/Ball
	DrawOnScreen(backgroundTexture, 0, 0);
	DrawOnScreen(paddleTexture, paddleXposition, fixedPaddleHeight);
	DrawOnScreen(ballTexture, ballPosition.x, ballPosition.y);

	//score 
	textScore = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, ("SCORE:" + to_string(score)).c_str(), { 0xFF, 0xFF, 0xFF, 0xFF }));
	DrawOnScreen(textScore, 800, 719);
	SDL_DestroyTexture(textScore);

	//Lives
	for (auto i = 0U; i <= numberOfLives - 1; i++) {DrawOnScreen(lifeTexture, lifeXposition += 35, 719);}
	lifeXposition = 140;
	DrawOnScreen(textLevel, 20, 719);

	
	//bricks
	
	brickField = IMG_Load("Images/prazno.png");
	for (auto i = 0U; i < brickObjects.size(); i++) {
		if (brickObjects.at(i).exists==true) {
			SDL_BlitSurface(brickSurface[brickObjects.at(i).id], NULL, brickField, &brickObjects.at(i).brickPosition);
		}
	}
	
	brickTexture = SDL_CreateTextureFromSurface(renderer, brickField);
	DrawOnScreen(brickTexture, 0, 0);
	SDL_FreeSurface(brickField);
	SDL_DestroyTexture(brickTexture);
	SDL_RenderPresent(renderer);
}

//Setting the scene on a new level load
void Scene::SetNewLevel(int lvl, SDL_Renderer *r)
{
#pragma region Initializing
	// Initialize PNG loading, SDL_mixer, SDL_tff
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());

	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());

	}
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
#pragma endregion Initializing		

	//load LevelsXML class
	level = lvl;
	renderer = r;
	LevelsXML LevelsXML(level);
	
	//Load empty surface for bliting bricks
	emptySurface = IMG_Load("Images/prazno.png");

	brickObjects.clear();
	// Load texture
	backgroundTexture = IMG_LoadTexture(renderer, LevelsXML.backgroundTexture.c_str());
	paddleTexture = IMG_LoadTexture(renderer, "Images/paddle.png");
	ballTexture = IMG_LoadTexture(renderer, "Images/ball.png");
	lifeTexture = IMG_LoadTexture(renderer, "Images/heart.png");
	
	//Level text
	font = TTF_OpenFont("Font/DejaVuSansMono.ttf", 25);
	textLevel = SDL_CreateTextureFromSurface(renderer,TTF_RenderText_Solid(font, ("LEVEL:" + to_string(level)).c_str(), { 0xFF, 0xFF, 0xFF, 0xFF }));

	
#pragma region Bricks
	// adjusting the center of the screen
	int rowWidth = 0;
	int i = 1;
	
	while (LevelsXML.bricksLayout[i] != '\n') {
		if (LevelsXML.bricksLayout[i] == ' ') {
			rowWidth += LevelsXML.columnSpacing;

		}
		else { rowWidth += 64; }

		i++;
	}
	int x = (1024 - rowWidth) / 2;
	int y = 30;
	int j = 0;

	for (auto i = 0U; i < LevelsXML.bricksLayout.length() - 1; i++) {
		switch (LevelsXML.bricksLayout[i]) {
		case '\n':
			if (i != 0) {
				y = y + brickHeight + LevelsXML.rowSpacing;
				x = (screenWidth - rowWidth) / 2;
			}
			break;
		case ' ': //means space between bricks
			x = x+ LevelsXML.columnSpacing;
			break;
		case '_': //means 'no brick'
			x = x + brickWidth;
			break;
		default:
				Brick b;
				brickObjects.push_back(b); 
				if (string(1, LevelsXML.bricksLayout[i]) != "I") {
				brickObjects.at(j).hitPoints = stoi(LevelsXML.bricksInfo[make_pair(string(1, LevelsXML.bricksLayout[i]), "HitPoints")]);
				brickObjects.at(j).breakSound = Mix_LoadWAV((LevelsXML.bricksInfo[make_pair(string(1, LevelsXML.bricksLayout[i]), "BreakSound")]).c_str());
				brickObjects.at(j).breakScore= stoi(LevelsXML.bricksInfo[make_pair(string(1, LevelsXML.bricksLayout[i]), "BreakScore")]);
				brickCount++;
				}
				brickObjects.at(j).id = LevelsXML.bricksLayout[i];
				brickObjects.at(j).brickPosition.x = x;
				brickObjects.at(j).brickPosition.y = y;
				brickObjects.at(j).hitSound = Mix_LoadWAV((LevelsXML.bricksInfo[make_pair(string(1, LevelsXML.bricksLayout[i]), "HitSound")]).c_str());
				brickSurface[LevelsXML.bricksLayout[i]] = IMG_Load(LevelsXML.bricksInfo[make_pair(string(1, LevelsXML.bricksLayout[i]), "Texture")].c_str());
				brickObjects.at(j).exists = true;

				x = x + 64;
				j++;
			}
		}

#pragma endregion Bricks
}