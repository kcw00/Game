
#include "Game.h"


const int thickness = 15;
const float paddleH = 100.0f;  //paddle height

Game::Game()
{
	mWindow=nullptr;
	mRenderer=nullptr;
	font = nullptr;
	l_s = 0;
	r_s = 0;
	mTicksCount=0;
	mIsRunning=true;
	mPaddleDir=0;
	mBallPos.x = 512;
	mBallPos.y = 384;
	mColor = 0;
	mTime = 0;
	backMusic = nullptr;
	soundEffect = nullptr;
	
}

bool Game::Initialize()
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"CMPT 1267", // Window title
		100,	// Top left x-coordinate of window  = my real computer screen
		100,	// Top left y-coordinate of window
		1024,	// Width of window
		768,	// Height of window
		0		// Flags (0 for no flags set)
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	TTF_Init();
	font = TTF_OpenFont("font1.ttf", FONT_SIZE);


	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	backMusic = Mix_LoadMUS("music2.wav");
	Mix_PlayMusic(backMusic, -1);
	soundEffect = Mix_LoadWAV("music1.wav");
	

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	//
	mPaddlePos.x = 10.0f;
	mPaddlePos.y = 768.0f/2.0f;
	mBallPos.x = 1024.0f/2.0f;
	mBallPos.y = 768.0f/2.0f;
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	
	// Update paddle direction based on W/S keys
	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddleDir += 1;
	}
	if (state[SDL_SCANCODE_Z])
	{

		if (Mix_PausedMusic() == 1)
		{
			// resume music
			Mix_ResumeMusic();
		}
		else // if music is playing
		{
			// pause music
			Mix_PauseMusic();
		}
	}

}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();

	// Update paddle position based on direction
	if (mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mPaddlePos.y < (paddleH / 2.0f + thickness))
		{
			mPaddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	// Update ball position based on ball velocity
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	// Bounce if needed
	// Did we intersect with the paddle?
	float diff = mPaddlePos.y - mBallPos.y;
	// Take absolute value of difference
	diff = (diff > 0.0f) ? diff : -diff;
	if (
		// Our y-difference is small enough
		diff <= paddleH / 2.0f &&
		// We are in the correct x-position
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		// The ball is moving to the left
		mBallVel.x < 0.0f)
	{
		mBallVel.x *= -1.0f;

		// when ball hits paddle, play music
		Mix_PlayChannel(-1, soundEffect, 0);

		// when ball hits paddle, add score
		l_s++;

		// when ball hits paddle, change color www
		if (mTime == 1)
		{
			mColor = 0;
		}
		else if (mTime == 0)
		{
			mColor = 1;
		}
	}
	// Did the ball go off the screen? (if so, end game)
	else if (mBallPos.x <= 0.0f)
	{
		mIsRunning = false;
	}
	// Did the ball collide with the right wall?
	else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1.0f;
		mTime = 1;
	}

	// Did the ball collide with the top wall?
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
		mTime = 1;
	}
	// Did the ball collide with the bottom wall?
	else if (mBallPos.y >= (768 - thickness) &&
		mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1;
		mTime = 0;
	}

	score1 = "Score: " + to_string(l_s);
	score2 = "Best Score: " + to_string(r_s);
}

void Game::GenerateOutput()
{
	
	if (mColor == 0)
		SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);
	else if (mColor == 1)
		SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);

	
	// Clear back buffer
	SDL_RenderClear(mRenderer);

	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 0, 255);
	
	// Draw top wall
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw right wall
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw paddle
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);
	
	// Draw ball
	SDL_Rect ball{	
		static_cast<int>(mBallPos.x - thickness/2),
		static_cast<int>(mBallPos.y - thickness/2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	WriteScore(score1, 1024 - FONT_SIZE * 2, FONT_SIZE * 2 + 15 );
	WriteScore(score2, 1024 - FONT_SIZE * 2, FONT_SIZE * 4);

	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::WriteScore(string text, int x, int y)
{

	SDL_Surface* surface;
	SDL_Texture* texture;
	const char* t = text.c_str(); // text 
	SDL_Color color = { 0,0,0 };
	surface = TTF_RenderText_Solid(font, t, color);
	texture = SDL_CreateTextureFromSurface(mRenderer, surface);
	score_board.w = surface->w;
	score_board.h = surface->h;
	score_board.x = x - score_board.w;
	score_board.y = y - score_board.h;
	SDL_FreeSurface(surface);
	SDL_RenderCopy(mRenderer, texture, NULL, &score_board);
	SDL_DestroyTexture(texture);

}
void Game::Shutdown()
{
	// close font
	TTF_CloseFont(font);
	// free the sound effect
	Mix_FreeChunk(soundEffect);
	// free the music
	Mix_FreeMusic(backMusic);
	// Quit SDL_mixer
	Mix_CloseAudio();
	// destroy renderer
	SDL_DestroyRenderer(mRenderer);
	// destroy window
	SDL_DestroyWindow(mWindow);
	// shuts down SDL
	SDL_Quit();
}