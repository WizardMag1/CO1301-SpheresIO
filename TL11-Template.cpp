// TL11-Template.cpp: A program using the TL-Engine

#include "TL-Engine11.h"
#include <iostream>
#include <sstream>
using namespace tle;
using namespace std;

// Speed constants (from Brief 1c: kSphereSpeed and kRotationSpeed)
const float kSphereSpeed = 0.2f;
const float kRotationSpeed = 0.2f;
const float kCameraSpeed = 0.2f;

// Game states (from Brief 1e, 3d)
enum EGameState { Playing, Paused, GameOver };

// Camera states (from Brief 2a & Guide 3)
enum ECameraState { TopDown, Isometric };

// Move player with W, S, D, A keys (Brief 1b)
void MovePlayer(TLEngine* myEngine, IModel* player)
{
	if (myEngine->KeyHeld(Key_W))
	{
		player->MoveLocalZ(kSphereSpeed);
	}
	if (myEngine->KeyHeld(Key_S))
	{
		player->MoveLocalZ(-kSphereSpeed);
	}
	if (myEngine->KeyHeld(Key_D))
	{
		player->RotateY(kRotationSpeed);
	}
	if (myEngine->KeyHeld(Key_A))
	{
		player->RotateY(-kRotationSpeed);
	}
}

// Switch between TopDown (Key 1) and Isometric (Key 2) camera (Brief 2a)
void SwitchCamera(TLEngine* myEngine, ICamera* camera, ECameraState& cameraState)
{
	if (myEngine->KeyHit(Key_2))
	{
		camera->SetPosition(150, 150, -150);
		camera->ResetOrientation();
		camera->RotateLocalY(-45);
		camera->RotateLocalX(45);
		cameraState = Isometric;
	}
	if (myEngine->KeyHit(Key_1))
	{
		camera->SetPosition(0, 200, 0);
		camera->ResetOrientation();
		camera->RotateLocalX(90);
		cameraState = TopDown;
	}
}

// Move camera with Arrow keys only in TopDown view (Brief 1d & Guide 3)
void MoveCamera(TLEngine* myEngine, ICamera* camera, ECameraState cameraState)
{
	if (cameraState == TopDown)
	{
		if (myEngine->KeyHeld(Key_Up))
		{
			camera->MoveZ(kCameraSpeed);
		}
		if (myEngine->KeyHeld(Key_Down))
		{
			camera->MoveZ(-kCameraSpeed);
		}
		if (myEngine->KeyHeld(Key_Right))
		{
			camera->MoveX(kCameraSpeed);
		}
		if (myEngine->KeyHeld(Key_Left))
		{
			camera->MoveX(-kCameraSpeed);
		}
	}
}

// Check if player stepped on water outside the 200x200 island (-100 to 100) (Brief 1b.v)
void CheckWater(IModel* player, EGameState& gameState)
{
	float playerX = player->GetX();
	float playerZ = player->GetZ();

	if (playerX > 100)
	{
		gameState = GameOver;
	}
	if (playerX < -100)
	{
		gameState = GameOver;
	}
	if (playerZ > 100)
	{
		gameState = GameOver;
	}
	if (playerZ < -100)
	{
		gameState = GameOver;
	}
}

// Check distance between sphere and 1 cube, add 10 points, grow at 40 points (Brief 1g, 2b, 2c)
void CheckCubePickup(IModel* player, IModel* cube, int& playerPoints)
{
	float vectorX = cube->GetX() - player->GetX();
	float vectorY = cube->GetY() - player->GetY();
	float vectorZ = cube->GetZ() - player->GetZ();

	float distance = sqrt(vectorX * vectorX + vectorY * vectorY + vectorZ * vectorZ);

	if (distance <= 12.5f)
	{
		cube->SetY(-100);
		playerPoints = playerPoints + 10;

		if (playerPoints == 40)
		{
			player->Scale(1.2f);
			player->SetY(12);
		}
	}
}

// Draw Points at top-right of window (Brief 2b & Lab 2B)
void DrawPoints(TLEngine* myEngine, IFont* myFont, int playerPoints)
{
	stringstream outText;
	outText << "Points: " << playerPoints;
	myFont->Draw(outText.str(), myEngine->GetWidth(), 0, kBlack, kRight);
}

int main()
{
	// Start engine and open window
	TLEngine* myEngine = New3DEngine(TL11);
	myEngine->StartWindowed();

	// Media folders
	myEngine->AddMediaFolder("C:\\Users\\Public\\Documents\\TL-Engine11\\Media");
	myEngine->AddMediaFolder("media");

	// Load font for score (Lab 2B)
	IFont* myFont = myEngine->LoadFont("Comic Sans MS", 36);

	// Load meshes (Basic Layout)
	IMesh* waterMesh  = myEngine->LoadMesh("water.x");
	IMesh* islandMesh = myEngine->LoadMesh("island.x");
	IMesh* skyMesh    = myEngine->LoadMesh("sky.x");
	IMesh* sphereMesh = myEngine->LoadMesh("spheremesh.x");
	IMesh* cubeMesh   = myEngine->LoadMesh("minicube.x");

	// Create models (Basic Layout)
	IModel* water  = waterMesh->CreateModel(0, -5, 0);
	IModel* island = islandMesh->CreateModel(0, -5, 0);
	IModel* sky    = skyMesh->CreateModel(0, -960, 0);
	IModel* player = sphereMesh->CreateModel(0, 10, 0);

	// Create 4 cubes (Basic Layout e)
	IModel* cube1 = cubeMesh->CreateModel(-80, 2.5f, 80);
	IModel* cube2 = cubeMesh->CreateModel(80, 2.5f, 80);
	IModel* cube3 = cubeMesh->CreateModel(80, 2.5f, -80);
	IModel* cube4 = cubeMesh->CreateModel(-80, 2.5f, -80);

	// Create top-down camera (Basic Layout g)
	ICamera* camera = myEngine->CreateCamera(kManual, 0, 200, 0);
	camera->RotateLocalX(90);

	EGameState gameState = Playing;
	ECameraState cameraState = TopDown;
	int playerPoints = 0;

	// Game loop
	while (myEngine->IsRunning())
	{
		myEngine->DrawScene();

		// Draw score every frame (Brief 2b)
		DrawPoints(myEngine, myFont, playerPoints);

		// Escape quits the game (Brief 1f)
		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}

		// P pauses and unpauses the game (Brief 1e)
		if (myEngine->KeyHit(Key_P))
		{
			if (gameState == Playing)
			{
				gameState = Paused;
			}
			else if (gameState == Paused)
			{
				gameState = Playing;
			}
		}

		// Only move and check when Playing
		if (gameState == Playing)
		{
			SwitchCamera(myEngine, camera, cameraState);
			MovePlayer(myEngine, player);
			MoveCamera(myEngine, camera, cameraState);
			CheckWater(player, gameState);

			CheckCubePickup(player, cube1, playerPoints);
			CheckCubePickup(player, cube2, playerPoints);
			CheckCubePickup(player, cube3, playerPoints);
			CheckCubePickup(player, cube4, playerPoints);
		}
	}

	// Stop engine
	myEngine->Delete();
}
