#pragma once
#include "LineModel.h"
#include "Shot.h"
#include "Line.h"
#include <vector>

class Player : public LineModel
{
public:
	bool Paused = false;
	bool NewLife = false;
	bool Exploding = false;
	bool GameOver = true;
	int Wave { 0 };
	int Score { 0 };
	int HighScore{ 0 };
	int Lives { 0 };

	Shot Shots[4];
	vector<Line*> Lines;

	Player();
	~Player();

	void LoadModel(string shipmodel, string flamemodel, string shieldmodel, vector<Vector3> dotModel);
	void LoadSound(Sound fireS, Sound thrustS, Sound exp, Sound bonus, Sound shieldHit, Sound shieldOn,
	Sound spawn);
	virtual void Initialize(float windowWidth, float windowHeight, Color TheColor);
	virtual void Input();
	virtual void Update(float deltaTime);
	virtual void Draw();

	void Hit();
	void ScoreUpdate(int addToScore);
	void NewGame();
	void Reset();
	bool GetShieldIsOn();
	bool ShieldHit(Vector3 hitbyPos, Vector3 hitbyVel);

private:
	bool ThrustIsOff = true;
	bool ThrustIsOn = false;
	int NextNewLifeScore = 10000;
	float ThrustSoundTime = 0;
	float ShieldPower = 100;
	Sound Sound05 = { 0 };
	Sound Sound06 = { 0 };
	Sound Sound07 = { 0 };

	LineModel Flame;
	LineModel Shield;

	void ThrustOn(float deltaTime);
	void ThrustOff(float deltaTime);
	void Fire();
	void ShieldOn();
	void ShieldOff();
	void Gamepad();
	void Keyboard();
};

