#include "raylib.h"
#include "Game.h"

int main()
{
	Game game;

	int windowHeight = 960; //height
	int windowWidth = 1280; //width

	InitWindow(windowWidth, windowHeight, "Asteroids Deluxe");
	InitAudioDevice();
	SetTargetFPS(60);

	Image icon = LoadImage("icon.png");
	ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
	SetWindowIcon(icon);
	// Define the camera to look into our 3D world
	Camera camera = { 0 };
	camera.position = { 0.0f, 0.0f, -50.0f };  // Camera position
	camera.target = { 0.0f, 0.0f, 0.0f };      // Camera looking at point
	camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	camera.fovy = 45.0f;                       // Camera field-of-view Y
	camera.projection = CAMERA_ORTHOGRAPHIC;   // Camera mode type
	game.TheCamera = camera;

	game.Initialize();
	game.Load();
	game.BeginRun();
	//game.GameLoop();

	while (!WindowShouldClose())
	{
		game.ProcessInput();
		game.Update(GetFrameTime());
		game.Draw();
	}


	game.Shutdown();
}