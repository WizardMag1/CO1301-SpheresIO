// Assignment1_aaslanian.cpp: A program using the TL-Engine
// CO1035: Game Concepts - Assignment 1: Spheres.io
// Student Name: Aleksandr Aslanian
// Student ID: 21379063
// Student Username: aaslanian

#include <TL-Engine11.h>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace tle;
using namespace std;

// Window settings
const int kWindowWidth = 1280;
const int kWindowHeight = 720;

// World and island boundaries
const float kIslandLimit = 100;
const float kWaterHeight = -5;
const float kSkyHeight = -960;
const float kHiddenY = -500;
const float kFloorBuffer = 1;

// Movement speeds
const float kSphereSpeed = 50;
const float kRotationSpeed = 100;
const float kCameraSpeed = 60;
const float kEnemySpeed = 38;
const float kEnemyChaseSpeed = 44;

// Radii and growth settings
const float kStartRadius = 10;
const float kGrowScale = 1.2f;
const int kPointsPerGrow = 40;
const int kPointsToWin = 120;
const int kPointsPerCube = 10;
const int kPointsPerEnemy = 40;

// Cubes and spawn limits
const int kCubeCount = 12;
const float kCubeHalfSize = 2.5f;
const float kCubeFloorY = 2;
const float kMinCubeDistance = 10;
const float kMaxSpawnDistance = 85;
const float kEnemyStartClearance = 60;

// Hyper mode settings
const float kHyperDuration = 5;
const float kHyperRadius = 50;
const float kMagnetSpeed = 40;

// Game states
enum EGameState
{
    Playing,
    Paused,
    GameOver,
    GameWon
};

// Camera modes
enum ECameraMode
{
    TopDown,
    Isometric
};

// Sphere power modes
enum ESphereMode
{
    Regular,
    Hyper
};

// -------------------------------------------------------------
// Helper Functions (Only 3 functions above main)
// -------------------------------------------------------------

// Calculate distance between two points on the floor
float GetDistance(float x1, float z1, float x2, float z2)
{
    float deltaX = x1 - x2;
    float deltaZ = z1 - z2;
    return sqrt(deltaX * deltaX + deltaZ * deltaZ);
}

// Sphere to box collision check from lecture 4b
bool SphereToBox(float sphereX, float sphereY, float sphereZ, float sphereRadius, float boxX, float boxY, float boxZ, float boxHalfSize)
{
    // Clamp sphere coordinates to box bounds
    float closestX = sphereX;
    if (closestX < boxX - boxHalfSize)
    {
        closestX = boxX - boxHalfSize;
    }
    if (closestX > boxX + boxHalfSize)
    {
        closestX = boxX + boxHalfSize;
    }

    float closestY = sphereY;
    if (closestY < boxY - boxHalfSize)
    {
        closestY = boxY - boxHalfSize;
    }
    if (closestY > boxY + boxHalfSize)
    {
        closestY = boxY + boxHalfSize;
    }

    float closestZ = sphereZ;
    if (closestZ < boxZ - boxHalfSize)
    {
        closestZ = boxZ - boxHalfSize;
    }
    if (closestZ > boxZ + boxHalfSize)
    {
        closestZ = boxZ + boxHalfSize;
    }

    float deltaX = sphereX - closestX;
    float deltaY = sphereY - closestY;
    float deltaZ = sphereZ - closestZ;

    float distanceSquared = deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;
    return distanceSquared <= (sphereRadius * sphereRadius);
}

// Spawn cube at a safe random position
void SpawnCube(IModel* cubeToSpawn, int cubeIndex,
               float playerX, float playerZ, float playerRadius,
               float enemyX, float enemyZ, float enemyRadius, bool enemyActive,
               IModel* cubes[], int totalCubes, IModel* hyperCube)
{
    float spawnX = 0;
    float spawnZ = 0;
    bool positionFound = false;

    // Loop until position is safe
    while (!positionFound)
    {
        float ratioX = (float)rand() / (float)RAND_MAX;
        float ratioZ = (float)rand() / (float)RAND_MAX;
        spawnX = -kMaxSpawnDistance + ratioX * (kMaxSpawnDistance * 2);
        spawnZ = -kMaxSpawnDistance + ratioZ * (kMaxSpawnDistance * 2);

        bool safe = true;

        // Check distance to player
        float minPlayerDistance = playerRadius + kCubeHalfSize + 5;
        if (GetDistance(spawnX, spawnZ, playerX, playerZ) < minPlayerDistance)
        {
            safe = false;
        }

        // Check distance to enemy
        if (enemyActive)
        {
            float minEnemyDistance = enemyRadius + kCubeHalfSize + 5;
            if (GetDistance(spawnX, spawnZ, enemyX, enemyZ) < minEnemyDistance)
            {
                safe = false;
            }
        }

        // Check distance to regular cubes
        for (int otherCube = 0; otherCube < totalCubes; otherCube++)
        {
            if (otherCube == cubeIndex)
            {
                continue;
            }
            if (cubes[otherCube]->GetY() <= 0)
            {
                continue;
            }

            float cubeX = cubes[otherCube]->GetX();
            float cubeZ = cubes[otherCube]->GetZ();
            if (GetDistance(spawnX, spawnZ, cubeX, cubeZ) < kMinCubeDistance)
            {
                safe = false;
            }
        }

        // Check distance to hypercube
        if (hyperCube != NULL)
        {
            if (cubeIndex != -1)
            {
                if (hyperCube->GetY() > 0)
                {
                    float hyperX = hyperCube->GetX();
                    float hyperZ = hyperCube->GetZ();
                    if (GetDistance(spawnX, spawnZ, hyperX, hyperZ) < kMinCubeDistance)
                    {
                        safe = false;
                    }
                }
            }
        }

        if (safe)
        {
            positionFound = true;
        }
    }

    cubeToSpawn->SetPosition(spawnX, kCubeFloorY, spawnZ);
}

// -------------------------------------------------------------
// Main Game Program
// -------------------------------------------------------------

int main()
{
    // Start 3D engine
    I3DEngine* myEngine = New3DEngine(kTLX);
    myEngine->StartWindowed(kWindowWidth, kWindowHeight);
    myEngine->AddMediaFolder("Media");

    // Load scenery meshes
    IMesh* islandMesh = myEngine->LoadMesh("island.x");
    islandMesh->CreateModel(0, kWaterHeight, 0);

    IMesh* waterMesh = myEngine->LoadMesh("water.x");
    waterMesh->CreateModel(0, kWaterHeight, 0);

    IMesh* skyMesh = myEngine->LoadMesh("sky.x");
    skyMesh->CreateModel(0, kSkyHeight, 0);

    // Load shared meshes
    IMesh* sphereMesh = myEngine->LoadMesh("spheremesh.x");
    IMesh* cubeMesh = myEngine->LoadMesh("minicube.x");

    // Player setup
    float playerRadius = kStartRadius;
    int playerPoints = 0;
    int playerGrowthTier = 0;
    ESphereMode playerMode = Regular;
    float playerHyperTimer = 0;

    IModel* player = sphereMesh->CreateModel(0, playerRadius, 0);
    player->SetSkin("regularsphere.jpg");

    // Camera setup
    ICamera* camera = myEngine->CreateCamera(kManual, 0, 200, 0);
    camera->RotateLocalX(90);
    ECameraMode cameraMode = TopDown;

    // Cubes setup
    IModel* cubes[kCubeCount];
    for (int cubeNum = 0; cubeNum < kCubeCount; cubeNum++)
    {
        cubes[cubeNum] = cubeMesh->CreateModel(0, kHiddenY, 0);
        cubes[cubeNum]->SetSkin("minicube.jpg");
    }

    // Hypercube setup
    IModel* hyperCube = cubeMesh->CreateModel(0, kHiddenY, 0);
    hyperCube->SetSkin("hypercube.jpg");

    // Enemy setup
    float enemyRadius = kStartRadius;
    int enemyPoints = 0;
    int enemyGrowthTier = 0;
    bool enemyActive = true;
    ESphereMode enemyMode = Regular;
    float enemyHyperTimer = 0;

    IModel* enemy = sphereMesh->CreateModel(0, kHiddenY, 0);
    enemy->SetSkin("enemysphere.jpg");

    // Seed random numbers with time
    srand((unsigned int)time(NULL));

    // Place enemy far away from player
    float enemySpawnX = 0;
    float enemySpawnZ = 0;
    bool enemySpawnSafe = false;
    while (!enemySpawnSafe)
    {
        float ratioX = (float)rand() / (float)RAND_MAX;
        float ratioZ = (float)rand() / (float)RAND_MAX;
        enemySpawnX = -kMaxSpawnDistance + ratioX * (kMaxSpawnDistance * 2);
        enemySpawnZ = -kMaxSpawnDistance + ratioZ * (kMaxSpawnDistance * 2);

        if (GetDistance(enemySpawnX, enemySpawnZ, 0, 0) >= kEnemyStartClearance)
        {
            enemySpawnSafe = true;
        }
    }
    enemy->SetPosition(enemySpawnX, enemyRadius, enemySpawnZ);

    // Spawn hypercube and regular cubes
    SpawnCube(hyperCube, -1, player->GetX(), player->GetZ(), playerRadius,
              enemy->GetX(), enemy->GetZ(), enemyRadius, enemyActive,
              cubes, kCubeCount, hyperCube);

    for (int cubeNum = 0; cubeNum < kCubeCount; cubeNum++)
    {
        SpawnCube(cubes[cubeNum], cubeNum, player->GetX(), player->GetZ(), playerRadius,
                  enemy->GetX(), enemy->GetZ(), enemyRadius, enemyActive,
                  cubes, kCubeCount, hyperCube);
    }

    // Load fonts
    IFont* hudFont = myEngine->LoadFont("Arial", 22);
    IFont* titleFont = myEngine->LoadFont("Arial", 36);

    // Game state and frame timer
    EGameState gameState = Playing;
    float frameTime = myEngine->Timer();

    // Main game loop
    while (myEngine->IsRunning())
    {
        myEngine->DrawScene();
        frameTime = myEngine->Timer();

        // Exit on Escape key
        if (myEngine->KeyHit(Key_Escape))
        {
            myEngine->Stop();
        }

        // Playing state logic
        if (gameState == Playing)
        {
            // Toggle pause state
            if (myEngine->KeyHit(Key_P))
            {
                gameState = Paused;
            }

            // Camera switching
            if (myEngine->KeyHit(Key_1))
            {
                cameraMode = TopDown;
                camera->SetPosition(0, 200, 0);
                camera->ResetOrientation();
                camera->RotateLocalX(90);
            }

            if (myEngine->KeyHit(Key_2))
            {
                cameraMode = Isometric;
                camera->SetPosition(150, 150, -150);
                camera->ResetOrientation();
                camera->RotateY(-45);
                camera->RotateLocalX(45);
            }

            // Top down camera movement
            if (cameraMode == TopDown)
            {
                if (myEngine->KeyHeld(Key_Up))
                {
                    camera->Move(0, 0, kCameraSpeed * frameTime);
                }
                if (myEngine->KeyHeld(Key_Down))
                {
                    camera->Move(0, 0, -kCameraSpeed * frameTime);
                }
                if (myEngine->KeyHeld(Key_Right))
                {
                    camera->Move(kCameraSpeed * frameTime, 0, 0);
                }
                if (myEngine->KeyHeld(Key_Left))
                {
                    camera->Move(-kCameraSpeed * frameTime, 0, 0);
                }
            }

            // Player movement
            if (myEngine->KeyHeld(Key_W))
            {
                player->MoveLocalZ(kSphereSpeed * frameTime);
            }
            if (myEngine->KeyHeld(Key_S))
            {
                player->MoveLocalZ(-kSphereSpeed * frameTime);
            }
            if (myEngine->KeyHeld(Key_A))
            {
                player->RotateY(-kRotationSpeed * frameTime);
            }
            if (myEngine->KeyHeld(Key_D))
            {
                player->RotateY(kRotationSpeed * frameTime);
            }

            // Store player coordinates
            float playerX = player->GetX();
            float playerZ = player->GetZ();

            // Check water boundary
            bool fellInWater = false;
            if (playerX > kIslandLimit)
            {
                fellInWater = true;
            }
            if (playerX < -kIslandLimit)
            {
                fellInWater = true;
            }
            if (playerZ > kIslandLimit)
            {
                fellInWater = true;
            }
            if (playerZ < -kIslandLimit)
            {
                fellInWater = true;
            }
            if (fellInWater)
            {
                gameState = GameOver;
            }

            // Player hyper mode
            if (playerMode == Hyper)
            {
                playerHyperTimer = playerHyperTimer - frameTime;
                if (playerHyperTimer <= 0)
                {
                    playerMode = Regular;
                    player->SetSkin("regularsphere.jpg");
                }

                // Attract regular cubes
                for (int cubeNum = 0; cubeNum < kCubeCount; cubeNum++)
                {
                    float cubeX = cubes[cubeNum]->GetX();
                    float cubeZ = cubes[cubeNum]->GetZ();
                    float dist = GetDistance(cubeX, cubeZ, playerX, playerZ);

                    if (dist < kHyperRadius)
                    {
                        if (dist > 0.1f)
                        {
                            float dirX = (playerX - cubeX) / dist;
                            float dirZ = (playerZ - cubeZ) / dist;
                            cubes[cubeNum]->Move(dirX * kMagnetSpeed * frameTime,
                                                 0,
                                                 dirZ * kMagnetSpeed * frameTime);
                        }
                    }
                }
            }

            // Player regular cube pickups
            for (int cubeNum = 0; cubeNum < kCubeCount; cubeNum++)
            {
                if (cubes[cubeNum]->GetY() <= 0)
                {
                    continue;
                }

                float cubeX = cubes[cubeNum]->GetX();
                float cubeY = cubes[cubeNum]->GetY();
                float cubeZ = cubes[cubeNum]->GetZ();

                bool hit = SphereToBox(playerX, player->GetY(), playerZ, playerRadius,
                                       cubeX, cubeY, cubeZ, kCubeHalfSize);
                if (hit)
                {
                    playerPoints = playerPoints + kPointsPerCube;

                    // Grow player sphere
                    while (playerPoints >= (playerGrowthTier + 1) * kPointsPerGrow)
                    {
                        playerGrowthTier = playerGrowthTier + 1;
                        playerRadius = playerRadius * kGrowScale;
                        player->Scale(kGrowScale);
                        player->SetY(playerRadius);
                    }

                    float enemyX = enemy->GetX();
                    float enemyZ = enemy->GetZ();

                    SpawnCube(cubes[cubeNum], cubeNum,
                              playerX, playerZ, playerRadius,
                              enemyX, enemyZ, enemyRadius, enemyActive,
                              cubes, kCubeCount, hyperCube);

                    if (playerPoints >= kPointsToWin)
                    {
                        gameState = GameWon;
                    }
                }
            }

            // Player hypercube pickup
            if (hyperCube->GetY() > 0)
            {
                float hyperX = hyperCube->GetX();
                float hyperY = hyperCube->GetY();
                float hyperZ = hyperCube->GetZ();

                bool hitHyper = SphereToBox(playerX, player->GetY(), playerZ, playerRadius,
                                            hyperX, hyperY, hyperZ, kCubeHalfSize);
                if (hitHyper)
                {
                    playerMode = Hyper;
                    playerHyperTimer = kHyperDuration;
                    player->SetSkin("hypersphere.jpg");

                    float enemyX = enemy->GetX();
                    float enemyZ = enemy->GetZ();

                    SpawnCube(hyperCube, -1,
                              playerX, playerZ, playerRadius,
                              enemyX, enemyZ, enemyRadius, enemyActive,
                              cubes, kCubeCount, hyperCube);
                }
            }

            // Enemy AI logic
            if (enemyActive)
            {
                if (enemyMode == Hyper)
                {
                    enemyHyperTimer = enemyHyperTimer - frameTime;
                    if (enemyHyperTimer <= 0)
                    {
                        enemyMode = Regular;
                        enemy->SetSkin("enemysphere.jpg");
                    }
                }

                float enemyX = enemy->GetX();
                float enemyZ = enemy->GetZ();
                int scoreDiff = enemyPoints - playerPoints;

                // State 1: Chase player
                if (scoreDiff > kPointsPerGrow)
                {
                    enemy->LookAt(playerX, enemy->GetY(), playerZ);
                    enemy->MoveLocalZ(kEnemyChaseSpeed * frameTime);
                }
                // State 2: Flee from player
                else if (scoreDiff < -kPointsPerGrow)
                {
                    float fleeX = enemyX - playerX;
                    float fleeZ = enemyZ - playerZ;
                    enemy->LookAt(enemyX + fleeX * 10, enemy->GetY(), enemyZ + fleeZ * 10);
                    enemy->MoveLocalZ(kEnemySpeed * frameTime);
                }
                // State 3: Gather nearest cube
                else
                {
                    float closestDist = 99999;
                    float targetX = 0;
                    float targetZ = 0;

                    for (int cubeNum = 0; cubeNum < kCubeCount; cubeNum++)
                    {
                        if (cubes[cubeNum]->GetY() <= 0)
                        {
                            continue;
                        }

                        float dist = GetDistance(enemyX, enemyZ,
                                                 cubes[cubeNum]->GetX(),
                                                 cubes[cubeNum]->GetZ());
                        if (dist < closestDist)
                        {
                            closestDist = dist;
                            targetX = cubes[cubeNum]->GetX();
                            targetZ = cubes[cubeNum]->GetZ();
                        }
                    }

                    if (hyperCube->GetY() > 0)
                    {
                        float distHyper = GetDistance(enemyX, enemyZ,
                                                      hyperCube->GetX(),
                                                      hyperCube->GetZ());
                        if (distHyper < closestDist)
                        {
                            closestDist = distHyper;
                            targetX = hyperCube->GetX();
                            targetZ = hyperCube->GetZ();
                        }
                    }

                    enemy->LookAt(targetX, enemy->GetY(), targetZ);
                    enemy->MoveLocalZ(kEnemySpeed * frameTime);
                }

                // Update enemy position after movement
                enemyX = enemy->GetX();
                enemyZ = enemy->GetZ();

                // Enemy pickup regular cubes
                for (int cubeNum = 0; cubeNum < kCubeCount; cubeNum++)
                {
                    if (cubes[cubeNum]->GetY() <= 0)
                    {
                        continue;
                    }

                    float cubeX = cubes[cubeNum]->GetX();
                    float cubeY = cubes[cubeNum]->GetY();
                    float cubeZ = cubes[cubeNum]->GetZ();

                    bool hit = SphereToBox(enemyX, enemy->GetY(), enemyZ, enemyRadius,
                                           cubeX, cubeY, cubeZ, kCubeHalfSize);
                    if (hit)
                    {
                        enemyPoints = enemyPoints + kPointsPerCube;

                        while (enemyPoints >= (enemyGrowthTier + 1) * kPointsPerGrow)
                        {
                            enemyGrowthTier = enemyGrowthTier + 1;
                            enemyRadius = enemyRadius * kGrowScale;
                            enemy->Scale(kGrowScale);
                            enemy->SetY(enemyRadius);
                        }

                        SpawnCube(cubes[cubeNum], cubeNum,
                                  playerX, playerZ, playerRadius,
                                  enemyX, enemyZ, enemyRadius, enemyActive,
                                  cubes, kCubeCount, hyperCube);

                        if (enemyPoints >= kPointsToWin)
                        {
                            gameState = GameOver;
                        }
                    }
                }

                // Enemy pickup hypercube
                if (hyperCube->GetY() > 0)
                {
                    float hyperX = hyperCube->GetX();
                    float hyperY = hyperCube->GetY();
                    float hyperZ = hyperCube->GetZ();

                    bool hitHyper = SphereToBox(enemyX, enemy->GetY(), enemyZ,
                                                enemyRadius,
                                                hyperX, hyperY, hyperZ, kCubeHalfSize);
                    if (hitHyper)
                    {
                        enemyMode = Hyper;
                        enemyHyperTimer = kHyperDuration;
                        enemy->SetSkin("hypersphere.jpg");

                        SpawnCube(hyperCube, -1,
                                  playerX, playerZ, playerRadius,
                                  enemyX, enemyZ, enemyRadius, enemyActive,
                                  cubes, kCubeCount, hyperCube);
                    }
                }

                // Sphere to sphere collision check
                float sphereDist = GetDistance(playerX, playerZ, enemyX, enemyZ);
                float radiusSum = playerRadius + enemyRadius;

                if (sphereDist <= radiusSum)
                {
                    int pointDiff = abs(playerPoints - enemyPoints);

                    // Equal or close score: bounce spheres
                    if (pointDiff <= kPointsPerGrow)
                    {
                        float deltaX = playerX - enemyX;
                        float deltaZ = playerZ - enemyZ;
                        float dist = sqrt(deltaX * deltaX + deltaZ * deltaZ);
                        if (dist < 0.01f)
                        {
                            dist = 0.01f;
                            deltaX = 0.01f;
                        }

                        float overlap = (playerRadius + enemyRadius) - dist;
                        float normalX = deltaX / dist;
                        float normalZ = deltaZ / dist;
                        float push = overlap * 0.5f + 0.1f;

                        player->SetPosition(playerX + normalX * push,
                                            player->GetY(),
                                            playerZ + normalZ * push);
                        enemy->SetPosition(enemyX - normalX * push,
                                           enemy->GetY(),
                                           enemyZ - normalZ * push);

                        // Clamp player within island
                        float playerLimit = kIslandLimit - playerRadius - kFloorBuffer;
                        if (player->GetX() < -playerLimit) player->SetX(-playerLimit);
                        if (player->GetX() > playerLimit) player->SetX(playerLimit);
                        if (player->GetZ() < -playerLimit) player->SetZ(-playerLimit);
                        if (player->GetZ() > playerLimit) player->SetZ(playerLimit);

                        // Clamp enemy within island
                        float enemyLimit = kIslandLimit - enemyRadius - kFloorBuffer;
                        if (enemy->GetX() < -enemyLimit) enemy->SetX(-enemyLimit);
                        if (enemy->GetX() > enemyLimit) enemy->SetX(enemyLimit);
                        if (enemy->GetZ() < -enemyLimit) enemy->SetZ(-enemyLimit);
                        if (enemy->GetZ() > enemyLimit) enemy->SetZ(enemyLimit);
                    }
                    // Difference > 40: bigger sphere eats smaller sphere
                    else
                    {
                        if (playerPoints > enemyPoints)
                        {
                            playerPoints = playerPoints + kPointsPerEnemy;

                            while (playerPoints >= (playerGrowthTier + 1) * kPointsPerGrow)
                            {
                                playerGrowthTier = playerGrowthTier + 1;
                                playerRadius = playerRadius * kGrowScale;
                                player->Scale(kGrowScale);
                                player->SetY(playerRadius);
                            }

                            enemyActive = false;
                            enemy->SetPosition(0, kHiddenY, 0);

                            if (playerPoints >= kPointsToWin)
                            {
                                gameState = GameWon;
                            }
                        }
                        else
                        {
                            gameState = GameOver;
                        }
                    }
                }

                // Clamp enemy inside island
                float enemyLimit = kIslandLimit - enemyRadius - kFloorBuffer;
                if (enemy->GetX() < -enemyLimit) enemy->SetX(-enemyLimit);
                if (enemy->GetX() > enemyLimit) enemy->SetX(enemyLimit);
                if (enemy->GetZ() < -enemyLimit) enemy->SetZ(-enemyLimit);
                if (enemy->GetZ() > enemyLimit) enemy->SetZ(enemyLimit);
            }

            // Clamp player inside island
            float playerLimit = kIslandLimit - playerRadius - kFloorBuffer;
            if (player->GetX() < -playerLimit) player->SetX(-playerLimit);
            if (player->GetX() > playerLimit) player->SetX(playerLimit);
            if (player->GetZ() < -playerLimit) player->SetZ(-playerLimit);
            if (player->GetZ() > playerLimit) player->SetZ(playerLimit);
        }
        else if (gameState == Paused)
        {
            if (myEngine->KeyHit(Key_P))
            {
                gameState = Playing;
            }

            titleFont->Draw("GAME PAUSED", 640, 300, kYellow, CentreAlign);
            hudFont->Draw("Press 'P' to Resume", 640, 360, kWhite, CentreAlign);
        }
        else if (gameState == GameOver)
        {
            // Enemy continues moving during game over
            if (enemyActive)
            {
                enemy->RotateY(25 * frameTime);
                enemy->MoveLocalZ(kEnemySpeed * 0.5f * frameTime);

                float enemyLimit = kIslandLimit - enemyRadius - kFloorBuffer;
                if (enemy->GetX() < -enemyLimit) enemy->SetX(-enemyLimit);
                if (enemy->GetX() > enemyLimit) enemy->SetX(enemyLimit);
                if (enemy->GetZ() < -enemyLimit) enemy->SetZ(-enemyLimit);
                if (enemy->GetZ() > enemyLimit) enemy->SetZ(enemyLimit);
            }

            titleFont->Draw("GAME OVER", 640, 300, kRed, CentreAlign);

            stringstream scoreStream;
            scoreStream << "Final Score: " << playerPoints;
            hudFont->Draw(scoreStream.str(), 640, 360, kWhite, CentreAlign);
        }
        else if (gameState == GameWon)
        {
            titleFont->Draw("CONGRATULATIONS! YOU WON!", 640, 300, kGreen, CentreAlign);

            stringstream winStream;
            winStream << "Winning Score: " << playerPoints;
            hudFont->Draw(winStream.str(), 640, 360, kWhite, CentreAlign);
        }

        // Draw HUD
        if (gameState != Paused)
        {
            // Camera mode indicator
            if (cameraMode == TopDown)
            {
                hudFont->Draw("Cam: Top-Down [1]", 20, 20, kWhite);
            }
            if (cameraMode == Isometric)
            {
                hudFont->Draw("Cam: Isometric [2]", 20, 20, kWhite);
            }

            // Hyper mode indicator
            if (playerMode == Hyper)
            {
                int secondsLeft = (int)playerHyperTimer + 1;
                stringstream hyperStream;
                hyperStream << "HYPER MODE! " << secondsLeft << "s";
                hudFont->Draw(hyperStream.str(), 20, 50, kCyan);
            }

            // Scoreboard (highest score on top)
            stringstream playerStream;
            playerStream << "Points: " << playerPoints;

            stringstream enemyStream;
            enemyStream << "NPC Score: " << enemyPoints;

            if (playerPoints >= enemyPoints)
            {
                hudFont->Draw(playerStream.str(), 1250, 20, kWhite, RightAlign);
                hudFont->Draw(enemyStream.str(), 1250, 50, kYellow, RightAlign);
            }
            else
            {
                hudFont->Draw(enemyStream.str(), 1250, 20, kYellow, RightAlign);
                hudFont->Draw(playerStream.str(), 1250, 50, kWhite, RightAlign);
            }
        }
    }

    // Close engine and clean up
    myEngine->Delete();
    return 0;
}
