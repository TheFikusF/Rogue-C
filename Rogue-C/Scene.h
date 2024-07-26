#pragma once
#include <functional>
#include "ECS.h"

class Scene
{
private:
    float mainDt = 1;
    float physicsDt = 1;
    
    bool started;
    std::function<void (void)> onStart;
    
    ECS* currentECS;

public:
    Scene(std::function<void (void)> onStart);
    ~Scene();

    void Start();
    void Clear();
};