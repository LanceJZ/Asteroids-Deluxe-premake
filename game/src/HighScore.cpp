#include "HighScore.h"

HighScore::HighScore(Color color)
{
	highScoretimer = new Timer();
	highScoretimer->Set(7);
	HighScore::color = color;
}

HighScore::~HighScore()
{
}

void HighScore::Input()
{

}

void HighScore::Update(float deltaTime)
{
	if (newHighScore && GameOver)
	{
		NewHighScoreEntry();
	}

	highScoretimer->Update(deltaTime);
}

void HighScore::Draw()
{
	if (GameOver)
	{
		if (newHighScore)
		{
			DrawText("Left/Right",
				(GetScreenWidth() / 2) - 80, (GetScreenHeight() / 2) - 250, 40, color);
			DrawText("arrow keys",
				(GetScreenWidth() / 2) - 90, (GetScreenHeight() / 2) - 200, 40, color);
			DrawText("to change letter,",
				(GetScreenWidth() / 2) - 130, (GetScreenHeight() / 2) - 150, 40, color);
			DrawText("down arrow key",
				(GetScreenWidth() / 2) - 120, (GetScreenHeight() / 2) - 100, 40, color);
			DrawText("to chose letter.",
				(GetScreenWidth() / 2) - 120, (GetScreenHeight() / 2) - 50, 40, color);
			DrawText(const_cast<char*>(highScoreEntryText.c_str()), (GetScreenWidth() / 2) - 30,
				(GetScreenHeight() / 2) + 130, 60, color);
		}
		else
		{
			if (showHighScores)
			{
				DisplayHighScoreList();
			}
			else
			{
				DrawText("GAME OVER", GetScreenWidth() / 2 - 160, GetScreenHeight() / 2 - 60, 60, color);
				DrawText("ONE COIN ONE PLAY", GetScreenWidth() / 2 - 290, GetScreenHeight() / 2 + 60, 60, color);
			}
		}

		if (highScoretimer->Elapsed())
		{
			showHighScores = !showHighScores;
			highScoretimer->Reset();
		}
	}
}

void HighScore::Load()
{
	if (FileExists("HighScoreList"))
	{
		highScoreListRaw = LoadFileText("HighScoreList");
		ConvertRawScoreListToArray();
		TheHighScore = Scores[0].Score;
	}
	else
	{
		MakeNewScoreList();
	}
}

void HighScore::Save()
{
	SaveFileText("HighScoreList", const_cast<char*>(highScoreListRaw.c_str()));
}

void HighScore::MakeNewScoreList()
{
	for (int i = 0; i < 10; i++)
	{
		Scores[i].Name = "AAA";
		Scores[i].Score = 1000;
	}

	ConvertScoreListToString();
	Save();
}

void HighScore::ConvertScoreListToString()
{
	highScoreListRaw = "";

	for (auto score : Scores)
	{
		highScoreListRaw += score.Name;
		highScoreListRaw += ":";
		highScoreListRaw += to_string(score.Score);
		highScoreListRaw += ",";
	}
}

void HighScore::ConvertRawScoreListToArray()
{
	int listNumber = 0;
	bool isLetter = true;
	string number = "";

	for (auto character : highScoreListRaw)
	{
		if (isLetter)
		{
			if (character !=  58) //58 for colon.
			{
				Scores[listNumber].Name.append(1, character);
			}
			else
			{
				isLetter = false;
			}
		}
		else
		{
			if (character != 44) //44 for comma.
			{
				number.append(1, character);
			}
			else
			{
				Scores[listNumber].Score = stoi(number);
				isLetter = true;
				listNumber++;
				number = "";
			}
		}
	}
}

void HighScore::CheckForNewHighScore(int score)
{
	for (int rank = 0; rank < 10; rank++)
	{
		if (score > Scores[rank].Score)
		{
			if (rank < 9)
			{
				ScoreList oldScores[10];

				for (int oldRank = rank; oldRank < 10; oldRank++)
				{
					oldScores[oldRank] = Scores[oldRank];
				}

				for (int newRank = rank; newRank < 9; newRank++)
				{
					Scores[newRank + 1] = oldScores[newRank];
				}
			}

			Scores[rank].Name = "XXX";
			Scores[rank].Score = score;
			highScoreEntryText = "___";
			newHighScoreRank = rank;
			newHighScore = true;

			break;
		}
	}

	while (IsKeyDown(KEY_DOWN || KEY_LEFT || KEY_RIGHT))
	{
		int i = 0;
	}
}

void HighScore::NewHighScoreEntry()
{
	if (IsKeyPressed(KEY_DOWN))
	{
		highScoreSelectedLetter++;

		if (highScoreSelectedLetter > 2)
		{
			Scores[newHighScoreRank].Name = highScoreEntryText;
			newHighScore = false;
			ConvertScoreListToString();
			Save();
			return;
		}
		else
		{
			highScoreEntryText[highScoreSelectedLetter] = (char)65;
		}
	}
	else if (IsKeyPressed(KEY_LEFT))
	{
		highScoreEntryText[highScoreSelectedLetter]--;

		if (highScoreEntryText[highScoreSelectedLetter] < (char)65)
		{
			highScoreEntryText[highScoreSelectedLetter] = (char)90;
		}
	}
	else if (IsKeyPressed(KEY_RIGHT))
	{
		highScoreEntryText[highScoreSelectedLetter]++;

		if (highScoreEntryText[highScoreSelectedLetter] > (char)90)
		{
			highScoreEntryText[highScoreSelectedLetter] = (char)65;
		}
	}
}

void HighScore::DisplayHighScoreList()
{
	string name = "";
	string score = "";
	int space = 30;
	int font = 30;
	int start = 350;
	int center = GetScreenWidth() / 2;

	DrawText("N to start new game.",
		center - 180, (GetScreenHeight() / 2) + 250, 40, color);


	DrawText("High Scores", (int)((center - space) * 2.25f), start - 50, font, color);

	for (int i = 0; i < 10; i++)
	{
		name = Scores[i].Name;
		score = to_string(Scores[i].Score);

		DrawText(const_cast<char*>(name.c_str()), center - (space + 10), start + (space * i), font, color);
		DrawText(const_cast<char*>(score.c_str()), center + (space + 10), start + (space * i), font, color);
	}
}
