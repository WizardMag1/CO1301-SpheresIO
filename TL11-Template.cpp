// TL11-Template.cpp: A program using the TL-Engine

#include "TL-Engine11.h"
#include <iostream>
#include <sstream>
using namespace tle;
using namespace std;

// Speed constants in units/second for variable timing (Brief 1c & 3b)
const float kSphereSpeed = 35.0f;
const float kRotationSpeed = 60.0f;
const float kCameraSpeed = 50.0f;
const int kTotalItems = 24;

// 4 Game states (Brief 3d: Playing, Paused, GameWon, GameOver)
enum EGameState { Playing, Paused, GameWon, GameOver };

// 3 Camera states: 1 = TopDown, 2 = Isometric, 3 = Close-up Follow view to inspect models
enum ECameraState { TopDown, Isometric, CloseView };

// Give an item a random colour skin from an array of skins (Lab 3B Section 5)
void SetRandomCubeColour(Model* item)
{
	string skins[8] = {
		"minicube.jpg", "tiles1.jpg", "RedBall.jpg", "YellowBall.jpg",
		"Baize.jpg", "Earth.jpg", "Jupiter.jpg", "Mars.jpg"
	};
	int randomIndex = rand() % 8;
	item->SetSkin(skins[randomIndex]);
}

// Spawn 1 item at a safe random floor position not within 10 units of other items or sphere (Brief 4a & 4b)
void SpawnCube(Model* items[], int currentItem, int totalSpawned, Model* player, float sphereRadius)
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

		// Check 10-unit distance to other spawned items (Brief 4a)
		for (int otherItem = 0; otherItem < totalSpawned; otherItem++)
		{
			if (otherItem != currentItem)
			{
				float itemVecX = randomX - items[otherItem]->GetX();
				float itemVecZ = randomZ - items[otherItem]->GetZ();
				float itemDist = sqrt(itemVecX * itemVecX + itemVecZ * itemVecZ);

				if (itemDist < 10.0f)
				{
					safePosition = false;
				}
			}
		}

		if (safePosition == true)
		{
			items[currentItem]->SetPosition(randomX, 2.5f, randomZ);
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

// Switch between TopDown (Key 1), Isometric (Key 2), and Close-up View (Key 3) (Brief 2a)
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
	if (myEngine->KeyHit(Key_3))
	{
		cameraState = CloseView;
	}
}

// Move camera with Arrow keys in TopDown, or follow player closely in CloseView (Brief 1d)
void MoveCamera(TLEngine* myEngine, Camera* camera, Model* player, ECameraState cameraState, float frameTime)
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
	if (cameraState == CloseView)
	{
		camera->SetPosition(player->GetX(), 45, player->GetZ() - 55);
		camera->ResetOrientation();
		camera->RotateLocalX(35);
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
bool SphereToBox(Model* player, Model* item, float sphereRadius)
{
	if (item->GetY() < 0)
	{
		return false;
	}

	float border = 2.5f + sphereRadius;
	float boxMinX = item->GetX() - border;
	float boxMaxX = item->GetX() + border;
	float boxMinZ = item->GetZ() - border;
	float boxMaxZ = item->GetZ() + border;

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

// Magnetically pull item toward player within 50 units (Brief 4c & Lab 4A Direction Vector)
void PullCubeToPlayer(Model* player, Model* item, float frameTime)
{
	if (item->GetY() < 0)
	{
		return;
	}

	float vectorX = player->GetX() - item->GetX();
	float vectorZ = player->GetZ() - item->GetZ();
	float distance = sqrt(vectorX * vectorX + vectorZ * vectorZ);

	if (distance <= 50.0f)
	{
		if (distance > 0.1f)
		{
			float directionX = vectorX / distance;
			float directionZ = vectorZ / distance;
			item->MoveX(directionX * kSphereSpeed * 1.5f * frameTime);
			item->MoveZ(directionZ * kSphereSpeed * 1.5f * frameTime);
		}
	}
}

// Longer steps (every 60 points / 6 items) so you have plenty of time to look at the models
void CheckCubePickup(Model* player, Model* items[], int currentItem, int& playerPoints, float& sphereRadius, bool& hyperMode, float& hyperTimer, EGameState& gameState)
{
	if (SphereToBox(player, items[currentItem], sphereRadius) == true)
	{
		// Respawn item at a new separated random position with a new random colour (Brief 4b)
		SpawnCube(items, currentItem, kTotalItems, player, sphereRadius);
		SetRandomCubeColour(items[currentItem]);

		playerPoints = playerPoints + 10;

		// Step 1 (60 points): Grow 1.2x and change sphere colour to Earth
		if (playerPoints == 60)
		{
			player->Scale(1.2f);
			sphereRadius = sphereRadius * 1.2f;
			player->SetY(sphereRadius);
			player->SetSkin("Earth.jpg");
		}

		// Step 2 (120 points): Grow 1.2x and change sphere colour to Jupiter
		if (playerPoints == 120)
		{
			player->Scale(1.2f);
			sphereRadius = sphereRadius * 1.2f;
			player->SetY(sphereRadius);
			player->SetSkin("Jupiter.jpg");
		}

		// Step 3 (180 points): Grow 1.2x, change to hyper skin, and turn ON Magnetic Pull for 8 seconds!
		if (playerPoints == 180)
		{
			player->Scale(1.2f);
			sphereRadius = sphereRadius * 1.2f;
			player->SetY(sphereRadius);
			player->SetSkin("hypersphere.jpg");
			hyperMode = true;
			hyperTimer = 8.0f;
		}

		// Step 4 (240 points): Grow 1.2x, change to Sun skin, and turn ON Magnetic Pull again!
		if (playerPoints == 240)
		{
			player->Scale(1.2f);
			sphereRadius = sphereRadius * 1.2f;
			player->SetY(sphereRadius);
			player->SetSkin("Sun.jpg");
			hyperMode = true;
			hyperTimer = 8.0f;
		}

		// Final Step (300 points): Win the game
		if (playerPoints == 300)
		{
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
		hyperTimer = 8.0f;
		player->SetSkin("hypersphere.jpg");
	}
}

// Update timer for Hyper Mode and pull items magnetically (Brief 4c.ii & 4c.iii)
void UpdateHyperMode(Model* player, Model* items[], bool& hyperMode, float& hyperTimer, float frameTime)
{
	if (hyperMode == true)
	{
		hyperTimer = hyperTimer - frameTime;

		for (int itemNumber = 0; itemNumber < kTotalItems; itemNumber++)
		{
			PullCubeToPlayer(player, items[itemNumber], frameTime);
		}

		if (hyperTimer <= 0.0f)
		{
			hyperMode = false;
			player->SetSkin("regularsphere.jpg");
		}
	}
}

// Draw Points, Current Step, and Camera Hint on screen (Brief 2b, 3c, 3d)
void DrawMessages(TLEngine* myEngine, Font* myFont, int playerPoints, EGameState gameState)
{
	stringstream outText;
	outText << "Points: " << playerPoints << " / 300";
	myFont->Draw(outText.str(), myEngine->GetWidth(), 0, Black, RightAlign);

	myFont->Draw("Keys: 1=Top  2=Iso  3=Close-Up View", 10, 0, Black, LeftAlign);

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

	// Load stage meshes
	Mesh* waterMesh  = myEngine->LoadMesh("water.x");
	Mesh* islandMesh = myEngine->LoadMesh("island.x");
	Mesh* skyMesh    = myEngine->LoadMesh("sky.x");
	Mesh* sphereMesh = myEngine->LoadMesh("spheremesh.x");

	// Load 4 different collectible 3D meshes (Cubes, Teapots, Torus Rings, Coins)
	Mesh* cubeMesh   = myEngine->LoadMesh("minicube.x");
	Mesh* teapotMesh = myEngine->LoadMesh("Teapot.x");
	Mesh* torusMesh  = myEngine->LoadMesh("Torus.x");
	Mesh* coinMesh   = myEngine->LoadMesh("TwoPence.x");

	// Create stage models
	Model* water  = waterMesh->CreateModel(0, -5, 0);
	Model* island = islandMesh->CreateModel(0, -5, 0);
	Model* sky    = skyMesh->CreateModel(0, -960, 0);
	Model* player = sphereMesh->CreateModel(0, 10, 0);

	// Set chessboard / grid style skin on island and dark border on water
	island->SetSkin("metalgrid2_albedo.png");
	water->SetSkin("Stars.jpg");
	player->SetSkin("regularsphere.jpg");

	float sphereRadius = 10.0f;

	// Create 24 collectible models on the island (12 Cubes, 4 Teapots, 4 Torus Rings, 4 Coins)
	Model* items[kTotalItems];
	for (int itemNumber = 0; itemNumber < kTotalItems; itemNumber++)
	{
		if (itemNumber < 12)
		{
			items[itemNumber] = cubeMesh->CreateModel(0, 2.5f, 0);
		}
		else if (itemNumber < 16)
		{
			items[itemNumber] = teapotMesh->CreateModel(0, 2.5f, 0);
			items[itemNumber]->Scale(0.4f);
		}
		else if (itemNumber < 20)
		{
			items[itemNumber] = torusMesh->CreateModel(0, 2.5f, 0);
			items[itemNumber]->Scale(0.2f);
		}
		else
		{
			items[itemNumber] = coinMesh->CreateModel(0, 2.5f, 0);
			items[itemNumber]->Scale(0.25f);
		}

		SpawnCube(items, itemNumber, itemNumber, player, sphereRadius);
		SetRandomCubeColour(items[itemNumber]);
	}

	// Create special HyperCube with hypercube.jpg skin (Brief 4c.i)
	Model* hyperCube = cubeMesh->CreateModel(50, 2.5f, 50);
	hyperCube->SetSkin("hypercube.jpg");

	// Create camera (starts in CloseView so you can clearly see the 3D models right away!)
	Camera* camera = myEngine->CreateCamera(ManualCamera, 0, 45, -55);
	camera->RotateLocalX(35);

	EGameState gameState = Playing;
	ECameraState cameraState = CloseView;
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
			MoveCamera(myEngine, camera, player, cameraState, frameTime);
			CheckWater(player, gameState);

			CheckHyperCube(player, hyperCube, sphereRadius, hyperMode, hyperTimer);
			UpdateHyperMode(player, items, hyperMode, hyperTimer, frameTime);

			// Slowly spin all 24 models so you can see their 3D shapes clearly
			for (int itemNumber = 0; itemNumber < kTotalItems; itemNumber++)
			{
				items[itemNumber]->RotateY(40.0f * frameTime);
				CheckCubePickup(player, items, itemNumber, playerPoints, sphereRadius, hyperMode, hyperTimer, gameState);
			}
		}
	}

	// Stop engine
	myEngine->Delete();
}
