// TL11-Template.cpp: A program using the TL-Engine

#include "TL-Engine11.h"
#include <iostream>
#include <sstream>
using namespace tle;
using namespace std;

// Speed constants in units/second for variable timing (Brief 1c & 3b)
const float kSphereSpeed = 40.0f;
const float kRotationSpeed = 60.0f;
const float kCameraSpeed = 50.0f;

// 4 Game states (Brief 3d: Playing, Paused, GameWon, GameOver)
enum EGameState { Playing, Paused, GameWon, GameOver };

// 2 Camera states (Brief 2a & Guide 3)
enum ECameraState { TopDown, Isometric };

// Move player with W/S and rotate with A/D or Left/Right mouse clicks (Brief 1b & 3b)
void MovePlayer(TLEngine* myEngine, Model* player, float frameTime)
{
	if (myEngine->KeyHeld(Key_W))
	{
		player->MoveLocalZ(kSphereSpeed * frameTime);
	}
	if (myEngine->KeyHeld(Key_S))
	{
		player->MoveLocalZ(-kSphereSpeed * frameTime);
	}
	if (myEngine->KeyHeld(Key_D))
	{
		player->RotateY(kRotationSpeed * frameTime);
	}
	if (myEngine->KeyHeld(Key_A))
	{
		player->RotateY(-kRotationSpeed * frameTime);
	}
	if (myEngine->KeyHeld(Mouse_RButton))
	{
		player->RotateY(kRotationSpeed * frameTime);
	}
	if (myEngine->KeyHeld(Mouse_LButton))
	{
		player->RotateY(-kRotationSpeed * frameTime);
	}
}

// Switch between TopDown (Key 1) and Isometric (Key 2) camera (Brief 2a)
void SwitchCamera(TLEngine* myEngine, Camera* camera, ECameraState& cameraState)
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
void MoveCamera(TLEngine* myEngine, Camera* camera, ECameraState cameraState, float frameTime)
{
	if (cameraState == TopDown)
	{
		if (myEngine->KeyHeld(Key_Up))
		{
			camera->MoveZ(kCameraSpeed * frameTime);
		}
		if (myEngine->KeyHeld(Key_Down))
		{
			camera->MoveZ(-kCameraSpeed * frameTime);
		}
		if (myEngine->KeyHeld(Key_Right))
		{
			camera->MoveX(kCameraSpeed * frameTime);
		}
		if (myEngine->KeyHeld(Key_Left))
		{
			camera->MoveX(-kCameraSpeed * frameTime);
		}
	}
}

// Check if player stepped on water outside the 200x200 island (-100 to 100) (Brief 1b.v)
void CheckWater(Model* player, EGameState& gameState)
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

// Sphere-to-box collision detection function (Brief 3b & Lab 4B Section 10)
bool SphereToBox(Model* player, Model* cube, float sphereRadius)
{
	if (cube->GetY() < 0)
	{
		return false;
	}

	float border = 2.5f + sphereRadius;
	float boxMinX = cube->GetX() - border;
	float boxMaxX = cube->GetX() + border;
	float boxMinZ = cube->GetZ() - border;
	float boxMaxZ = cube->GetZ() + border;

	float playerX = player->GetX();
	float playerZ = player->GetZ();

	if (playerX > boxMinX)
	{
		if (playerX < boxMaxX)
		{
			if (playerZ > boxMinZ)
			{
				if (playerZ < boxMaxZ)
				{
					return true;
				}
			}
		}
	}
	return false;
}

// Check pickup for 1 cube, add 10 points, grow every 4 cubes, win at 120 (Brief 3b & 3c)
void CheckCubePickup(Model* player, Model* cube, int& playerPoints, float& sphereRadius, EGameState& gameState)
{
	if (SphereToBox(player, cube, sphereRadius) == true)
	{
		cube->SetY(-100);
		playerPoints = playerPoints + 10;

		if (playerPoints == 40)
		{
			player->Scale(1.2f);
			sphereRadius = sphereRadius * 1.2f;
			player->SetY(sphereRadius);
		}
		if (playerPoints == 80)
		{
			player->Scale(1.2f);
			sphereRadius = sphereRadius * 1.2f;
			player->SetY(sphereRadius);
		}
		if (playerPoints == 120)
		{
			player->Scale(1.2f);
			sphereRadius = sphereRadius * 1.2f;
			player->SetY(sphereRadius);
			gameState = GameWon;
		}
	}
}

// Draw Points at top-right and GameWon / GameOver messages at center (Brief 2b, 3c, 3d)
void DrawMessages(TLEngine* myEngine, Font* myFont, int playerPoints, EGameState gameState)
{
	stringstream outText;
	outText << "Points: " << playerPoints;
	myFont->Draw(outText.str(), myEngine->GetWidth(), 0, Black, RightAlign);

	int centerX = myEngine->GetWidth() / 2;
	int centerY = myEngine->GetHeight() / 2;

	if (gameState == GameWon)
	{
		myFont->Draw("Congrats, You WON", centerX, centerY, Blue, CentreAlign, MiddleAlign);
	}
	if (gameState == GameOver)
	{
		myFont->Draw("Game Over", centerX, centerY, Red, CentreAlign, MiddleAlign);
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

	// Load font (single-word font name for MakeSpriteFont)
	Font* myFont = myEngine->LoadFont("Arial", 36);

	// Load meshes (Basic Layout)
	Mesh* waterMesh  = myEngine->LoadMesh("water.x");
	Mesh* islandMesh = myEngine->LoadMesh("island.x");
	Mesh* skyMesh    = myEngine->LoadMesh("sky.x");
	Mesh* sphereMesh = myEngine->LoadMesh("spheremesh.x");
	Mesh* cubeMesh   = myEngine->LoadMesh("minicube.x");

	// Create models (Basic Layout)
	Model* water  = waterMesh->CreateModel(0, -5, 0);
	Model* island = islandMesh->CreateModel(0, -5, 0);
	Model* sky    = skyMesh->CreateModel(0, -960, 0);
	Model* player = sphereMesh->CreateModel(0, 10, 0);

	// Create array of 12 cubes at random floor positions (Brief 3a)
	Model* cubes[12];
	for (int cubeNumber = 0; cubeNumber < 12; cubeNumber++)
	{
		int randomX = (rand() % 161) - 80;
		int randomZ = (rand() % 161) - 80;
		cubes[cubeNumber] = cubeMesh->CreateModel(randomX, 2.5f, randomZ);
	}

	// Create top-down camera (Basic Layout g)
	Camera* camera = myEngine->CreateCamera(ManualCamera, 0, 200, 0);
	camera->RotateLocalX(90);

	EGameState gameState = Playing;
	ECameraState cameraState = TopDown;
	int playerPoints = 0;
	float sphereRadius = 10.0f;

	// Game loop
	while (myEngine->IsRunning())
	{
		myEngine->DrawScene();
		float frameTime = myEngine->FrameTime();

		// Draw score and win/lose text
		DrawMessages(myEngine, myFont, playerPoints, gameState);

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

		// Only move and check when Playing (Brief 3c, 3d)
		if (gameState == Playing)
		{
			SwitchCamera(myEngine, camera, cameraState);
			MovePlayer(myEngine, player, frameTime);
			MoveCamera(myEngine, camera, cameraState, frameTime);
			CheckWater(player, gameState);

			for (int cubeNumber = 0; cubeNumber < 12; cubeNumber++)
			{
				CheckCubePickup(player, cubes[cubeNumber], playerPoints, sphereRadius, gameState);
			}
		}
	}

	// Stop engine
	myEngine->Delete();
}
