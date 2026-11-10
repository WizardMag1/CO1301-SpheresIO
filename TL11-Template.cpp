// TL11-Template.cpp: A program using the TL-Engine

#include "TL-Engine11.h"
#include <iostream>
#include <sstream>
using namespace tle;
using namespace std;

// Speed and world size constants (Brief 1c, 3b, 5)
const float kSphereSpeed = 55.0f;
const float kEnemySpeed = 35.0f;
const float kRotationSpeed = 70.0f;
const float kCameraSpeed = 80.0f;

// 3x bigger world: island is scaled 3x so borders are -300 to +300 (600x600 units)
const float kIslandBorder = 300.0f;
const int kTotalItems = 36;
const int kTotalEnemies = 5;

// 4 Game states (Brief 3d: Playing, Paused, GameWon, GameOver)
enum EGameState { Playing, Paused, GameWon, GameOver };

// 3 Camera states: 1 = TopDown, 2 = Isometric, 3 = Close-Up Follow
enum ECameraState { TopDown, Isometric, CloseView };

// Give an item a random colour skin (Lab 3B Section 5)
void SetRandomCubeColour(Model* item)
{
	string skins[8] = {
		"minicube.jpg", "tiles1.jpg", "RedBall.jpg", "YellowBall.jpg",
		"Baize.jpg", "Earth.jpg", "Jupiter.jpg", "Mars.jpg"
	};
	int randomIndex = rand() % 8;
	item->SetSkin(skins[randomIndex]);
}

// Spawn 1 item at a safe random floor position (-260 to +260) at least 10 units from others (Brief 4a & 4b)
void SpawnCube(Model* items[], int currentItem, int totalSpawned, Model* player, float sphereRadius)
{
	bool safePosition = false;

	while (safePosition == false)
	{
		float randomX = (float)((rand() % 521) - 260);
		float randomZ = (float)((rand() % 521) - 260);
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

// Spawn enemy sphere far away from the player (Brief 5a.ii)
void SpawnEnemy(Model* enemy, Model* player)
{
	bool farEnough = false;

	while (farEnough == false)
	{
		float randomX = (float)((rand() % 501) - 250);
		float randomZ = (float)((rand() % 501) - 250);

		float vecX = randomX - player->GetX();
		float vecZ = randomZ - player->GetZ();
		float distance = sqrt(vecX * vecX + vecZ * vecZ);

		if (distance > 120.0f)
		{
			enemy->SetPosition(randomX, 10.0f, randomZ);
			farEnough = true;
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
		camera->SetPosition(350, 350, -350);
		camera->ResetOrientation();
		camera->RotateLocalY(-45);
		camera->RotateLocalX(45);
		cameraState = Isometric;
	}
	if (myEngine->KeyHit(Key_1))
	{
		camera->SetPosition(0, 500, 0);
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
		camera->SetPosition(player->GetX(), 65, player->GetZ() - 90);
		camera->ResetOrientation();
		camera->RotateLocalX(32);
	}
}

// Check if player stepped on water outside the 600x600 island (-300 to +300) (Brief 1b.v)
void CheckWater(Model* player, EGameState& gameState)
{
	float playerX = player->GetX();
	float playerZ = player->GetZ();

	if (playerX > kIslandBorder)
	{
		gameState = GameOver;
	}
	if (playerX < -kIslandBorder)
	{
		gameState = GameOver;
	}
	if (playerZ > kIslandBorder)
	{
		gameState = GameOver;
	}
	if (playerZ < -kIslandBorder)
	{
		gameState = GameOver;
	}
}

// Sphere-to-box collision detection function (Brief 3b & Lab 4B Section 10)
bool SphereToBox(Model* sphere, Model* item, float sphereRadius)
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

	float sphereX = sphere->GetX();
	float sphereZ = sphere->GetZ();

	if (sphereX > boxMinX)
	{
		if (sphereX < boxMaxX)
		{
			if (sphereZ > boxMinZ)
			{
				if (sphereZ < boxMaxZ)
				{
					return true;
				}
			}
		}
	}
	return false;
}

// Magnetically pull item toward player within 80 units (Brief 4c & Lab 4A Direction Vector)
void PullCubeToPlayer(Model* player, Model* item, float frameTime)
{
	if (item->GetY() < 0)
	{
		return;
	}

	float vectorX = player->GetX() - item->GetX();
	float vectorZ = player->GetZ() - item->GetZ();
	float distance = sqrt(vectorX * vectorX + vectorZ * vectorZ);

	if (distance <= 80.0f)
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

// Check if player reaches either win goal: 500 points OR 5 swallowed opponents
void CheckWinGoal(int playerPoints, int enemiesSwallowed, EGameState& gameState)
{
	if (playerPoints >= 500)
	{
		gameState = GameWon;
	}
	if (enemiesSwallowed >= 5)
	{
		gameState = GameWon;
	}
}

// Player picks up items, grows every 80 points, activates Hyper Mode, checks 500-point win goal
void CheckCubePickup(Model* player, Model* items[], int currentItem, int& playerPoints, int enemiesSwallowed, float& sphereRadius, bool& hyperMode, float& hyperTimer, EGameState& gameState)
{
	if (SphereToBox(player, items[currentItem], sphereRadius) == true)
	{
		SpawnCube(items, currentItem, kTotalItems, player, sphereRadius);
		SetRandomCubeColour(items[currentItem]);

		playerPoints = playerPoints + 10;

		// Grow sphere 1.2x every 80 points (8 items)
		if (playerPoints == 80)
		{
			player->Scale(1.2f);
			sphereRadius = sphereRadius * 1.2f;
			player->SetY(sphereRadius);
			player->SetSkin("Earth.jpg");
		}
		if (playerPoints == 160)
		{
			player->Scale(1.2f);
			sphereRadius = sphereRadius * 1.2f;
			player->SetY(sphereRadius);
			player->SetSkin("Jupiter.jpg");
		}
		if (playerPoints == 240)
		{
			player->Scale(1.2f);
			sphereRadius = sphereRadius * 1.2f;
			player->SetY(sphereRadius);
			player->SetSkin("hypersphere.jpg");
			hyperMode = true;
			hyperTimer = 8.0f;
		}
		if (playerPoints == 320)
		{
			player->Scale(1.2f);
			sphereRadius = sphereRadius * 1.2f;
			player->SetY(sphereRadius);
			player->SetSkin("Sun.jpg");
			hyperMode = true;
			hyperTimer = 8.0f;
		}
		if (playerPoints == 400)
		{
			player->Scale(1.2f);
			sphereRadius = sphereRadius * 1.2f;
			player->SetY(sphereRadius);
			hyperMode = true;
			hyperTimer = 8.0f;
		}

		CheckWinGoal(playerPoints, enemiesSwallowed, gameState);
	}
}

// Move 1 enemy NPC automatically toward the closest item on the floor (Brief 5a.iii & 5b.i)
void MoveEnemy(Model* enemy, bool enemyActive, Model* items[], float& enemyRadius, int& enemyPoints, Model* player, float sphereRadius, EGameState& gameState, float frameTime)
{
	if (enemyActive == false)
	{
		return;
	}

	// Find closest item on floor using vector distance (Lab 4A)
	int closestItem = 0;
	float shortestDist = 99999.0f;

	for (int itemNumber = 0; itemNumber < kTotalItems; itemNumber++)
	{
		float vecX = items[itemNumber]->GetX() - enemy->GetX();
		float vecZ = items[itemNumber]->GetZ() - enemy->GetZ();
		float dist = sqrt(vecX * vecX + vecZ * vecZ);

		if (dist < shortestDist)
		{
			shortestDist = dist;
			closestItem = itemNumber;
		}
	}

	// Move enemy along normalized direction vector toward closest item
	if (shortestDist > 0.1f)
	{
		float dirX = (items[closestItem]->GetX() - enemy->GetX()) / shortestDist;
		float dirZ = (items[closestItem]->GetZ() - enemy->GetZ()) / shortestDist;
		enemy->MoveX(dirX * kEnemySpeed * frameTime);
		enemy->MoveZ(dirZ * kEnemySpeed * frameTime);
	}

	// Check if enemy picks up an item (Brief 5a.iii)
	for (int itemNumber = 0; itemNumber < kTotalItems; itemNumber++)
	{
		if (SphereToBox(enemy, items[itemNumber], enemyRadius) == true)
		{
			SpawnCube(items, itemNumber, kTotalItems, player, sphereRadius);
			SetRandomCubeColour(items[itemNumber]);
			enemyPoints = enemyPoints + 10;

			if (enemyPoints == 80)
			{
				enemy->Scale(1.2f);
				enemyRadius = enemyRadius * 1.2f;
				enemy->SetY(enemyRadius);
			}
			if (enemyPoints == 160)
			{
				enemy->Scale(1.2f);
				enemyRadius = enemyRadius * 1.2f;
				enemy->SetY(enemyRadius);
			}
		}
	}

	// If enemy reaches 500 points before player, GameOver (Brief 5b)
	if (enemyPoints >= 500)
	{
		gameState = GameOver;
	}
}

// Sphere-to-sphere collision between Player and Enemy NPC (Brief 5a.v & 5b)
void CheckEnemyCollision(Model* player, float& playerRadius, int& playerPoints, int& enemiesSwallowed, Model* enemy, bool& enemyActive, float enemyRadius, int& enemyPoints, EGameState& gameState)
{
	if (enemyActive == false)
	{
		return;
	}

	float vecX = enemy->GetX() - player->GetX();
	float vecZ = enemy->GetZ() - player->GetZ();
	float distance = sqrt(vecX * vecX + vecZ * vecZ);

	if (distance <= (playerRadius + enemyRadius))
	{
		int pointDiff = playerPoints - enemyPoints;

		// Player has > 40 more points than enemy -> Swallow enemy! (+40 points, grow 1.2x) (Brief 5a.v)
		if (pointDiff > 40)
		{
			enemyActive = false;
			enemy->SetY(-200);
			playerPoints = playerPoints + 40;
			enemiesSwallowed = enemiesSwallowed + 1;

			player->Scale(1.2f);
			playerRadius = playerRadius * 1.2f;
			player->SetY(playerRadius);

			CheckWinGoal(playerPoints, enemiesSwallowed, gameState);
		}
		// Enemy has > 40 more points than player -> Enemy swallows player! (Brief 5b)
		else if (pointDiff < -40)
		{
			player->SetY(-200);
			enemyPoints = enemyPoints + 40;
			enemy->Scale(1.2f);
			gameState = GameOver;
		}
		// Difference is 40 or less -> Bounce off each other! (Brief 5a.v)
		else
		{
			if (distance > 0.1f)
			{
				float pushX = (vecX / distance) * 15.0f;
				float pushZ = (vecZ / distance) * 15.0f;
				player->MoveX(-pushX);
				player->MoveZ(-pushZ);
				enemy->MoveX(pushX);
				enemy->MoveZ(pushZ);
			}
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

// Draw Leaderboard (Highest Score First), Swallowed Count, and Win/Lose text (Brief 2b, 3c, 5c)
void DrawMessages(TLEngine* myEngine, Font* myFont, int playerPoints, int enemyPoints, int enemiesSwallowed, bool hyperMode, float hyperTimer, EGameState gameState)
{
	int rightX = myEngine->GetWidth() - 10;

	// List points of Player and NPC, highest score first, aligned right (Brief 5c)
	stringstream firstLine;
	stringstream secondLine;

	if (playerPoints >= enemyPoints)
	{
		firstLine << "1st Player Points: " << playerPoints << " / 500";
		secondLine << "2nd Enemy Points: " << enemyPoints;
	}
	else
	{
		firstLine << "1st Enemy Points: " << enemyPoints;
		secondLine << "2nd Player Points: " << playerPoints << " / 500";
	}

	myFont->Draw(firstLine.str(), rightX, 0, Black, RightAlign);
	myFont->Draw(secondLine.str(), rightX, 40, Black, RightAlign);

	// Show Goal 2: Swallowed Opponents (X / 5)
	stringstream swallowText;
	swallowText << "Opponents Swallowed: " << enemiesSwallowed << " / 5";
	myFont->Draw(swallowText.str(), rightX, 80, Blue, RightAlign);

	myFont->Draw("Keys: 1=Top  2=Iso  3=Close-Up", 10, 0, Black, LeftAlign);

	if (hyperMode == true)
	{
		stringstream hyperText;
		hyperText << "HYPER MAGNET ON: " << (int)hyperTimer + 1 << "s";
		myFont->Draw(hyperText.str(), 10, 40, Magenta, LeftAlign);
	}

	int centerX = myEngine->GetWidth() / 2;
	int centerY = myEngine->GetHeight() / 2;

	if (gameState == GameWon)
	{
		myFont->Draw("Congrats, You WON! (Goal Reached)", centerX, centerY, Blue, CentreAlign, MiddleAlign);
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
	Font* myFont = myEngine->LoadFont("Arial", 32);

	// Load stage meshes
	Mesh* waterMesh  = myEngine->LoadMesh("water.x");
	Mesh* islandMesh = myEngine->LoadMesh("island.x");
	Mesh* skyMesh    = myEngine->LoadMesh("sky.x");
	Mesh* sphereMesh = myEngine->LoadMesh("spheremesh.x");

	// Load 4 collectible 3D meshes (Cubes, Teapots, Torus Rings, Coins)
	Mesh* cubeMesh   = myEngine->LoadMesh("minicube.x");
	Mesh* teapotMesh = myEngine->LoadMesh("Teapot.x");
	Mesh* torusMesh  = myEngine->LoadMesh("Torus.x");
	Mesh* coinMesh   = myEngine->LoadMesh("TwoPence.x");

	// Create stage models and scale island 3x bigger (600 x 600 world!)
	Model* water  = waterMesh->CreateModel(0, -5, 0);
	Model* island = islandMesh->CreateModel(0, -5, 0);
	Model* sky    = skyMesh->CreateModel(0, -960, 0);
	Model* player = sphereMesh->CreateModel(0, 10, 0);

	island->ScaleX(3.0f);
	island->ScaleZ(3.0f);
	water->ScaleX(3.0f);
	water->ScaleZ(3.0f);

	island->SetSkin("metalgrid2_albedo.png");
	water->SetSkin("Stars.jpg");
	player->SetSkin("regularsphere.jpg");

	float sphereRadius = 10.0f;

	// Create 36 collectible models across the huge 600x600 island
	Model* items[kTotalItems];
	for (int itemNumber = 0; itemNumber < kTotalItems; itemNumber++)
	{
		if (itemNumber < 18)
		{
			items[itemNumber] = cubeMesh->CreateModel(0, 2.5f, 0);
		}
		else if (itemNumber < 24)
		{
			items[itemNumber] = teapotMesh->CreateModel(0, 2.5f, 0);
			items[itemNumber]->Scale(0.4f);
		}
		else if (itemNumber < 30)
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

	// Create 5 Enemy NPC Spheres with enemysphere.jpg skin at random positions far from player (Brief 5a)
	Model* enemies[kTotalEnemies];
	bool enemyActive[kTotalEnemies];
	float enemyRadius[kTotalEnemies];
	int enemyPoints[kTotalEnemies];

	for (int enemyNum = 0; enemyNum < kTotalEnemies; enemyNum++)
	{
		enemies[enemyNum] = sphereMesh->CreateModel(0, 10, 0);
		enemies[enemyNum]->SetSkin("enemysphere.jpg");
		SpawnEnemy(enemies[enemyNum], player);
		enemyActive[enemyNum] = true;
		enemyRadius[enemyNum] = 10.0f;
		enemyPoints[enemyNum] = 0;
	}

	// Create special HyperCube with hypercube.jpg skin (Brief 4c.i)
	Model* hyperCube = cubeMesh->CreateModel(60, 2.5f, 60);
	hyperCube->SetSkin("hypercube.jpg");

	// Create camera starting in CloseView
	Camera* camera = myEngine->CreateCamera(ManualCamera, 0, 65, -90);
	camera->RotateLocalX(32);

	EGameState gameState = Playing;
	ECameraState cameraState = CloseView;
	int playerPoints = 0;
	int enemiesSwallowed = 0;
	bool hyperMode = false;
	float hyperTimer = 0.0f;

	// Game loop
	while (myEngine->IsRunning())
	{
		myEngine->DrawScene();
		float frameTime = myEngine->FrameTime();

		// Find highest enemy score for leaderboard display (Brief 5c)
		int highestEnemyPoints = 0;
		for (int enemyNum = 0; enemyNum < kTotalEnemies; enemyNum++)
		{
			if (enemyPoints[enemyNum] > highestEnemyPoints)
			{
				highestEnemyPoints = enemyPoints[enemyNum];
			}
		}

		// Draw leaderboard, swallowed counter, and win/lose text
		DrawMessages(myEngine, myFont, playerPoints, highestEnemyPoints, enemiesSwallowed, hyperMode, hyperTimer, gameState);

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

		// Enemy spheres continue moving in Playing AND GameOver state (Brief 5b.i)
		if (gameState == Playing)
		{
			for (int enemyNum = 0; enemyNum < kTotalEnemies; enemyNum++)
			{
				MoveEnemy(enemies[enemyNum], enemyActive[enemyNum], items, enemyRadius[enemyNum], enemyPoints[enemyNum], player, sphereRadius, gameState, frameTime);
			}
		}
		if (gameState == GameOver)
		{
			for (int enemyNum = 0; enemyNum < kTotalEnemies; enemyNum++)
			{
				MoveEnemy(enemies[enemyNum], enemyActive[enemyNum], items, enemyRadius[enemyNum], enemyPoints[enemyNum], player, sphereRadius, gameState, frameTime);
			}
		}

		// Player only moves and checks pickups when Playing
		if (gameState == Playing)
		{
			SwitchCamera(myEngine, camera, cameraState);
			MovePlayer(myEngine, player, frameTime);
			MoveCamera(myEngine, camera, player, cameraState, frameTime);
			CheckWater(player, gameState);

			CheckHyperCube(player, hyperCube, sphereRadius, hyperMode, hyperTimer);
			UpdateHyperMode(player, items, hyperMode, hyperTimer, frameTime);

			for (int itemNumber = 0; itemNumber < kTotalItems; itemNumber++)
			{
				items[itemNumber]->RotateY(40.0f * frameTime);
				CheckCubePickup(player, items, itemNumber, playerPoints, enemiesSwallowed, sphereRadius, hyperMode, hyperTimer, gameState);
			}

			for (int enemyNum = 0; enemyNum < kTotalEnemies; enemyNum++)
			{
				CheckEnemyCollision(player, sphereRadius, playerPoints, enemiesSwallowed, enemies[enemyNum], enemyActive[enemyNum], enemyRadius[enemyNum], enemyPoints[enemyNum], gameState);
			}
		}
	}

	// Stop engine
	myEngine->Delete();
}
