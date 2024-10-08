#pragma once
#include "./include/core/ecs/ECS.h"
#include <functional>

namespace Core {
    class Scene
    {
    private:
        bool started;
        std::function<void (void)> registerComponents;
        std::function<void (void)> onStart;

    public:
        Scene(std::function<void (void)> registerComponents, std::function<void (void)> onStart);
        ~Scene();

        void Start();
        void Clear();
    };
}