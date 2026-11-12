// TL11-Template.cpp: A program using the TL-Engine
// Oleksandr Tsyhankov 21342332
// Version 2.1

#include "TL-Engine11.h"
#include <iostream>
#include <sstream>
using namespace tle;
using namespace std;

// I am Speed
const float kSphereSpeed = 55;
const float kRotationSpeed = 70;
const float kCameraSpeed = 80;
const float kEnemySpeed = 35;

// World and spawn 
const float kIslandBorder = 300;
const int kSpawnRange = 500;
const int kSpawnMin = -250;
const float kMinItemSpacing = 10;
const float kMinEnemyStartDist = 120;
const float kCubeHalfWidth = 2.5f;

// Gameplay
const int kTotalItems = 36;
const int kTotalEnemies = 5;
const int kTotalSkins = 8;
const int kPointsPerItem = 10;
const int kPointsPerGrow = 80;
const int kSwallowPointLead = 40;
const int kWinPoints = 500;
const float kGrowScale = 1.2f;
const float kHyperRadius = 80;
const float kHyperSeconds = 8;

// Game states 
enum EGameState { Playing, Paused, GameWon, GameOver };

// Camera states 
enum ECameraState { TopDown, Isometric, ThirdView };

// Sphere states 
enum ESphereState { Regular, Hyper };

// Distance between two points
float GetDistance(float x1, float z1, float x2, float z2)
{
	float vectorX = x2 - x1;
	float vectorZ = z2 - z1;
	float distance = sqrt(vectorX * vectorX + vectorZ * vectorZ);
	return distance;
}

// Sphere-to-box collision check 
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

// Spawn n items at safe random positions 
void SpawnItem(Model* items[], float itemX[], float itemZ[], string skins[], int currentItem, int totalSpawned, float playerX, float playerZ, float playerRadius)
{
	bool safePosition = false;

	while (safePosition == false)
	{
		float randomX = (float)((rand() % kSpawnRange) + kSpawnMin);
		float randomZ = (float)((rand() % kSpawnRange) + kSpawnMin);
		safePosition = true;

		// Check distance from player
		float distToPlayer = GetDistance(randomX, randomZ, playerX, playerZ);
		if (distToPlayer < (playerRadius + kMinItemSpacing))
		{
			safePosition = false;
		}

		// Check 10-unit distance from other items
		for (int otherItem = 0; otherItem < totalSpawned; otherItem++)
		{
			if (otherItem != currentItem)
			{
				float distToItem = GetDistance(randomX, randomZ, itemX[otherItem], itemZ[otherItem]);
				if (distToItem < kMinItemSpacing)
				{
					safePosition = false;
				}
			}
		}

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
	// Start engine
	TLEngine* myEngine = New3DEngine(TL11);
	myEngine->StartWindowed();

	// Media
	myEngine->AddMediaFolder("/media");
	myEngine->AddMediaFolder("media");

	// Load font
	Font* myFont = myEngine->LoadFont("Arial", 32);

	// Load stages
	Mesh* waterMesh  = myEngine->LoadMesh("water.x");
	Mesh* islandMesh = myEngine->LoadMesh("island.x");
	Mesh* skyMesh    = myEngine->LoadMesh("sky.x");
	Mesh* sphereMesh = myEngine->LoadMesh("spheremesh.x");

	// Load items
	Mesh* cubeMesh   = myEngine->LoadMesh("minicube.x");
	Mesh* teapotMesh = myEngine->LoadMesh("Teapot.x");
	Mesh* cswimMesh  = myEngine->LoadMesh("torus.x");
	Mesh* coinMesh   = myEngine->LoadMesh("TwoPence.x");

	// Create models
	Model* water  = waterMesh->CreateModel(0, -5, 0);
	Model* island = islandMesh->CreateModel(0, -5, 0);
	Model* sky    = skyMesh->CreateModel(0, -960, 0);
	Model* player = sphereMesh->CreateModel(0, 10, 0);

	island->ScaleX(3);
	island->ScaleZ(3);
	water->ScaleX(3);
	water->ScaleZ(3);

	island->SetSkin("metalgrid2_albedo.png");
	water->SetSkin("Stars.jpg");
	player->SetSkin("regularsphere.jpg");

	// Player variables
	float playerX = 0;
	float playerZ = 0;
	float playerRadius = 10;
	int playerPoints = 0;
	int enemiesSwallowed = 0;

	// Random colour skins
	string skins[kTotalSkins] = {
		"minicube.jpg", "tiles1.jpg", "RedBall.jpg", "YellowBall.jpg",
		"Baize.jpg", "Earth.jpg", "Jupiter.jpg", "Mars.jpg"
	};

	// Create n items on the floor
	Model* items[kTotalItems];
	float itemX[kTotalItems];
	float itemZ[kTotalItems];

	for (int itemNum = 0; itemNum < kTotalItems; itemNum++)
	{
		if (itemNum < 18)
		{
			items[itemNum] = cubeMesh->CreateModel(0, kCubeHalfWidth, 0);
		}
		else if (itemNum < 24)
		{
			items[itemNum] = teapotMesh->CreateModel(0, kCubeHalfWidth, 0);
			items[itemNum]->Scale(0.4f);
		}
		else if (itemNum < 30)
		{
			items[itemNum] = cswimMesh->CreateModel(0, kCubeHalfWidth, 0);
			items[itemNum]->Scale(0.2f);
		}
		else
		{
			items[itemNum] = coinMesh->CreateModel(0, kCubeHalfWidth, 0);
			items[itemNum]->Scale(0.25f);
		}

		SpawnItem(items, itemX, itemZ, skins, itemNum, itemNum, playerX, playerZ, playerRadius);
	}

	// Create special HyperCube
	Model* hyperCube = cubeMesh->CreateModel(60, kCubeHalfWidth, 60);
	hyperCube->SetSkin("hypercube.jpg");
	float hyperCubeX = 60;
	float hyperCubeZ = 60;
	bool hyperCubeActive = true;

	// Create n Enemy NPC
	Model* enemies[kTotalEnemies];
	float enemyX[kTotalEnemies];
	float enemyZ[kTotalEnemies];
	float enemyRadius[kTotalEnemies];
	int enemyPoints[kTotalEnemies];
	bool enemyActive[kTotalEnemies];

	for (int enemyNum = 0; enemyNum < kTotalEnemies; enemyNum++)
	{
		enemies[enemyNum] = sphereMesh->CreateModel(0, 10, 0);
		enemies[enemyNum]->SetSkin("enemysphere.jpg");
		enemyRadius[enemyNum] = 10;
		enemyPoints[enemyNum] = 0;
		enemyActive[enemyNum] = true;

		bool farEnough = false;
		while (farEnough == false)
		{
			float randX = (float)((rand() % kSpawnRange) + kSpawnMin);
			float randZ = (float)((rand() % kSpawnRange) + kSpawnMin);

			if (GetDistance(randX, randZ, playerX, playerZ) > kMinEnemyStartDist)
			{
				enemyX[enemyNum] = randX;
				enemyZ[enemyNum] = randZ;
				enemies[enemyNum]->SetPosition(randX, 10, randZ);
				farEnough = true;
			}
		}
	}

	//Camera
	Camera* camera = myEngine->CreateCamera(ManualCamera, 0, 65, -90);
	camera->RotateLocalX(32);

	EGameState gameState = Playing;
	ECameraState cameraState = ThirdView;
	ESphereState sphereState = Regular;
	float hyperTimer = 0;

	// Game Of Something
	while (myEngine->IsRunning())
	{
		myEngine->DrawScene();
		float frameTime = myEngine->FrameTime();
		if (frameTime > 0.1f)
		{
			frameTime = 0.016f;
		}

		// Quit of this BS
		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}

		// Pause this 
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

		// Find highest enemy score
		int bestEnemyPoints = 0;
		for (int enemyNum = 0; enemyNum < kTotalEnemies; enemyNum++)
		{
			if (enemyPoints[enemyNum] > bestEnemyPoints)
			{
				bestEnemyPoints = enemyPoints[enemyNum];
			}
		}

		// Draw leaderboard and UI text
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

		myFont->Draw(firstLine.str(), rightX, 0, White, RightAlign);
		myFont->Draw(secondLine.str(), rightX, 40, White, RightAlign);

		stringstream swallowText;
		swallowText << "Opponents Swallowed: " << enemiesSwallowed << " / " << kTotalEnemies;
		myFont->Draw(swallowText.str(), rightX, 80, Blue, RightAlign);

		myFont->Draw("Keys: 1=Top  2=Iso  3=Third-View", 10, 0, White, LeftAlign);

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
			myFont->Draw("Congrats, You WON! Nothing!!", centerX, centerY, Green, CentreAlign, MiddleAlign);
		}
		if (gameState == GameOver)
		{
			myFont->Draw("Game Over, How did you lose?", centerX, centerY, Red, CentreAlign, MiddleAlign);
		}

		// Check if enemies should move
		bool moveEnemies = false;
		if (gameState == Playing)
		{
			moveEnemies = true;
		}
		if (gameState == GameOver)
		{
			moveEnemies = true;
		}

		if (moveEnemies == true)
		{
			for (int enemyNum = 0; enemyNum < kTotalEnemies; enemyNum++)
			{
				if (enemyActive[enemyNum] == true)
				{
					// Find closest item
					int closestItem = 0;
					float shortestDist = 99999;

					for (int itemNum = 0; itemNum < kTotalItems; itemNum++)
					{
						float dist = GetDistance(enemyX[enemyNum], enemyZ[enemyNum], itemX[itemNum], itemZ[itemNum]);
						if (dist < shortestDist)
						{
							shortestDist = dist;
							closestItem = itemNum;
						}
					}

					// Move enemy toward closest item
					if (shortestDist > 0.1f)
					{
						float directionX = (itemX[closestItem] - enemyX[enemyNum]) / shortestDist;
						float directionZ = (itemZ[closestItem] - enemyZ[enemyNum]) / shortestDist;

						float step = kEnemySpeed * frameTime;
						if (step > shortestDist)
						{
							step = shortestDist;
						}

						enemies[enemyNum]->MoveX(directionX * step);
						enemies[enemyNum]->MoveZ(directionZ * step);

						enemyX[enemyNum] = enemies[enemyNum]->GetX();
						enemyZ[enemyNum] = enemies[enemyNum]->GetZ();
					}

					// Check enemy item pickup
					for (int itemNum = 0; itemNum < kTotalItems; itemNum++)
					{
						if (SphereToBox(enemyX[enemyNum], enemyZ[enemyNum], enemyRadius[enemyNum], itemX[itemNum], itemZ[itemNum]) == true)
						{
							SpawnItem(items, itemX, itemZ, skins, itemNum, kTotalItems, playerX, playerZ, playerRadius);
							enemyPoints[enemyNum] = enemyPoints[enemyNum] + kPointsPerItem;

							if (enemyPoints[enemyNum] % kPointsPerGrow == 0)
							{
								enemies[enemyNum]->Scale(kGrowScale);
								enemyRadius[enemyNum] = enemyRadius[enemyNum] * kGrowScale;
								enemies[enemyNum]->SetY(enemyRadius[enemyNum]);
							}
						}
					}

					// Check if enemy wins first
					if (enemyPoints[enemyNum] >= kWinPoints)
					{
						gameState = GameOver;
					}
				}
			}
		}

		// Player movement and pickups when Playing
		if (gameState == Playing)
		{
			// Switch camera views
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

			// Move player
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

			playerX = player->GetX();
			playerZ = player->GetZ();

			// Move camera
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

			// Check water borders
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

			// Check HyperCube pickup
			if (hyperCubeActive == true)
			{
				if (SphereToBox(playerX, playerZ, playerRadius, hyperCubeX, hyperCubeZ) == true)
				{
					hyperCubeActive = false;
					hyperCube->SetY(-100);
					sphereState = Hyper;
					hyperTimer = kHyperSeconds;
					player->SetSkin("hypersphere.jpg");
				}
			}

			// Update Hyper Mode magnet
			if (sphereState == Hyper)
			{
				hyperTimer = hyperTimer - frameTime;

				for (int itemNum = 0; itemNum < kTotalItems; itemNum++)
				{
					float dist = GetDistance(itemX[itemNum], itemZ[itemNum], playerX, playerZ);
					if (dist <= kHyperRadius)
					{
						if (dist > 0.1f)
						{
							float directionX = (playerX - itemX[itemNum]) / dist;
							float directionZ = (playerZ - itemZ[itemNum]) / dist;

							items[itemNum]->MoveX(directionX * kSphereSpeed * 1.5f * frameTime);
							items[itemNum]->MoveZ(directionZ * kSphereSpeed * 1.5f * frameTime);

							itemX[itemNum] = items[itemNum]->GetX();
							itemZ[itemNum] = items[itemNum]->GetZ();
						}
					}
				}

				if (hyperTimer <= 0)
				{
					sphereState = Regular;
					player->SetSkin("regularsphere.jpg");
				}
			}

			// Rotate items and check player pickup
			for (int itemNum = 0; itemNum < kTotalItems; itemNum++)
			{
				items[itemNum]->RotateY(40 * frameTime);

				if (SphereToBox(playerX, playerZ, playerRadius, itemX[itemNum], itemZ[itemNum]) == true)
				{
					SpawnItem(items, itemX, itemZ, skins, itemNum, kTotalItems, playerX, playerZ, playerRadius);
					playerPoints = playerPoints + kPointsPerItem;

					// Grow sphere
					if (playerPoints % kPointsPerGrow == 0)
					{
						player->Scale(kGrowScale);
						playerRadius = playerRadius * kGrowScale;
						player->SetY(playerRadius);
					}

					// Activate Hyper Mode
					if (playerPoints == (kPointsPerGrow * 3))
					{
						sphereState = Hyper;
						hyperTimer = kHyperSeconds;
						player->SetSkin("hypersphere.jpg");
					}

					// Check the win
					if (playerPoints >= kWinPoints)
					{
						gameState = GameWon;
					}
				}
			}

			// Check collision between Player and Enemies
			for (int enemyNum = 0; enemyNum < kTotalEnemies; enemyNum++)
			{
				if (enemyActive[enemyNum] == true)
				{
					float dist = GetDistance(playerX, playerZ, enemyX[enemyNum], enemyZ[enemyNum]);

					if (dist <= (playerRadius + enemyRadius[enemyNum]))
					{
						int pointDiff = playerPoints - enemyPoints[enemyNum];

						// Player swallows enemy (> 40 points)
						if (pointDiff > kSwallowPointLead)
						{
							enemyActive[enemyNum] = false;
							enemies[enemyNum]->SetY(-200);
							playerPoints = playerPoints + kSwallowPointLead;
							enemiesSwallowed = enemiesSwallowed + 1;

							player->Scale(kGrowScale);
							playerRadius = playerRadius * kGrowScale;
							player->SetY(playerRadius);

							if (playerPoints >= kWinPoints)
							{
								gameState = GameWon;
							}
							if (enemiesSwallowed >= kTotalEnemies)
							{
								gameState = GameWon;
							}
						}
						// Enemy swallows player (> 40 points)
						else if (pointDiff < -kSwallowPointLead)
						{
							player->SetY(-200);
							enemyPoints[enemyNum] = enemyPoints[enemyNum] + kSwallowPointLead;
							enemies[enemyNum]->Scale(kGrowScale);
							gameState = GameOver;
						}
						// Bounce apart (difference <= 40)
						else
						{
							if (dist > 0.1f)
							{
								float pushX = ((enemyX[enemyNum] - playerX) / dist) * 15;
								float pushZ = ((enemyZ[enemyNum] - playerZ) / dist) * 15;

								player->MoveX(-pushX);
								player->MoveZ(-pushZ);
								enemies[enemyNum]->MoveX(pushX);
								enemies[enemyNum]->MoveZ(pushZ);

								playerX = player->GetX();
								playerZ = player->GetZ();
								enemyX[enemyNum] = enemies[enemyNum]->GetX();
								enemyZ[enemyNum] = enemies[enemyNum]->GetZ();
							}
						}
					}
				}
			}
		}
	}

	// Stop this 
	myEngine->Delete();
}
