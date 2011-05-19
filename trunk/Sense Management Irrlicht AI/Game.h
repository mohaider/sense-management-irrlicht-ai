#ifndef GAME_H
#define GAME_H

/*
	Game.h
	Shaun Mitchell
*/

#include <map>
#include <vector>
#include "GameObject.h"
#include "Singleton.h"
#include "IrrlichtScene.h"
#include "Wall.h"
#include <SDL.h>
#include <assert.h>
#include <list>

class GameState;
class AIObject;

class Game 
{
public:

	typedef std::map<int, GameObject*> GameObjects; // list of game objects
	typedef std::map<int, AIObject*> AIObjects; // need a separate list of AI objects as well
	typedef std::vector<ISceneNode*> BoundingBoxes;
	typedef std::vector<Wall*> Walls;
	typedef std::vector<ILightSceneNode*> Lights;

	~Game() {}

	void Init();
	void Clean();

	void ChangeState(GameState* state);
	void PushState(GameState* state);
	void PopState();

	void HandleEvents();
	void Update(float dt);
	void Draw();

	const int GetNextID() const { return m_nextID; }
	const int GetNextID_AI() const { return m_nextID_AI; }

	void IncrementNextID() { m_nextID++; }
	void IncrementNextID_AI() { m_nextID_AI++; }

	GameObjects& GetGameObjects() { return m_gameObjects; }
	void AddGameObject(GameObject* go) { m_gameObjects[GetNextID()] = go; }
	void AddGameObject(int id, GameObject* go) { assert(id >= GetNextID()); m_gameObjects[id] = go; }

	AIObjects& GetAIObjects() { return m_aiObjects; }
	void AddAIObject(AIObject* ao) { m_aiObjects[GetNextID_AI()] = ao; }
	void AddAIObject(int id, AIObject* ao) { assert(id >= GetNextID_AI()); m_aiObjects[id] = ao; }

	BoundingBoxes& GetBoundingBoxes() { return m_boundingBoxes; }

	AIObject* GetAIObject(int id) { return m_aiObjects[id]; }
	GameObject* GetGameObject(int id) { return m_gameObjects[id]; }

	Walls& GetWalls() { return m_walls; }
	Lights& GetLights() { return m_lights; }

	void MoveCameraControl();

	float GetCameraDirection() { return m_cameraDirection; }
	ICameraSceneNode* GetCamera() { return m_camera; }
	ICameraSceneNode* GetMiniMapCam() { return m_minimapCam; }

	IrrlichtScene* GetScene() { return m_pIrrScene; }

	std::list<vector2df>& GetPatrolPath() { return m_pPath; }

private: 

	Game() { m_nextID = 0; m_nextID_AI = 0; }

	friend class Singleton<Game>;
	
	std::vector<GameState*> states;

	GameObjects m_gameObjects;
	AIObjects m_aiObjects;
	BoundingBoxes m_boundingBoxes;
	Walls m_walls;
	Lights m_lights;

	std::list<vector2df> m_pPath;

	int m_nextID;
	int m_nextID_AI;

	ICameraSceneNode* m_camera;
	ICameraSceneNode* m_minimapCam;
	float m_cameraDirection;
	float m_zdirection;

	IrrlichtScene* m_pIrrScene;
};

typedef Singleton<Game> TheGame;

#endif