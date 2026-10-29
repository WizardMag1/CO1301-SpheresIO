// TL11-Template.cpp: A program using the TL-Engine

#include "TL-Engine11.h" // TL-Engine11 include file and namespace
#include "math.h"
using namespace tle;

const float kWaterY = -5.0f;
const float kIslandY = -5.0f;
const float kSkyY = -960.0f;
const float kInitialSphereRadius = 10.0f;
const float kInitialSphereY = 10.0f; // Sits flush on island surface, flush means right on top of it (Y = 0)
const float kCubeWidth = 5.0f;
const float kCubeHalfWidth = 2.5f;
const float kCubeY = 2.5f;          // Make sure that cubes are not going to fall

const float kIslandHalfWidth = 100.0f;
const float kIslandHalfDepth = 100.0f;
const float kSphereSpeed = 60.0f;
const float kRotationSpeed = 120.0f;
const float kCameraSpeed = 80.0f;

// Pickup threshold: sphere radius (10) + cube half-width (2.5) = 12.5 TL units. We will compare squared distances to avoid square root calculations. Also we will use a slightly larger threshold to make it easier to pick up cubes.
const float kPickupDistSq = 12.5f * 12.5f;

enum EGameState { Playing, Paused, GameOver };

struct Cube {
	IModel* model;
	float x, y, z;
	bool active;
};

float DistanceSquared(float x1, float y1, float z1, float x2, float y2, float z2) {
	float dx = x2 - x1, dy = y2 - y1, dz = z2 - z1;
	return dx * dx + dy * dy + dz * dz;
}

int main()
{
	// Create a 3D engine (using TL11 engine here) and open a window for it
	TLEngine* myEngine = New3DEngine(TL11);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\Users\\Public\\Documents\\TL-Engine11\\Media");
	myEngine->AddMediaFolder("media");

	/**** Set up your scene here ****/



	/**** Models  ****/

	// 1. Load Meshes (Blueprints)
	IMesh* waterMesh  = myEngine->LoadMesh("water.x");
	IMesh* islandMesh = myEngine->LoadMesh("island.x");
	IMesh* skyMesh    = myEngine->LoadMesh("sky.x");
	IMesh* sphereMesh = myEngine->LoadMesh("spheremesh.x");
	IMesh* cubeMesh   = myEngine->LoadMesh("minicube.x");


	// 2. Create Models (Placed Instances)
	IModel* water  = waterMesh->CreateModel(0.0f, kWaterY, 0.0f);
	IModel* island = islandMesh->CreateModel(0.0f, kIslandY, 0.0f);
	IModel* sky    = skyMesh->CreateModel(0.0f, kSkyY, 0.0f);
	IModel* player = sphereMesh->CreateModel(0.0f, kInitialSphereY, 0.0f);


	// 4 Cubes
	Cube cubes[4] = {
		{ cubeMesh->CreateModel(-80.0f, kCubeY,  80.0f), -80.0f, kCubeY,  80.0f, true },
		{ cubeMesh->CreateModel(80.0f, kCubeY,  80.0f),  80.0f, kCubeY,  80.0f, true },
		{ cubeMesh->CreateModel(80.0f, kCubeY, -80.0f),  80.0f, kCubeY, -80.0f, true },
		{ cubeMesh->CreateModel(-80.0f, kCubeY, -80.0f), -80.0f, kCubeY, -80.0f, true }
	};

	ICamera* camera = myEngine->CreateCamera(kManual, 0.0f, 200.0f, 0.0f);
	camera->RotateLocalX(90.0f);

	EGameState gameState = Playing;

	while (myEngine->IsRunning())
	{
		myEngine->DrawScene();
		float frameTime = myEngine->FrameTime();

		if (myEngine->KeyHit(Key_Escape)) myEngine->Stop();

		if (myEngine->KeyHit(Key_P)) {
			gameState = (gameState == Playing) ? Paused : (gameState == Paused ? Playing : gameState);
		}

		if (gameState == Playing)
		{
			// Player controls
			if (myEngine->KeyHeld(Key_W)) player->MoveLocalZ(kSphereSpeed * frameTime);
			if (myEngine->KeyHeld(Key_S)) player->MoveLocalZ(-kSphereSpeed * frameTime);
			if (myEngine->KeyHeld(Key_D)) player->RotateY(kRotationSpeed * frameTime);
			if (myEngine->KeyHeld(Key_A)) player->RotateY(-kRotationSpeed * frameTime);

			// Camera panning
			if (myEngine->KeyHeld(Key_Up))    camera->MoveZ(kCameraSpeed * frameTime);
			if (myEngine->KeyHeld(Key_Down))  camera->MoveZ(-kCameraSpeed * frameTime);
			if (myEngine->KeyHeld(Key_Right)) camera->MoveX(kCameraSpeed * frameTime);
			if (myEngine->KeyHeld(Key_Left))  camera->MoveX(-kCameraSpeed * frameTime);

			// Water boundary check
			float px = player->GetX(), pz = player->GetZ();
			if (px > kIslandHalfWidth || px < -kIslandHalfWidth || pz > kIslandHalfDepth || pz < -kIslandHalfDepth) {
				gameState = GameOver;
			}

			// Cube collision & pickup
			for (int i = 0; i < 4; ++i) {
				if (cubes[i].active && DistanceSquared(px, player->GetY(), pz, cubes[i].x, cubes[i].y, cubes[i].z) <= kPickupDistSq) {
					cubes[i].active = false;
					cubes[i].model->SetY(-100.0f); // Hide beneath ground
				}
			}
		}
	}
	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
