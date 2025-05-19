#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define FONT_SIZE 20

struct Vector2
{
	float x;
	float y;
};

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	void WriteScore(string score, int x, int y);

	SDL_Window *mWindow;
	SDL_Renderer *mRenderer;
	TTF_Font *font;
	SDL_Rect score_board; // a rectangle, with the origin at the upper left
	string score1, score2;
	int l_s, r_s;
	Uint32 mTicksCount;

	bool mIsRunning;
	int mPaddleDir;
	Vector2 mPaddlePos;
	Vector2 mBallPos;
	Vector2 mBallVel;
	int mColor;
	int mTime;
	Mix_Music *backMusic;
	Mix_Chunk *soundEffect;
};
