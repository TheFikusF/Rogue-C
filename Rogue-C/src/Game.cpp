#include "Game.h"

const int WIDTH = 800;
const int HEIGHT = 450;

Game::Game() : _gameRunning(true), _currentScene(0), _barrier(3,  [this]() { 
    if(this->_scheduledScene > -1 && this->_scheduledScene < this->_scenes.size()) {
        this->_scenes[this->_currentScene].Clear();
        this->_scenes[this->_scheduledScene].Start();
        this->_currentScene = this->_scheduledScene;
        this->_scheduledScene = -1;
        return;
    }
    ECS::FreeBin(); 
}), _scheduledScene(-1) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    InitWindow(WIDTH, HEIGHT, "Rogue-C");
    InitAudioDevice();
    MaximizeWindow();
    OPEN_LOG();

    SetRandomSeed(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
    SpriteManager::Init();
}

Game::~Game() {
    SpriteManager::UnloadAll();
    AudioManager::UnloadAll();
    CLOSE_LOG();
}

void Game::AddScenes(std::vector<Scene> scenes) {
    _scenes = scenes;
}

void Game::Run() {
    if(_scenes.size() == 0) {
        LOG_ERROR("THERE ARE NO SCENES");
        return;
    }
    
    _scenes[_currentScene].Start();

    std::thread mainThread(ProcessMain, this); 
    std::thread physicsThread(ProcessPhysics, this); 
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        
        ECS::Draw();

        DrawText(std::format("FPS: {}", GetFPS()).c_str(), 0, 0, 10, WHITE);

        EndDrawing();
        _barrier.arrive_and_wait();
    }
    
    _gameRunning = false;
    
    CloseWindow(); 

    mainThread.join();
    physicsThread.join();
}

void Game::ProcessMain(Game* instance) {
    float previousTime = GetTime();
    while (instance->_gameRunning) {
        float currentTime = GetTime();
        instance->_mainDt = currentTime - previousTime;
        previousTime = currentTime;
        
        ECS::Update(instance->_mainDt);
        
        instance->_barrier.arrive_and_wait();
    }

    instance->_gameRunning = false;
}

void Game::ProcessPhysics(Game* instance) {
    float previousTime = GetTime();
    while (instance->_gameRunning) {
        float currentTime = GetTime();
        instance->_physicsDt = currentTime - previousTime;
        previousTime = currentTime;

        ECS::PhysicsUpdate(instance->_physicsDt);

        instance->_barrier.arrive_and_wait();
    }

}