#include "ResourceManager.h"

ResourceManager* ResourceManager::sInst = nullptr;	//Initalize the RM Singleton

ResourceManager::ResourceManager()
{
	//Register Resources
}

bool ResourceManager::RegisterResource(Actor& actor, int ID)
{
	if (ActorResources.at(ID) == nullptr) {
		ActorResources.insert(std::pair<int, Actor*>(ID, &actor));
		return true;
	}
	
	return false;
}

void ResourceManager::Render()
{
	for (size_t i = 0; i <= ActorResources.size(); i++) {
		ActorResources.at(i).
	}
}
