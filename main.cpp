#include <iostream>
#include "Display.h"
#include "Shader.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Texture.h"
#include "Camera.h"

#include <gl/GLU.h>

#include <gl/GL.h>
#include <SDL/SDL.h>

#include <btBulletDynamicsCommon.h>
#include "BulletSoftBody/btDefaultSoftBodySolver.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

#include <vector>
#include "Helpers.h"
#include "Timer.h"
#include "RigidBodyObject.h"
#include "SoftBodyObject.h"
#include "RigidBodyConvexSphereObject.h"
#include "RigidBodyPlaneObject.h"
#include "RigidBodyConvexBoxObject.h"
#include "RigidBodyConvexConeObject.h"
#include "RigidBodyConvexCylinderObject.h"


#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "RigidBodyConvexMeshObject.h"
#include "RigidBodyTriangleMeshObject.h"


#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

GLUquadric* gQuad;

btSoftRigidDynamicsWorld* gWorld;
btDispatcher* gDispatcher;
btBroadphaseInterface* gBroadphase;
btConstraintSolver* gConstraintSolver;
btCollisionConfiguration* gCollisionConfig;

btSoftBodySolver* gSoftBodySolver;
btSoftBodyWorldInfo gSoftBodyWorldInfo;


std::vector<GameObject*> gGameObjects;
BroadphaseNativeTypes buildType;

bool collisionCallbackFunc(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
{
	btVector3 point = cp.getPositionWorldOnA();
	HELPERS::GetObjectsInRadiusFromPoint(point, gGameObjects, 20);

	int g;
	GameObject* gO;
	gO = reinterpret_cast<GameObject*>(colObj0Wrap->getCollisionObject()->getUserPointer());

	if (gO->Tag() != "bomb")
		gO = reinterpret_cast<GameObject*>(colObj1Wrap->getCollisionObject()->getUserPointer());

	if (gO->Tag() != "bomb")
		return false;

	for (int i = 0; i < gGameObjects.size(); ++i)
	{
		if (gGameObjects[i] == gO)
			g = i;
	}

	if (!gO->isSoftBody()) gWorld->removeCollisionObject(reinterpret_cast<RigidBodyObject*>(gO)->Body());
	delete gO;
	gGameObjects.erase(gGameObjects.begin() + g);

	return false;
}

//Initialize SDL
void InitializeSDL()
{
	SDL_Init(SDL_INIT_EVERYTHING);
}

int main(int argc, char* args[]) //MAIN IS MESSY FOR TESTING PURPOSES
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(4677);
	Timer timer;
	InitializeSDL();
	Display display(1280, 720, "WindowTitle");

	Mesh meshBox("./Resources/Meshes/box.obj");
	Mesh meshTeapot("./Resources/Meshes/cube.obj");

	Texture texture("./Resources/Textures/fur.png");

	Shader shader("./Resources/Shaders/BasicShader");

	Shader shaderTexDiffuse("./Resources/Shaders/BasicShader");
	shader.SetColor(glm::vec4(1, 0, 1, 1));

	shaderTexDiffuse.SetDiffuseTexture(&texture);
	shaderTexDiffuse.SetOpacityTexture(&texture);

	Camera camera({ 0,5,-10 }, 70, float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.01f, 1000.0f, -3.1415 / 2);
	glm::vec3 cameraVel = glm::vec3();

	buildType = SPHERE_SHAPE_PROXYTYPE;
	gQuad = gluNewQuadric();

	//BULLET

	gCollisionConfig = new btSoftBodyRigidBodyCollisionConfiguration();
	gDispatcher = new btCollisionDispatcher(gCollisionConfig);
	gBroadphase = new btDbvtBroadphase();
	gConstraintSolver = new btSequentialImpulseConstraintSolver;

	gSoftBodySolver = new btDefaultSoftBodySolver();

	gWorld = new btSoftRigidDynamicsWorld(gDispatcher, gBroadphase, gConstraintSolver, gCollisionConfig, gSoftBodySolver);
	gWorld->setGravity(btVector3(0, -10, 0));

	gSoftBodyWorldInfo.m_dispatcher = gDispatcher;
	gSoftBodyWorldInfo.m_broadphase = gBroadphase;
	gSoftBodyWorldInfo.m_gravity = gWorld->getGravity();
	gSoftBodyWorldInfo.m_sparsesdf.Initialize();

	gContactAddedCallback = collisionCallbackFunc;

	glm::vec3 pos = { 0.0f,0.0f,0.0f };
	gGameObjects.push_back(new RigidBodyPlaneObject(0, &shader, pos, gWorld));
	bool isSpawningItem = false;
	bool mouseIsDown = false;

	pos = { 0.0f,20.0f,0.0f };
	gGameObjects.push_back(new RigidBodyConvexSphereObject(1, &shader, pos, 1, 1, gWorld));
	
	pos = { 5.0f,10.0f,0.0f };
	gGameObjects.push_back(new RigidBodyConvexCylinderObject(1, &shader, pos, 1, 5, 1, gWorld));
	
	pos = { -5.0f,20.0f,0.0f };
	gGameObjects.push_back(new RigidBodyConvexConeObject(7, &shader, pos, 1, 5, 1, gWorld));

	pos = { 15.0f,10.0f,3.0f };
	gGameObjects.push_back(new RigidBodyConvexBoxObject(1, &shader, pos, {2,2,2},&meshBox, 1, gWorld));

	pos = { -15.0f,10.0f,-3.0f };
	gGameObjects.push_back(new RigidBodyConvexMeshObject(1, &shader, pos, &meshTeapot, 1.0f, gWorld));

	pos = { 15.0f,10.0f,-6.0f };
	gGameObjects.push_back(new RigidBodyTriangleMeshObject(1, &shaderTexDiffuse, pos, &meshTeapot, 1.0f, gWorld));


	//TODO: Create separate classes for these
	HELPERS::CreateCloth(&shader, gWorld, &gSoftBodyWorldInfo, gGameObjects, 4, 15, 15);
	gGameObjects[gGameObjects.size() - 1]->SetColor(glm::vec4(1, 0, 0, 1));

	btVector3 rad = btVector3(3, 3, 3);
	btVector3 center = btVector3(10, 10, 10);
	HELPERS::CreateClothBall(&shader, gWorld, &gSoftBodyWorldInfo, gGameObjects, &rad, &center, 400);
	gGameObjects[gGameObjects.size() - 1]->SetColor(glm::vec4(1, 0, 0, 1));

	HELPERS::CreateClothMesh(&shader, gWorld, &gSoftBodyWorldInfo, gGameObjects, &meshTeapot, &btVector3(-40, 10, 0));
	gGameObjects[gGameObjects.size() - 1]->SetColor(glm::vec4(1, 0, 0, 1));

	while (display.IsOpen())
	{
		timer.Update();

		gSoftBodyWorldInfo.m_sparsesdf.GarbageCollect();
		gWorld->stepSimulation(timer.deltaTime());

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				display.Close();

			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_s:
					cameraVel.z = -.4f;
					break;
				case SDLK_w:
					cameraVel.z = .4f;
					break;
				case SDLK_a:
					cameraVel.x = -.4f;
					break;
				case SDLK_d:
					cameraVel.x = .4f;
					break;
				case SDLK_q:
					cameraVel.y = -.4f;
					break;
				case SDLK_e:
					cameraVel.y = .4f;
					break;
				case SDLK_f:
					if (HELPERS::RayCastHit(gWorld, camera.GetPosition(), camera.GetPosition() + camera.GetForward()*2000.0f))
					{
						btVector3 hP, n;
						auto hitObject = HELPERS::GetRayCastHitObject(gWorld, camera.GetPosition(), camera.GetPosition() + camera.GetForward()*2000.0f, n, hP);

						if (!hitObject->isSoftBody())
						{
							auto BT = BroadphaseNativeTypes(reinterpret_cast<RigidBodyObject*>(hitObject)->Body()->getCollisionShape()->getShapeType());
							if (BT != STATIC_PLANE_PROXYTYPE)
								buildType = BT;
						}
					}
					break;
				}
				if (e.key.keysym.sym == SDLK_p)
				{
					glm::vec3 spawn = HELPERS::ToGLMVec(&(camera.GetPosition() + camera.GetForward() * 2));
					auto gO = new RigidBodyConvexSphereObject(1, &shader, spawn, 1, 3, gWorld);
					gGameObjects.push_back(gO);
					glm::vec3 dir = camera.GetForward() * 20.0f;

					gO->SetTag("bomb");
					gO->SetColor(glm::vec4(1, 0.25, 0, 1));

					gO->Body()->setLinearVelocity(btVector3(dir.x, dir.y, dir.z));
					gO->Body()->setCollisionFlags(btCollisionObject::CollisionFlags::CF_CUSTOM_MATERIAL_CALLBACK | gO->Body()->getCollisionFlags());
				}
				if (e.key.keysym.sym == SDLK_SPACE)
				{
					glm::vec3 spawn = HELPERS::ToGLMVec(&(camera.GetPosition() + camera.GetForward() * 2));

					auto gO = new RigidBodyConvexSphereObject(1, &shader, spawn, 1, 3, gWorld);
					gGameObjects.push_back(gO);
					glm::vec3 look = camera.GetForward() * 20.0f;
					gO->Body()->setLinearVelocity(btVector3(look.x, look.y, look.z));
				}
			}

			if (e.type == SDL_KEYUP)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_s:
					cameraVel.z = 0;
					break;
				case SDLK_w:
					cameraVel.z = 0;
					break;
				case SDLK_a:
					cameraVel.x = 0;
					break;
				case SDLK_d:
					cameraVel.x = 0;
					break;
				case SDLK_q:
					cameraVel.y = 0;
					break;
				case SDLK_e:
					cameraVel.y = 0;
					break;
				}
			}

			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
				{
					isSpawningItem = true;
					glm::vec3 spawnPos = { 0,20,0 };
					if (buildType == SPHERE_SHAPE_PROXYTYPE)
					{
						gGameObjects.push_back(new RigidBodyConvexSphereObject(8, &shader, spawnPos, 1, 3, gWorld, false));
						gGameObjects[gGameObjects.size() - 1]->SetColor(glm::vec4(1, 1, 0, 1));
					}
					if (buildType == BOX_SHAPE_PROXYTYPE)
					{
						glm::vec3 extents = { 1,1,1 };
						auto gO = new RigidBodyConvexBoxObject(1, &shader, pos, extents, &meshBox, 3, gWorld);
						gGameObjects.push_back(gO);
						gO->SetColor(glm::vec4(1, 1, 0, 1));

					}
					if (buildType == CONE_SHAPE_PROXYTYPE)
					{
						auto gO = new RigidBodyConvexConeObject(1, &shader, pos, 1, 5, 3, gWorld);
						gGameObjects.push_back(gO);
						gO->SetColor(glm::vec4(1, 1, 0, 1));
					}
					if (buildType == CYLINDER_SHAPE_PROXYTYPE)
					{
						auto gO = new RigidBodyConvexCylinderObject(1, &shader, pos, 1, 5, 3, gWorld);
						gGameObjects.push_back(gO);
						gO->SetColor(glm::vec4(1, 1, 0, 1));
					}
				}

				if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
				{
					mouseIsDown = true;
				}
				SDL_WarpMouseInWindow(display.GetWindow(), WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
			}

			if (e.type == SDL_MOUSEBUTTONUP)
			{
				if (isSpawningItem)
				{
					gWorld->addRigidBody(reinterpret_cast<RigidBodyObject*>(gGameObjects[gGameObjects.size() - 1])->Body());
					gGameObjects[gGameObjects.size() - 1]->SetColor(glm::vec4(0, 0, 1, 1));
					isSpawningItem = false;
				}

				if (mouseIsDown && !(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)))
				{
					mouseIsDown = false;
				}
			}
		}

		camera.Move(camera.GetRight() * cameraVel.x);
		camera.Move(camera.GetUp() * cameraVel.y);
		camera.Move(camera.GetForward() * cameraVel.z);

		if (isSpawningItem)
		{
			if (HELPERS::RayCastHit(gWorld, camera.GetPosition(), camera.GetPosition() + camera.GetForward() * 2000.0f))
			{
				btVector3 hP, n;
				btTransform t;
				t.setIdentity();
				HELPERS::GetRayCastHit(gWorld, camera.GetPosition(), camera.GetPosition() + camera.GetForward() * 2000.0f, n, hP);
				t.setOrigin(hP + n);

				reinterpret_cast<RigidBodyObject*>(gGameObjects[gGameObjects.size() - 1])->Body()->setMotionState(new btDefaultMotionState(t));
			}
		}

		if (mouseIsDown)
		{
			float vel = 0.01f;
			int MidX = WINDOW_WIDTH / 2;
			int MidY = WINDOW_HEIGHT / 2;
			SDL_ShowCursor(SDL_DISABLE);
			int tmpx, tmpy;
			SDL_GetMouseState(&tmpx, &tmpy);
			//camYaw += mousevel*(MidX - tmpx);
			//camPitch += mousevel*(MidY - tmpy);
			float xChange = vel * (MidX - tmpx);
			float yChange = vel * (MidY - tmpy);
			camera.RotateX(yChange);
			camera.RotateY(xChange);
			SDL_WarpMouseInWindow(display.GetWindow(), MidX, MidY);
		}
		display.Clear();

		for (int i = 0; i < gGameObjects.size(); ++i)
			gGameObjects[i]->Draw(gQuad, &camera);

		display.Update();
	}

	for (int i = 0; i < gGameObjects.size(); ++i)
	{
		auto gO = gGameObjects[i];
		if (!gO->isSoftBody())
			gWorld->removeCollisionObject(reinterpret_cast<RigidBodyObject*>(gO)->Body());
		else
			gWorld->removeSoftBody(reinterpret_cast<SoftBodyObject*>(gO)->Body());

		delete gO;
	}

	gSoftBodyWorldInfo.m_sparsesdf.Reset();

	delete gSoftBodySolver;
	delete gDispatcher;
	delete gCollisionConfig;
	delete gBroadphase;
	delete gWorld;
	delete gConstraintSolver;

	SDL_Quit();
	return 0;
}