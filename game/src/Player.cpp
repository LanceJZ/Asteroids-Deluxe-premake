#include "Common.h"
#include "Player.h"
#include "raymath.h"
#include <string>

Player::Player()
{
}

Player::~Player()
{
	UnloadSound(Sound01);
	UnloadSound(Sound02);
	UnloadSound(Sound03);
	UnloadSound(Sound04);
	UnloadSound(Sound05);
	UnloadSound(Sound06);
	UnloadSound(Sound07);

	for (int i = 0; i < Lines.size(); i++)
	{
		delete Lines[i];
	}

	Lines.clear();
}

void Player::LoadModel(string shipmodel, string flamemodel, string shieldmodel, vector<Vector3> dotModel)
{
	LineModel::LoadModel(shipmodel);
	Flame.LoadModel(flamemodel);
	Shield.LoadModel(shieldmodel);

	for (int i = 0; i < 4; i++)
	{
		Shots[i].SetModel(dotModel);
	}
}

void Player::LoadSound(Sound fireS, Sound thrustS, Sound exp, Sound bonus, Sound shieldHit, Sound shieldOn,
	Sound spawn)
{
	Sound01 = fireS;
	Sound02 = thrustS;
	Sound03 = exp;
	Sound04 = bonus;
	Sound05 = shieldHit;
	Sound06 = shieldOn;
	Sound07 = spawn;

	SetSoundVolume(Sound01, 0.25f);
	SetSoundVolume(Sound02, 0.5f);
	SetSoundVolume(Sound03, 0.95f);
	SetSoundVolume(Sound04, 0.75f);
	SetSoundVolume(Sound05, 0.75f);
	SetSoundVolume(Sound06, 0.75f);
	SetSoundVolume(Sound07, 0.75f);
}

void Player::Initialize(float windowWidth, float windowHeight, Color TheColor)
{
	WindowWidth = windowWidth;
	WindowHeight = windowHeight;
	Player::ModelColor = TheColor;
	ModelColor = ModelColor;
	Flame.ModelColor = ModelColor;
	Shield.ModelColor = ModelColor;

	MaxSpeed = 20;
	Radius = 0.6f;
	Enabled = false;
	Shield.Enabled = false;

	Flame.Scale = Scale;
	Flame.Enabled = false;
	Flame.HasCollision = false;
	AddChild(&Flame);
	AddChild(&Shield);

	for (int i = 0; i < 6; i++)
	{
		Lines.push_back(new Line(ModelColor));
	}

	for (int i = 0; i < 4; i++)
	{
		Shots[i].Initialize(windowWidth, windowHeight);
	}
}

void Player::Input()
{
	if (IsGamepadAvailable(0))
	{
		Gamepad();
	}
	else
	{
		Keyboard();
	}
}

void Player::Update(float deltaTime)
{
	PositionedObject::Update(deltaTime);
	Entity::CheckScreenEdge();

	Flame.Update(deltaTime);
	Shield.Update(deltaTime);

	for (auto &shot : Shots)
	{
		shot.Update(deltaTime);
	}

	if (ThrustIsOff)
	{
		ThrustOff(deltaTime);
	}
	else if (ThrustIsOn && Enabled)
	{
		ThrustOn(deltaTime);
	}

	if (Shield.Enabled)
	{
		if (ShieldPower > 0)
		{
			ShieldPower -= 10 * deltaTime;

			if (ShieldPower < 0)
				ShieldPower = 0;
		}
	}
	else
	{
		if (ShieldPower < 100)
		{
			ShieldPower += 1 * deltaTime;
		}
	}
}

void Player::Draw()
{
	LineModel::Draw();
	Flame.Draw();
	Shield.Draw();

	for (auto line : Lines)
	{
		line->Draw();
	}

	for (auto &shot : Shots)
	{
		shot.Draw();
	}
}

void Player::Hit()
{
	StopSound(Sound02);
	PlaySound(Sound03);
	Enabled = false;
	ThrustIsOff = true;
	ThrustIsOn = false;
	Exploding = true;
	Flame.Enabled = false;
	Shield.Enabled = false;
	Acceleration = { 0 };
	Velocity = { 0 };

	for (auto line : Lines)
	{
		line->Spawn(Position);
	}


#ifdef _DEBUG
	if (!Debug)
		Lives--;
#else
	Lives--;
#endif
}

void Player::ScoreUpdate(int addToScore)
{
	Score += addToScore;

	if (Score > HighScore)
	{
		HighScore = Score;
	}

	if (Score > NextNewLifeScore)
	{
		PlaySound(Sound04);
		NextNewLifeScore += 10000;
		Lives++;
		NewLife = true;
	}
}

void Player::NewGame()
{
	Lives = 4;
	NextNewLifeScore = 10000;
	Score = 0;
	Wave = 0;
	GameOver = false;
	Reset();
}

void Player::Reset()
{
	PlaySound(Sound07);

	Position = { 0, 0, 0 };
	Velocity = { 0, 0, 0 };
	Enabled = true;

	for (auto line : Lines)
	{
		line->Clear();
	}
}

void Player::ThrustOn(float deltaTime)
{
	if (!IsSoundPlaying(Sound02))
	{
		PlaySound(Sound02);
	}

	float acceleration = 10.666f;
	float topaccel = 0.05f;
	Acceleration.x = ((cos(Rotation) - (Velocity.x * topaccel)) * acceleration) * deltaTime;
	Acceleration.y = ((sin(Rotation) - (Velocity.y * topaccel)) * acceleration) * deltaTime;
	ThrustIsOff = false;
	Flame.Enabled = true;
}

void Player::ThrustOff(float deltaTime)
{
	StopSound(Sound02);

	float deceleration = 0.5f;
	Acceleration.x = (-Velocity.x * deceleration) * deltaTime;
	Acceleration.y = (-Velocity.y * deceleration) * deltaTime;
	Flame.Enabled = false;
}

void Player::Fire()
{
	float speed = 25.5f;

	for (auto &shot : Shots)
	{
		if (!shot.Enabled)
		{
			PlaySound(Sound01);
			shot.Spawn(Position, VelocityFromAngleZ(speed), 1.5f);
			break;
		}
	}
}

bool Player::ShieldHit(Vector3 hitbyPos, Vector3 hitbyVel)
{
	if (IsSoundPlaying(Sound05))
	{
		StopSound(Sound05);
	}

	if (ShieldPower == 0 || !Shield.Enabled)
	{
		Hit();
		return false;
	}

	PlaySound(Sound05);

	Acceleration = { 0 };
	Velocity.x = (Velocity.x * 0.1f) * -1;
	Velocity.y = (Velocity.y * 0.1f) * -1;
	Velocity.x = hitbyVel.x * 0.95f;
	Velocity.y = hitbyVel.y * 0.95f;
	Vector3 vel = VelocityFromAngleZ(AngleFromVectorsZ(hitbyPos, Position), 4.5f);
	Velocity.x += vel.x;
	Velocity.y += vel.y;

	if (ShieldPower > 20)
	{
		ShieldPower -= 20;
	}
	else
	{
		ShieldPower = 0;
	}

	return true;
}

void Player::ShieldOn()
{
	if (ShieldPower > 0)
	{
		if (!IsSoundPlaying(Sound06))
		{
			PlaySound(Sound06);
		}

		Shield.Enabled = true;
		Shield.Alpha = 2.55f * ShieldPower;
	}
}

void Player::ShieldOff()
{
	StopSound(Sound06);

	Shield.Enabled = false;
}

void Player::Gamepad()
{
	//Button B is 6 for Shield //Button A is 7 for Fire //Button Y is 8 for Hyperspace
	//Button X is 5	//Left bumper is 9 //Right bumper is 11 for Shield //Left Trigger is 10
	//Right Trigger is 12 for Thrust //Dpad Up is 1 for	//Dpad Down is 3 for
	//Dpad Left is 4 for rotate left //Dpad Right is 2 for rotate right
	//Axis 1 is -1 for , 1 for  on left stick.
	//Axis 0 is -1 for Left, 1 for right on left stick.

	if (IsGamepadButtonDown(0, 12))
	{
		ThrustIsOn = true;
		ThrustIsOff = false;
	}
	else
	{
		ThrustIsOn = false;
		ThrustIsOff = true;
	}

	float velocityRotZ = 0.07666f;

	if (IsGamepadButtonDown(0, 4) || GetGamepadAxisMovement(0, 0) < -0.25f)
	{
		Rotation -= velocityRotZ;
	}
	else if (IsGamepadButtonDown(0, 2) || GetGamepadAxisMovement(0, 0) > 0.25f)
	{
		Rotation += velocityRotZ;
	}

	if (IsGamepadButtonPressed(0, 7))
	{
		Fire();
	}

	if (IsGamepadButtonDown(0, 11) || IsGamepadButtonDown(0, 6))
	{
		ShieldOn();
	}
	else
	{
		ShieldOff();
	}
}

void Player::Keyboard()
{
	float velocityRotZ = 0.07666f;

	if (IsKeyDown(KEY_RIGHT))
	{
		Rotation += velocityRotZ;
	}
	else if (IsKeyDown(KEY_LEFT))
	{
		Rotation -= velocityRotZ;
	}

	if (IsKeyDown(KEY_UP))
	{
		ThrustIsOn = true;
		ThrustIsOff = false;
	}
	else
	{
		ThrustIsOn = false;
		ThrustIsOff = true;
	}

	if (IsKeyPressed(KEY_RIGHT_CONTROL) || IsKeyPressed(KEY_LEFT_CONTROL) || IsKeyPressed(KEY_SPACE))
	{
		Fire();
	}

	if (IsKeyDown(KEY_DOWN))
	{
		ShieldOn();
	}
	else
	{
		ShieldOff();
	}
}

bool Player::GetShieldIsOn()
{
	return Shield.Enabled;
}
