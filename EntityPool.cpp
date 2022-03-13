#include "EntityPool.h"

EntityPool::EntityPool()
{
    std::pair<Entity*, float>a(new Entity(), -1);
    Entities.reserve(EntityLimit);
    for (int i = 0; i < EntityLimit; ++i) {
        Entities.push_back(a);
    }
}

EntityPool::~EntityPool()
{
    Entities.clear();   //Clean up entity pool
}

void EntityPool::Allocate(Entity* entity, int priority)
{
    for (int i = 0; i < EntityLimit; i++) {
        Entity* ent = Entities.at(i).first;
        UINT pri = Entities.at(i).second;

        std::pair<Entity*, int> found;
        found.first = entity;
        found.second = priority;

        //If there's a free entity, set and activate it
        if (!ent->IsActive()) {
            entity->Init();

            found.first->Init();
            found.first->SetActive(true);
            found.first->SetID(i);

            Entities.at(i) = found;
    
            return;
        }

        //Else, find an entity of a lower priority, and replace it.
        else {
            if (priority >= pri) {
                Entities.at(i) = found;    //Set the new entity pair
            }
        }
    }
}

void EntityPool::Update(float dt)
{
    //Update each entity that's active in the pool
    for (int i = 0; i < EntityLimit; i++) {
        Entity* e = Entities.at(i).first;

        if (e->IsActive()) {
            e->Update(dt);
        }
        
    }
}


