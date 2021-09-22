#pragma once
#include "Entity.h"
#include <algorithm>
#include <vector>

class EntityPool {
public:
    EntityPool();
    ~EntityPool();

    virtual void Allocate(Entity* entity, int priority);
    virtual void Update(float dt);
    
private:
    static const int EntityLimit{ 500 };
    std::vector<std::pair<Entity*, int>> Entities;
};