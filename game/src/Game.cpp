#include "Game.h"
#include <string>

Game::Game()
{

}

Game::~Game()
{

}

bool Game::Initialise()
{
	float per = 42.75f;
	float playScreenW = GetScreenWidth() / per; //Play area width.
	float playScreenH = GetScreenHeight() / per; //Play area height.

	playerClear.Radius = 10.0f;
	playerClear.Enabled = false;
	Comm = new CrossCom();
	ThePlayer = new Player(playScreenW, playScreenH, TheColor);
	TheUFOControl = new UFOControl(playScreenW, playScreenH, ThePlayer, Comm, TheColor);
	TheRockControl = new RockControl(playScreenW, playScreenH, ThePlayer, TheUFOControl->TheUFO, Comm, TheColor);
	TheWedgeControl = new WedgeControl(playScreenW, playScreenH, ThePlayer, TheUFOControl->TheUFO, Comm, TheColor);

	TheHighscores = new HighScore(TheColor);
	TheHighscores->Load();
	ThePlayer->HighScore = TheHighscores->TheHighScore;
	BackgroundTimer = new Timer();

	return false;
}

bool Game::Load()
{
	string rockOne = "Models/RockOne.vec";
	string rockTwo = "Models/RockTwo.vec";
	string rockThree = "Models/RockThree.vec";
	string rockFour = "Models/RockFour.vec";
	LineModel dotModel;
	dotModel.LoadModel("Models/Dot.vec");

	Sound fireS = LoadSound("sounds/playerfire.wav");
	Sound thrustS = LoadSound("sounds/thrust2.wav");
	Sound playerExpS = LoadSound("sounds/PlayerExplode.wav");
	Sound playerBonusS = LoadSound("sounds/BonusShip.wav");
	Sound rockExpS = LoadSound("sounds/RockExplosion.wav");
	Sound ufoExpS = LoadSound("sounds/UFOExplosion.wav");
	Sound ufoBigS = LoadSound("sounds/UFOLarge.wav");
	Sound ufoSmallS = LoadSound("sounds/UFOSmall.wav");
	Sound ufoFire = LoadSound("sounds/UFOFire.wav");
	Sound playerShieldOn = LoadSound("sounds/PlayerShieldOn.wav");
	Sound playerShieldHit = LoadSound("sounds/PlayerShieldHit.wav");
	Sound playerSpawn = LoadSound("sounds/PlayerSpawn.wav");
	Sound wedgeExplode = LoadSound("sounds/WedgeExplode.wav");
	Sound wedgeGroupSpawn = LoadSound("sounds/WedgeGroupSpawn.wav");
	BackgroundHighSound = LoadSound("sounds/BackgroundHigh.wav");
	BackgroundLowSound = LoadSound("sounds/BackgroundLow.wav");
	SetSoundVolume(BackgroundHighSound, 0.15f);
	SetSoundVolume(BackgroundLowSound, 0.15f);

	ThePlayer->LoadModel("Models/PlayerShip.vec", "Models/PlayerFlame.vec",
		"Models/PlayerShield.vec", dotModel.GetModel());
	ThePlayer->LoadSound(fireS, thrustS, playerExpS, playerBonusS, playerShieldHit, playerShieldOn, playerSpawn);
	TheRockControl->LoadModel(rockOne, rockTwo, rockThree, rockFour, dotModel.GetModel());
	TheRockControl->LoadSound(rockExpS);
	TheUFOControl->LoadModel("Models/UFO.vec", dotModel.GetModel());
	TheWedgeControl->LoadModel("Models/Wedge.vec");
	TheWedgeControl->LoadSound(wedgeExplode, wedgeGroupSpawn);
	TheUFOControl->LoadSound(ufoExpS, ufoBigS, ufoSmallS, ufoFire);

	return 0;
}

bool Game::BeginRun()
{
	ThePlayer->Initialize();
	TheUFOControl->Initialize();
	TheRockControl->Initialize();
	TheRockControl->NewGame();
	TheWedgeControl->Initialise();

	for (int i = 0; i < 4; i++)
	{
		PlayerShips.push_back(new LineModel());
		PlayerShips[PlayerShips.size() - 1]->SetModel(ThePlayer->GetModel());
		PlayerShips[i]->Scale = ThePlayer->Scale;
		PlayerShips[i]->Enabled = false;
		PlayerShips[i]->ModelColor = TheColor;
	}

	return false;
}

void Game::ProcessInput()
{
	if (ThePlayer->Enabled)
	{
		ThePlayer->Input();
	}
	else if (IsKeyPressed(KEY_N))
	{
		ThePlayer->NewGame();
		TheRockControl->NewGame();
		TheUFOControl->NewGame();
		PlayerShipDisplay();
		TheHighscores->GameOver = false;
	}

	if (IsKeyPressed(KEY_P) && !ThePlayer->GameOver)
	{
		ThePlayer->Paused = !ThePlayer->Paused;
	}

	if (IsKeyPressed(KEY_D) && !ThePlayer->GameOver)
	{
		ThePlayer->Debug = true;
		TheUFOControl->TheUFO->Debug = true;
		TheRockControl->Debug = true;

		for (auto rock : TheRockControl->Rocks)
		{
			rock->Debug = true;
		}

		for (auto wedgepairs : TheWedgeControl->TheWedgeGroup->WedgePairs)
		{
			wedgepairs->SetDebug();
		}

		TheWedgeControl->TheWedgeGroup->Debug = true;
	}

	if (IsKeyPressed(KEY_M))
	{
		MuteBackground = !MuteBackground;
	}
}


void Game::Update(float deltaTime)
{
	if (ThePlayer->Paused)
	{
		return;
	}

	if (!TheHighscores->GameOver)
	{
		BackgroundTimer->Update(deltaTime);

		if (!MuteBackground)
		{
			if (Comm->NewWave)
			{
				BackgroundTimer->Set(1);
				BackgroundHighPitch = 1;
				BackgroundLowPitch = 1;
			}

			if (BackgroundTimer->Elapsed())
				PlayBackgroundSound();
		}
	}

	UpdateCamera(&TheCamera);

	ThePlayer->Update(deltaTime);
	TheRockControl->Update(deltaTime);
	TheUFOControl->Update(deltaTime);
	TheWedgeControl->Update(deltaTime);

	for (auto line : ThePlayer->Lines)
	{
		line->Update(deltaTime);
	}

	if (ThePlayer->Enabled)
	{
		playerClear.Enabled = false;

		if (ThePlayer->NewLife)
		{
			ThePlayer->NewLife = false;
			PlayerShipDisplay();
		}
	}
	else
	{
		if (ThePlayer->Exploding)
		{
			bool done = true;

			for (auto line : ThePlayer->Lines)
			{
				if (line->Enabled)
				{
					done = false;
					return;
				}
			}

			if (done)
			{
				ThePlayer->Exploding = false;
			}
		}

		if (ThePlayer->Lives > 0)
		{
			playerClear.Enabled = true;
			TheRockControl->Update(deltaTime);
			TheUFOControl->Update(deltaTime);
			TheWedgeControl->Update(deltaTime);
			CheckPlayerClear();
			PlayerShipDisplay();
		}
		else if (!ThePlayer->GameOver)
		{
			PlayerShipDisplay();
			TheHighscores->TheHighScore = ThePlayer->HighScore;
			TheHighscores->CheckForNewHighScore(ThePlayer->Score);
			ThePlayer->GameOver = true;
			TheHighscores->GameOver = true;
		}
		else
		{
			TheHighscores->Update(deltaTime);
		}
	}
}

void Game::Draw()
{
	BeginDrawing();
	ClearBackground({ 8, 2, 16, 100 });
	BeginMode3D(TheCamera);
	//3D Drawing here.
	ThePlayer->Draw();
	TheRockControl->Draw();
	TheUFOControl->Draw();
	TheWedgeControl->Draw();

	for (auto ship : PlayerShips)
	{
		ship->Draw();
	}

	EndMode3D();
	//2D drawing, fonts go here.
	TheHighscores->Draw();

	if (ThePlayer->Paused)
	{
		DrawText("Paused", (GetScreenWidth() / 2) - 80, (GetScreenHeight() / 2) - 20, 50, TheColor);
	}

	if (ThePlayer->Debug)
	{
		DrawText("Debug Mode Active!", (int)(GetScreenWidth() / 1.1f), (int)(GetScreenHeight() / 40), 10, TheColor);
	}

	DrawText(const_cast<char*>(to_string(ThePlayer->Score).c_str()), 200, 5, 45, TheColor);
	DrawText(const_cast<char*>(to_string(ThePlayer->HighScore).c_str()), GetScreenWidth() / 2, 4, 20, TheColor);
	DrawText("(C) 1980 ATARI INC", (GetScreenWidth() / 2) - 15, GetScreenHeight() - 12, 8, TheColor);

	EndDrawing();
}

void Game::PlayerShipDisplay()
{
	float line = ThePlayer->WindowHeight - ThePlayer->Radius - 2.5f;
	float column = 20.0f;

	if (ThePlayer->Lives > PlayerShips.size())
	{
		PlayerShips.push_back(new LineModel());
		PlayerShips[PlayerShips.size() - 1]->SetModel(ThePlayer->GetModel());
		PlayerShips[PlayerShips.size() - 1]->ModelColor = TheColor;
	}

	for (int i = 0; i < PlayerShips.size(); i++)
	{
		PlayerShips[i]->Y(line);
		PlayerShips[i]->X(column);
		PlayerShips[i]->Rotation = PI / 2;
		PlayerShips[i]->Enabled = false;
		column += 1.125f;
	}

	for (int i = 0; i < ThePlayer->Lives; i++)
	{
		PlayerShips[i]->Enabled = true;
	}
}

void Game::CheckPlayerClear()
{
	bool clear = true;

	for (auto rock : TheRockControl->Rocks)
	{
		if (playerClear.CirclesIntersect(rock))
		{
			clear = false;
		}

		if (playerClear.CirclesIntersect(TheUFOControl->TheUFO))
		{
			clear = false;
		}

		if (playerClear.CirclesIntersect(TheUFOControl->TheUFO->TheShot))
		{
			clear = false;
		}

		if (playerClear.CirclesIntersect(TheWedgeControl->TheWedgeGroup))
		{
			clear = false;
		}

		for (auto wedgePair : TheWedgeControl->TheWedgeGroup->WedgePairs)
		{
			if (playerClear.CirclesIntersect(wedgePair))
			{
				clear = false;
			}

			for (auto wedge : wedgePair->Wedges)
			{
				if (playerClear.CirclesIntersect(wedge))
				{
					clear = false;
				}
			}
		}

		if (TheUFOControl->TheUFO->Enabled)
		{
			clear = false;
		}
	}

	if (clear)
	{
		ThePlayer->Reset();
	}
}

void Game::PlayBackgroundSound()
{
	float pitchChange = 0.005f;
	float pitchMax = 1.5f;

	if (PlayBackgroundHigh)
	{
		if (!IsSoundPlaying(BackgroundLowSound) && !IsSoundPlaying(BackgroundHighSound))
		{
			PlayBackgroundHigh = false;
			PlayBackgroundLow = true;
			PlaySound(BackgroundHighSound);
		}

		if (BackgroundHighPitch < pitchMax)
		{
			BackgroundHighPitch += pitchChange;
			SetSoundPitch(BackgroundHighSound, BackgroundHighPitch);
		}
	}

	if (PlayBackgroundLow)
	{
		if (!IsSoundPlaying(BackgroundHighSound) && !IsSoundPlaying(BackgroundLowSound))
		{
			PlayBackgroundLow = false;
			PlayBackgroundHigh = true;
			PlaySound(BackgroundLowSound);
		}

		if (BackgroundLowPitch < pitchMax)
		{
			BackgroundLowPitch += pitchChange;
			SetSoundPitch(BackgroundLowSound, BackgroundLowPitch);
		}
	}

	if (BackgroundTimer->Amount > 0.5f)
	{
		BackgroundTimer->Amount *= 0.995f;
	}

	BackgroundTimer->Reset();
}


void Game::Shutdown()
{
	CloseWindow();
	CloseAudioDevice();
}