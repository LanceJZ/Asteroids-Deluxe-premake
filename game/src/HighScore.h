#pragma once
#include "raylib.h"
#include "Timer.h"
#include <string>
using namespace std;

struct ScoreList
{
	string Name;
	int Score = 0;
};

class HighScore
{
public:
	bool GameOver = true;
	int TheHighScore = { 0 };
	ScoreList Scores[10];
	HighScore(Color color);
	virtual ~HighScore();
	virtual void Input();
	virtual void Update(float deltaTime);
	virtual void Draw();

	void Load();
	void Save();
	void MakeNewScoreList();
	void ConvertScoreListToString();
	void ConvertRawScoreListToArray();
	void CheckForNewHighScore(int score);
	void NewHighScoreEntry();


private:
	bool newHighScore = false;
	bool showHighScores = false;
	int newHighScoreRank = 0;
	int highScoreSelectedLetter = 0;
	Color color = WHITE;

	string highScoreEntryText = "";
	string highScoreListRaw = "";
	Timer* highScoretimer;

	void DisplayHighScoreList();
};

