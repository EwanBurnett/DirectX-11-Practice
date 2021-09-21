#pragma once
#include <map>	
#include <list>
#include "AppIncl.h"	//For DirectX stuff
#include "Actor.h"	//For Actor Reference


class ResourceManager {

public:
	ResourceManager();
	~ResourceManager() = default;

	//Registers the actor in the resource manager. Actors with lower IDs will be updated first.
	bool RegisterResource(Actor& actor, int ID);

	static Actor& LoadActor();
	bool DestroyActor(int ID);

	void Init();
	void Update(float dt);

	void Render();
private:
	std::map<int, Actor*, std::less<int>> ActorResources;
	std::list<int> RenderQueue;

	//ResourceManager Singleton
	static ResourceManager* sInst;
};
