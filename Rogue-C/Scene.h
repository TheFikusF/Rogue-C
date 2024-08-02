#pragma once
#include <functional>
#include "ECS.h"

class Scene
{
private:
    bool started;
    std::function<void (void)> onStart;

public:
    Scene(std::function<void (void)> onStart);
    ~Scene();

    void Start();
    void Clear();
};