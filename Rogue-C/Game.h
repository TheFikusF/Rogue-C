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
    std::barrier<void(*)()> _barrier;

    Scene* _scenes;
    int _scenesCount;
    int _currentScene;

public:
    Game();
    ~Game();

    void Run();
    void AddScenes(Scene* scenes, int scenesCount);

private:
    static void ProcessMain(Game* instance);
    static void ProcessPhysics(Game* instance);
};
