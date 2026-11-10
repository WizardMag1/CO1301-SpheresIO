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

// Give a cube a random colour skin from an array of skins (Lab 3B Section 5)
void SetRandomCubeColour(Model* cube)
{
	string cubeSkins[5] = { "minicube.jpg", "tiles1.jpg", "RedBall.jpg", "YellowBall.jpg", "Baize.jpg" };
	int randomIndex = rand() % 5;
	cube->SetSkin(cubeSkins[randomIndex]);
}

// Spawn 1 cube at a safe random floor position not within 10 units of other cubes or sphere (Brief 4a & 4b)
void SpawnCube(Model* cubes[], int currentCube, int totalSpawned, Model* player, float sphereRadius)
{
	bool safePosition = false;

	while (safePosition == false)
	{
		float randomX = (float)((rand() % 161) - 80);
		float randomZ = (float)((rand() % 161) - 80);
		safePosition = true;

		// Check distance to player sphere
		float playerVecX = randomX - player->GetX();
		float playerVecZ = randomZ - player->GetZ();
		float playerDist = sqrt(playerVecX * playerVecX + playerVecZ * playerVecZ);

		if (playerDist < (sphereRadius + 10.0f))
		{
			safePosition = false;
		}

		// Check 10-unit distance to other spawned cubes (Brief 4a)
		for (int otherCube = 0; otherCube < totalSpawned; otherCube++)
		{
			if (otherCube != currentCube)
			{
				float cubeVecX = randomX - cubes[otherCube]->GetX();
				float cubeVecZ = randomZ - cubes[otherCube]->GetZ();
				float cubeDist = sqrt(cubeVecX * cubeVecX + cubeVecZ * cubeVecZ);

				if (cubeDist < 10.0f)
				{
					safePosition = false;
				}
			}
		}

		if (safePosition == true)
		{
			cubes[currentCube]->SetPosition(randomX, 2.5f, randomZ);
		}
	}
}

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

// Magnetically pull cube toward player within 50 units (Brief 4c & Lab 4A Direction Vector)
void PullCubeToPlayer(Model* player, Model* cube, float frameTime)
{
	if (cube->GetY() < 0)
	{
		return;
	}

	float vectorX = player->GetX() - cube->GetX();
	float vectorZ = player->GetZ() - cube->GetZ();
	float distance = sqrt(vectorX * vectorX + vectorZ * vectorZ);

	if (distance <= 50.0f)
	{
		if (distance > 0.1f)
		{
			float directionX = vectorX / distance;
			float directionZ = vectorZ / distance;
			cube->MoveX(directionX * kSphereSpeed * 1.5f * frameTime);
			cube->MoveZ(directionZ * kSphereSpeed * 1.5f * frameTime);
		}
	}
}

// Check pickup for 1 cube, respawn it, grow every 4 cubes, activate Hyper Mode after 3 steps (Brief 3b, 3c, 4b, 4c)
void CheckCubePickup(Model* player, Model* cubes[], int currentCube, int& playerPoints, float& sphereRadius, bool& hyperMode, float& hyperTimer, EGameState& gameState)
{
	if (SphereToBox(player, cubes[currentCube], sphereRadius) == true)
	{
		// Respawn cube at a new separated random position with a new random colour (Brief 4b)
		SpawnCube(cubes, currentCube, 12, player, sphereRadius);
		SetRandomCubeColour(cubes[currentCube]);

		playerPoints = playerPoints + 10;

		// At 3 steps (30 points), activate magnetic Hyper Mode and change sphere colour (Brief 4c)
		if (playerPoints == 30)
		{
			hyperMode = true;
			hyperTimer = 5.0f;
			player->SetSkin("hypersphere.jpg");
		}

		// Every 4 cubes (40, 80, 120 points), grow sphere by 1.2x and change sphere colour (Brief 2c & 3b)
		if (playerPoints == 40)
		{
			player->Scale(1.2f);
			sphereRadius = sphereRadius * 1.2f;
			player->SetY(sphereRadius);
			player->SetSkin("Earth.jpg");
		}
		if (playerPoints == 80)
		{
			player->Scale(1.2f);
			sphereRadius = sphereRadius * 1.2f;
			player->SetY(sphereRadius);
			player->SetSkin("Jupiter.jpg");
			hyperMode = true;
			hyperTimer = 5.0f;
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

// Check if player picks up the special hypercube (Brief 4c.i & 4c.ii)
void CheckHyperCube(Model* player, Model* hyperCube, float sphereRadius, bool& hyperMode, float& hyperTimer)
{
	if (SphereToBox(player, hyperCube, sphereRadius) == true)
	{
		hyperCube->SetY(-100);
		hyperMode = true;
		hyperTimer = 5.0f;
		player->SetSkin("hypersphere.jpg");
	}
}

// Update 5-second timer for Hyper Mode and pull cubes magnetically (Brief 4c.ii & 4c.iii)
void UpdateHyperMode(Model* player, Model* cubes[], bool& hyperMode, float& hyperTimer, float frameTime)
{
	if (hyperMode == true)
	{
		hyperTimer = hyperTimer - frameTime;

		for (int cubeNumber = 0; cubeNumber < 12; cubeNumber++)
		{
			PullCubeToPlayer(player, cubes[cubeNumber], frameTime);
		}

		if (hyperTimer <= 0.0f)
		{
			hyperMode = false;
			player->SetSkin("regularsphere.jpg");
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

	// Load font
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

	// Set initial player sphere colour skin (Lab 3A Section 3)
	player->SetSkin("regularsphere.jpg");

	float sphereRadius = 10.0f;

	// Create array of 12 cubes at separated random floor positions with random colours (Brief 3a & 4a)
	Model* cubes[12];
	for (int cubeNumber = 0; cubeNumber < 12; cubeNumber++)
	{
		cubes[cubeNumber] = cubeMesh->CreateModel(0, 2.5f, 0);
		SpawnCube(cubes, cubeNumber, cubeNumber, player, sphereRadius);
		SetRandomCubeColour(cubes[cubeNumber]);
	}

	// Create special HyperCube with hypercube.jpg skin (Brief 4c.i)
	Model* hyperCube = cubeMesh->CreateModel(50, 2.5f, 50);
	hyperCube->SetSkin("hypercube.jpg");

	// Create top-down camera (Basic Layout g)
	Camera* camera = myEngine->CreateCamera(ManualCamera, 0, 200, 0);
	camera->RotateLocalX(90);

	EGameState gameState = Playing;
	ECameraState cameraState = TopDown;
	int playerPoints = 0;
	bool hyperMode = false;
	float hyperTimer = 0.0f;

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

		// Only move and check when Playing
		if (gameState == Playing)
		{
			SwitchCamera(myEngine, camera, cameraState);
			MovePlayer(myEngine, player, frameTime);
			MoveCamera(myEngine, camera, cameraState, frameTime);
			CheckWater(player, gameState);

			CheckHyperCube(player, hyperCube, sphereRadius, hyperMode, hyperTimer);
			UpdateHyperMode(player, cubes, hyperMode, hyperTimer, frameTime);

			for (int cubeNumber = 0; cubeNumber < 12; cubeNumber++)
			{
				CheckCubePickup(player, cubes, cubeNumber, playerPoints, sphereRadius, hyperMode, hyperTimer, gameState);
			}
		}
	}

	// Stop engine
	myEngine->Delete();
}
