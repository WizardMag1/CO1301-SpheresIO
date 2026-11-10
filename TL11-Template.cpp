// TL11-Template.cpp: A program using the TL-Engine

#include "TL-Engine11.h"
#include <iostream>
#include <sstream>
using namespace tle;
using namespace std;

// Speed constants (Milestone 1c)
const float kSphereSpeed = 55.0f;
const float kRotationSpeed = 70.0f;
const float kCameraSpeed = 80.0f;
const float kEnemySpeed = 35.0f;

// World and spawn range constants (No magic numbers - Brief Page 3)
const float kIslandBorder = 300.0f;
const int kSpawnRange = 500;
const int kSpawnMin = -250;
const float kMinItemSpacing = 10.0f;
const float kMinEnemyStartDist = 120.0f;
const float kCubeHalfWidth = 2.5f;

// Gameplay constants (Points, Growth, Hyper Mode)
const int kTotalItems = 36;
const int kTotalEnemies = 5;
const int kTotalSkins = 8;
const int kPointsPerItem = 10;
const int kPointsPerGrow = 80;
const int kSwallowPointLead = 40;
const int kWinPoints = 500;
const float kGrowScale = 1.2f;
const float kHyperRadius = 80.0f;
const float kHyperSeconds = 8.0f;

// Game states (Milestone 3d)
enum EGameState { Playing, Paused, GameWon, GameOver };

// Camera states (Milestone 2a)
enum ECameraState { TopDown, Isometric, ThirdView };

// Sphere states (Milestone 4c)
enum ESphereState { Regular, Hyper };

// Distance between two (X, Z) points using Pythagoras (Lab 4A Section 3)
float GetDistance(float x1, float z1, float x2, float z2)
{
	float vectorX = x2 - x1;
	float vectorZ = z2 - z1;
	float distance = sqrt(vectorX * vectorX + vectorZ * vectorZ);
	return distance;
}

// Sphere-to-box collision detection (Milestone 3b & Lab 4B Section 10)
// Returns true (YES, touching) or false (NO, not touching)
bool SphereToBox(float sphereX, float sphereZ, float sphereRadius, float boxX, float boxZ)
{
	float border = kCubeHalfWidth + sphereRadius;

	float boxMinX = boxX - border;
	float boxMaxX = boxX + border;
	float boxMinZ = boxZ - border;
	float boxMaxZ = boxZ + border;

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

// Spawn or respawn 1 item at a safe random position >= 10 units away (Milestone 4a & 4b)
void SpawnItem(Model* items[], float itemX[], float itemZ[], string skins[], int currentItem, int totalSpawned, float playerX, float playerZ, float playerRadius)
{
	bool safePosition = false;

	while (safePosition == false)
	{
		// Pick random X and Z between -250 and +250 on the floor
		float randomX = (float)((rand() % kSpawnRange) + kSpawnMin);
		float randomZ = (float)((rand() % kSpawnRange) + kSpawnMin);
		safePosition = true;

		// Rule 1: Must not spawn touching the player sphere (Milestone 4a)
		float distToPlayer = GetDistance(randomX, randomZ, playerX, playerZ);
		if (distToPlayer < (playerRadius + kMinItemSpacing))
		{
			safePosition = false;
		}

		// Rule 2: Must be at least 10 units away from all other items (Milestone 4a)
		for (int other = 0; other < totalSpawned; other++)
		{
			if (other != currentItem)
			{
				float distToItem = GetDistance(randomX, randomZ, itemX[other], itemZ[other]);
				if (distToItem < kMinItemSpacing)
				{
					safePosition = false;
				}
			}
		}

		// If both rules passed, place the item on the floor
		if (safePosition == true)
		{
			itemX[currentItem] = randomX;
			itemZ[currentItem] = randomZ;
			items[currentItem]->SetPosition(randomX, kCubeHalfWidth, randomZ);

			int randomSkin = rand() % kTotalSkins;
			items[currentItem]->SetSkin(skins[randomSkin]);
		}
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

	// Load font (Lab 2B)
	Font* myFont = myEngine->LoadFont("Arial", 32);

	// Load stage meshes (Basic Layout)
	Mesh* waterMesh  = myEngine->LoadMesh("water.x");
	Mesh* islandMesh = myEngine->LoadMesh("island.x");
	Mesh* skyMesh    = myEngine->LoadMesh("sky.x");
	Mesh* sphereMesh = myEngine->LoadMesh("spheremesh.x");

	// Load 4 collectible meshes (Cubes, Teapots, Rings, Coins)
	Mesh* cubeMesh   = myEngine->LoadMesh("minicube.x");
	Mesh* teapotMesh = myEngine->LoadMesh("Teapot.x");
	Mesh* torusMesh  = myEngine->LoadMesh("Torus.x");
	Mesh* coinMesh   = myEngine->LoadMesh("TwoPence.x");

	// Create stage models and scale island 3x bigger (600x600 world)
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

	// Player tracking variables
	float playerX = 0.0f;
	float playerZ = 0.0f;
	float playerRadius = 10.0f;
	int playerPoints = 0;
	int enemiesSwallowed = 0;

	// Array of 8 random colour skins (Lab 3B Section 5)
	string skins[kTotalSkins] = {
		"minicube.jpg", "tiles1.jpg", "RedBall.jpg", "YellowBall.jpg",
		"Baize.jpg", "Earth.jpg", "Jupiter.jpg", "Mars.jpg"
	};

	// Create 36 collectible items and store their X, Z positions in arrays (Milestone 3a & 4a)
	Model* items[kTotalItems];
	float itemX[kTotalItems];
	float itemZ[kTotalItems];

	for (int i = 0; i < kTotalItems; i++)
	{
		if (i < 18)
		{
			items[i] = cubeMesh->CreateModel(0, kCubeHalfWidth, 0);
		}
		else if (i < 24)
		{
			items[i] = teapotMesh->CreateModel(0, kCubeHalfWidth, 0);
			items[i]->Scale(0.4f);
		}
		else if (i < 30)
		{
			items[i] = torusMesh->CreateModel(0, kCubeHalfWidth, 0);
			items[i]->Scale(0.2f);
		}
		else
		{
			items[i] = coinMesh->CreateModel(0, kCubeHalfWidth, 0);
			items[i]->Scale(0.25f);
		}

		SpawnItem(items, itemX, itemZ, skins, i, i, playerX, playerZ, playerRadius);
	}

	// Create special HyperCube at a safe position (Milestone 4c.i)
	Model* hyperCube = cubeMesh->CreateModel(60, kCubeHalfWidth, 60);
	hyperCube->SetSkin("hypercube.jpg");
	float hyperCubeX = 60.0f;
	float hyperCubeZ = 60.0f;
	bool hyperCubeActive = true;

	// Create 5 Enemy NPC Spheres far from player (Milestone 5a)
	Model* enemies[kTotalEnemies];
	float enemyX[kTotalEnemies];
	float enemyZ[kTotalEnemies];
	float enemyRadius[kTotalEnemies];
	int enemyPoints[kTotalEnemies];
	bool enemyActive[kTotalEnemies];

	for (int e = 0; e < kTotalEnemies; e++)
	{
		enemies[e] = sphereMesh->CreateModel(0, 10, 0);
		enemies[e]->SetSkin("enemysphere.jpg");
		enemyRadius[e] = 10.0f;
		enemyPoints[e] = 0;
		enemyActive[e] = true;

		// Spawn enemy at least 120 units away from player (Milestone 5a.ii)
		bool farEnough = false;
		while (farEnough == false)
		{
			float randX = (float)((rand() % kSpawnRange) + kSpawnMin);
			float randZ = (float)((rand() % kSpawnRange) + kSpawnMin);

			if (GetDistance(randX, randZ, playerX, playerZ) > kMinEnemyStartDist)
			{
				enemyX[e] = randX;
				enemyZ[e] = randZ;
				enemies[e]->SetPosition(randX, 10.0f, randZ);
				farEnough = true;
			}
		}
	}

	// Create camera starting in ThirdView (Basic Layout & Milestone 2a)
	Camera* camera = myEngine->CreateCamera(ManualCamera, 0, 65, -90);
	camera->RotateLocalX(32);

	EGameState gameState = Playing;
	ECameraState cameraState = ThirdView;
	ESphereState sphereState = Regular;
	float hyperTimer = 0.0f;

	// Main Game Loop
	while (myEngine->IsRunning())
	{
		myEngine->DrawScene();
		float frameTime = myEngine->FrameTime();
		if (frameTime > 0.1f)
		{
			frameTime = 0.016f;
		}

		// 1. Escape quits the game in all states (Milestone 1f)
		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}

		// 2. P toggles Pause and Playing states (Milestone 1e)
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

		// 3. Find highest enemy score and draw UI text (Milestone 2b, 3c, 5c)
		int bestEnemyPoints = 0;
		for (int e = 0; e < kTotalEnemies; e++)
		{
			if (enemyPoints[e] > bestEnemyPoints)
			{
				bestEnemyPoints = enemyPoints[e];
			}
		}

		int rightX = myEngine->GetWidth() - 10;
		stringstream firstLine;
		stringstream secondLine;

		if (playerPoints >= bestEnemyPoints)
		{
			firstLine << "1st Player Points: " << playerPoints << " / " << kWinPoints;
			secondLine << "2nd Enemy Points: " << bestEnemyPoints;
		}
		else
		{
			firstLine << "1st Enemy Points: " << bestEnemyPoints;
			secondLine << "2nd Player Points: " << playerPoints << " / " << kWinPoints;
		}

		myFont->Draw(firstLine.str(), rightX, 0, Black, RightAlign);
		myFont->Draw(secondLine.str(), rightX, 40, Black, RightAlign);

		stringstream swallowText;
		swallowText << "Opponents Swallowed: " << enemiesSwallowed << " / " << kTotalEnemies;
		myFont->Draw(swallowText.str(), rightX, 80, Blue, RightAlign);

		myFont->Draw("Keys: 1=Top  2=Iso  3=Close-Up", 10, 0, Black, LeftAlign);

		if (sphereState == Hyper)
		{
			stringstream hyperText;
			hyperText << "HYPER MAGNET ON: " << (int)hyperTimer + 1 << "s";
			myFont->Draw(hyperText.str(), 10, 40, Magenta, LeftAlign);
		}

		int centerX = myEngine->GetWidth() / 2;
		int centerY = myEngine->GetHeight() / 2;

		if (gameState == GameWon)
		{
			myFont->Draw("Congrats, You WON!", centerX, centerY, Blue, CentreAlign, MiddleAlign);
		}
		if (gameState == GameOver)
		{
			myFont->Draw("Game Over", centerX, centerY, Red, CentreAlign, MiddleAlign);
		}

		// 4. Enemy spheres move in Playing AND GameOver states (Milestone 5a.iii & 5b.i)
		if (gameState == Playing || gameState == GameOver)
		{
			for (int e = 0; e < kTotalEnemies; e++)
			{
				if (enemyActive[e] == true)
				{
					// Find closest item on the floor (Lab 4A)
					int closest = 0;
					float shortestDist = 99999.0f;

					for (int i = 0; i < kTotalItems; i++)
					{
						float dist = GetDistance(enemyX[e], enemyZ[e], itemX[i], itemZ[i]);
						if (dist < shortestDist)
						{
							shortestDist = dist;
							closest = i;
						}
					}

					// Move enemy along direction vector toward closest item (Lab 4A Section 4)
					if (shortestDist > 0.1f)
					{
						float directionX = (itemX[closest] - enemyX[e]) / shortestDist;
						float directionZ = (itemZ[closest] - enemyZ[e]) / shortestDist;

						float step = kEnemySpeed * frameTime;
						if (step > shortestDist)
						{
							step = shortestDist;
						}

						enemies[e]->MoveX(directionX * step);
						enemies[e]->MoveZ(directionZ * step);

						enemyX[e] = enemies[e]->GetX();
						enemyZ[e] = enemies[e]->GetZ();
					}

					// Check if enemy picks up any item (Milestone 5a.iii)
					for (int i = 0; i < kTotalItems; i++)
					{
						if (SphereToBox(enemyX[e], enemyZ[e], enemyRadius[e], itemX[i], itemZ[i]) == true)
						{
							SpawnItem(items, itemX, itemZ, skins, i, kTotalItems, playerX, playerZ, playerRadius);
							enemyPoints[e] = enemyPoints[e] + kPointsPerItem;

							if (enemyPoints[e] % kPointsPerGrow == 0)
							{
								enemies[e]->Scale(kGrowScale);
								enemyRadius[e] = enemyRadius[e] * kGrowScale;
								enemies[e]->SetY(enemyRadius[e]);
							}
						}
					}

					// If enemy reaches 500 points first, GameOver (Milestone 5b)
					if (enemyPoints[e] >= kWinPoints)
					{
						gameState = GameOver;
					}
				}
			}
		}

		// 5. Player movement, camera, Hyper Mode, and collisions when Playing
		if (gameState == Playing)
		{
			// Switch camera views with Key 1, Key 2, Key 3 (Milestone 2a)
			if (myEngine->KeyHit(Key_1))
			{
				camera->SetPosition(0, 500, 0);
				camera->ResetOrientation();
				camera->RotateLocalX(90);
				cameraState = TopDown;
			}
			if (myEngine->KeyHit(Key_2))
			{
				camera->SetPosition(350, 350, -350);
				camera->ResetOrientation();
				camera->RotateLocalY(-45);
				camera->RotateLocalX(45);
				cameraState = Isometric;
			}
			if (myEngine->KeyHit(Key_3))
			{
				cameraState = ThirdView;
			}

			// Move player with W/S and rotate with A/D or mouse buttons (Milestone 1b)
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

			// Read player X and Z once per frame
			playerX = player->GetX();
			playerZ = player->GetZ();

			// Move camera with Arrow keys in TopDown, or follow player in ThirdView (Milestone 1d & Lab 2C)
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
			if (cameraState == ThirdView)
			{
				camera->SetPosition(playerX, 65, playerZ - 90);
				camera->ResetOrientation();
				camera->RotateLocalX(32);
			}

			// Check if player stepped into the water outside the 600x600 island (Milestone 1b.v)
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

			// Check if player picks up the special HyperCube (Milestone 4c.i & 4c.ii)
			if (hyperCubeActive == true)
			{
				if (SphereToBox(playerX, playerZ, playerRadius, hyperCubeX, hyperCubeZ) == true)
				{
					hyperCubeActive = false;
					hyperCube->SetY(-100.0f);
					sphereState = Hyper;
					hyperTimer = kHyperSeconds;
					player->SetSkin("hypersphere.jpg");
				}
			}

			// If in Hyper state, pull items within 80 units toward player (Milestone 4c.iii & Lab 4A)
			if (sphereState == Hyper)
			{
				hyperTimer = hyperTimer - frameTime;

				for (int i = 0; i < kTotalItems; i++)
				{
					float dist = GetDistance(itemX[i], itemZ[i], playerX, playerZ);
					if (dist <= kHyperRadius)
					{
						if (dist > 0.1f)
						{
							float directionX = (playerX - itemX[i]) / dist;
							float directionZ = (playerZ - itemZ[i]) / dist;

							items[i]->MoveX(directionX * kSphereSpeed * 1.5f * frameTime);
							items[i]->MoveZ(directionZ * kSphereSpeed * 1.5f * frameTime);

							itemX[i] = items[i]->GetX();
							itemZ[i] = items[i]->GetZ();
						}
					}
				}

				if (hyperTimer <= 0.0f)
				{
					sphereState = Regular;
					player->SetSkin("regularsphere.jpg");
				}
			}

			// Rotate items and check if player picks them up (Milestone 2b, 2c, 3b, 4b)
			for (int i = 0; i < kTotalItems; i++)
			{
				items[i]->RotateY(40.0f * frameTime);

				if (SphereToBox(playerX, playerZ, playerRadius, itemX[i], itemZ[i]) == true)
				{
					SpawnItem(items, itemX, itemZ, skins, i, kTotalItems, playerX, playerZ, playerRadius);
					playerPoints = playerPoints + kPointsPerItem;

					// Every 80 points (8 pickups), grow sphere by 1.2x and raise Y (Milestone 2c)
					if (playerPoints % kPointsPerGrow == 0)
					{
						player->Scale(kGrowScale);
						playerRadius = playerRadius * kGrowScale;
						player->SetY(playerRadius);
					}

					// At 240 points (3 growth steps), activate magnetic Hyper Mode!
					if (playerPoints == (kPointsPerGrow * 3))
					{
						sphereState = Hyper;
						hyperTimer = kHyperSeconds;
						player->SetSkin("hypersphere.jpg");
					}

					// Win Goal 1: Reach 500 points (Milestone 3c)
					if (playerPoints >= kWinPoints)
					{
						gameState = GameWon;
					}
				}
			}

			// Check sphere-to-sphere collision between Player and Enemies (Milestone 5a.v & Lab 4B Section 6)
			for (int e = 0; e < kTotalEnemies; e++)
			{
				if (enemyActive[e] == true)
				{
					float dist = GetDistance(playerX, playerZ, enemyX[e], enemyZ[e]);

					if (dist <= (playerRadius + enemyRadius[e]))
					{
						int pointDiff = playerPoints - enemyPoints[e];

						// Player has > 40 more points -> Swallow enemy! (+40 points, grow 1.2x)
						if (pointDiff > kSwallowPointLead)
						{
							enemyActive[e] = false;
							enemies[e]->SetY(-200.0f);
							playerPoints = playerPoints + kSwallowPointLead;
							enemiesSwallowed = enemiesSwallowed + 1;

							player->Scale(kGrowScale);
							playerRadius = playerRadius * kGrowScale;
							player->SetY(playerRadius);

							// Check both win goals: 500 points OR 5 swallowed opponents
							if (playerPoints >= kWinPoints)
							{
								gameState = GameWon;
							}
							if (enemiesSwallowed >= kTotalEnemies)
							{
								gameState = GameWon;
							}
						}
						// Enemy has > 40 more points -> Enemy swallows player! (Milestone 5b)
						else if (pointDiff < -kSwallowPointLead)
						{
							player->SetY(-200.0f);
							enemyPoints[e] = enemyPoints[e] + kSwallowPointLead;
							enemies[e]->Scale(kGrowScale);
							gameState = GameOver;
						}
						// Point difference is 40 or less -> Bounce apart! (Milestone 5a.v)
						else
						{
							if (dist > 0.1f)
							{
								float pushX = ((enemyX[e] - playerX) / dist) * 15.0f;
								float pushZ = ((enemyZ[e] - playerZ) / dist) * 15.0f;

								player->MoveX(-pushX);
								player->MoveZ(-pushZ);
								enemies[e]->MoveX(pushX);
								enemies[e]->MoveZ(pushZ);

								playerX = player->GetX();
								playerZ = player->GetZ();
								enemyX[e] = enemies[e]->GetX();
								enemyZ[e] = enemies[e]->GetZ();
							}
						}
					}
				}
			}
		}
	}

	// Delete engine on exit
	myEngine->Delete();
}
