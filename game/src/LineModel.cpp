#include "LineModel.h"
#include "rlgl.h"

void LineModel::Update(float deltaTime)
{
	Entity::Update(deltaTime);
	ModelColor.a = (char)Alpha;
}

void LineModel::Draw()
{
	Entity::Draw();

	if (!Enabled)
	{
		return;
	}

	if (LinePoints.size() < 2)
	{
		return;
	}

	rlPushMatrix();

	if (IsConnectedChild)
	{
		for (auto parent : Parents)
		{
			rlTranslatef(parent->Position.x, parent->Position.y, Position.z);
			rlRotatef(parent->Rotation, RotationAxis.x, RotationAxis.y, RotationAxis.z);
		}
	}

	rlTranslatef(Position.x, Position.y, Position.z);
	rlRotatef(Rotation, RotationAxis.x, RotationAxis.y, RotationAxis.z);
	rlScalef(Scale, Scale, Scale);
	rlBegin(RL_LINES);
	rlColor4ub(ModelColor.r, ModelColor.g, ModelColor.b, ModelColor.a);

	for (int i = 0; i < LinePoints.size() - 1; i++)
	{
		rlVertex3f(LinePoints[i].x, LinePoints[i].y, LinePoints[i].z);
		rlVertex3f(LinePoints[i + 1].x, LinePoints[i + 1].y, LinePoints[i + 1].z);
	}

	rlPopMatrix();
	rlEnd();
}

void LineModel::Load()
{

}

void LineModel::LoadModel(string fileName)
{
	if (FileExists(const_cast<char*>(fileName.c_str())))
	{
		string linestemp = LoadFileText(const_cast<char*>(fileName.c_str()));
		LinePoints = ConvertStringToVector(linestemp);
	}
}

vector<Vector3> LineModel::GetModel()
{
	return LinePoints;
}

void LineModel::SetModel(vector<Vector3> lines)
{
	LineModel::LinePoints = lines;
}

LineModel::LineModel()
{

}

LineModel::~LineModel()
{
}

vector<Vector3> LineModel::ConvertStringToVector(string linesString)
{
	vector<Vector3> linesConverted;
	bool isNumber = false;
	float x = 0;
	float y = 0;
	float z = 0;
	string number;
	string onAxis;
	Vector3 line = { 0 };

	for (auto character : linesString)
	{
		if (character > 44 && character < 58)
		{
			number.append(1, character);
		}
		else
		{
			if (character == 88)
			{
				onAxis = "X";
			}
			else if (character == 89)
			{
				onAxis = "Y";

				line.x = stof(number);
				number = "";
			}
			else if (character == 90)
			{
				onAxis = "Z";

				line.y = stof(number);
				number = "";
			}
			else if (character == 125)
			{
				line.z = stof(number);
				number = "";

				linesConverted.push_back(line);
			}
		}
	}

	return linesConverted;
}