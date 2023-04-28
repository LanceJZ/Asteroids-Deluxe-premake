#pragma once
#pragma warning(disable:4244)  //Gets rid of "conversion from 'const int' to 'float', possible loss of data"
#include "raylib.h"
#include <vector>
#include "Player.h"
#include "RockControl.h"
#include "UFOControl.h"
#include "WedgeControl.h"
#include "HighScore.h"
#include "CrossCom.h"
using namespace std;

enum GameState
{
	PlayerHit,
	Over,
	InPlay,
	Pause,
	HighScores,
	MainMenu
};

class Game
{
public:
	Game();
	~Game();

	bool Initialize();
	bool Load();
	bool BeginRun();
	void ProcessInput();
	void Update(float deltaTime);
	void Draw();
	void Shutdown();

	Camera TheCamera = { 0 };


private:
	bool MuteBackground = false;
	bool PlayBackgroundHigh = false;
	bool PlayBackgroundLow = true;
	float BackgroundHighPitch = 1;
	float BackgroundLowPitch = 1;
	Color TheColor{ 160, 160, 255, 255 };
	Sound BackgroundHighSound;
	Sound BackgroundLowSound;
	Model PlayerShipModel;
	Entity playerClear;
	Player ThePlayer;
	RockControl TheRockControl;
	UFOControl TheUFOControl;
	WedgeControl TheWedgeControl;
	CrossCom Comm;
	HighScore TheHighscores;
	vector<LineModel*> PlayerShips;

	Timer BackgroundTimer;

	void PlayerShipDisplay();
	void CheckPlayerClear();
	void PlayBackgroundSound();
};

