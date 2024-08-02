#pragma once
#include "ECS.h"
#include "Scene.h"
#include "./include/raylib/raylib.h"
#include <thread>
#include <barrier>
#include "AudioManager.h"
#include "SpriteManager.h"
#include "Animation.h"

class Game
{
private:
    float _mainDt = 1;
    float _physicsDt = 1;
    std::atomic<bool> _gameRunning;
    std::barrier<std::function<void()>> _barrier;

    std::vector<Scene> _scenes;
    int _currentScene;
    int _scheduledScene;

public:
    Game();
    ~Game();

    void Run();
    void AddScenes(std::vector<Scene> scenes);

private:
    static void ProcessMain(Game* instance);
    static void ProcessPhysics(Game* instance);
};
