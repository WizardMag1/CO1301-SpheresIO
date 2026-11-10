// TL11-Template.cpp: A program using the TL-Engine

#include "TL-Engine11.h"
#include <iostream>
using namespace tle;
using namespace std;

// Speed constants (from Brief 1c: kSphereSpeed and kRotationSpeed)
const float kSphereSpeed = 0.2f;
const float kRotationSpeed = 0.2f;
const float kCameraSpeed = 0.2f;

// Game states (from Brief 3d: Playing, Paused, GameOver)
enum EGameState { Playing, Paused, GameOver };

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

// Move camera with Arrow keys (Brief 1d)
void MoveCamera(TLEngine* myEngine, ICamera* camera)
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

// Check distance between sphere and 1 cube, hide cube if distance <= 12.5 (Brief 1g & Lab 4A/4B)
void CheckCubePickup(IModel* player, IModel* cube)
{
	float vectorX = cube->GetX() - player->GetX();
	float vectorY = cube->GetY() - player->GetY();
	float vectorZ = cube->GetZ() - player->GetZ();

	float distance = sqrt(vectorX * vectorX + vectorY * vectorY + vectorZ * vectorZ);

	if (distance <= 12.5f)
	{
		cube->SetY(-100);
	}
}

int main()
{
	// Start engine and open window
	TLEngine* myEngine = New3DEngine(TL11);
	myEngine->StartWindowed();

	// Media folders
	myEngine->AddMediaFolder("C:\\Users\\Public\\Documents\\TL-Engine11\\Media");
	myEngine->AddMediaFolder("media");

	// Load meshes (Basic Layout)
	IMesh* waterMesh  = myEngine->LoadMesh("water.x");
	IMesh* islandMesh = myEngine->LoadMesh("island.x");
	IMesh* skyMesh    = myEngine->LoadMesh("sky.x");
	IMesh* sphereMesh = myEngine->LoadMesh("spheremesh.x");
	IMesh* cubeMesh   = myEngine->LoadMesh("minicube.x");

	// Create models at exact coordinates from Brief page 5
	IModel* water  = waterMesh->CreateModel(0, -5, 0);
	IModel* island = islandMesh->CreateModel(0, -5, 0);
	IModel* sky    = skyMesh->CreateModel(0, -960, 0);
	IModel* player = sphereMesh->CreateModel(0, 10, 0);

	// Create 4 cubes (Brief Basic Layout e: cube1, cube2, cube3, cube4)
	IModel* cube1 = cubeMesh->CreateModel(-80, 2.5f, 80);
	IModel* cube2 = cubeMesh->CreateModel(80, 2.5f, 80);
	IModel* cube3 = cubeMesh->CreateModel(80, 2.5f, -80);
	IModel* cube4 = cubeMesh->CreateModel(-80, 2.5f, -80);

	// Create top-down camera (Brief Basic Layout g)
	ICamera* camera = myEngine->CreateCamera(kManual, 0, 200, 0);
	camera->RotateLocalX(90);

	EGameState gameState = Playing;

	// Game loop
	while (myEngine->IsRunning())
	{
		myEngine->DrawScene();

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
			MovePlayer(myEngine, player);
			MoveCamera(myEngine, camera);
			CheckWater(player, gameState);

			CheckCubePickup(player, cube1);
			CheckCubePickup(player, cube2);
			CheckCubePickup(player, cube3);
			CheckCubePickup(player, cube4);
		}
	}

	// Stop engine
	myEngine->Delete();
}
