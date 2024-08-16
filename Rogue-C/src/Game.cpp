#include "Game.h"
#include "SceneManager.h"
#include "Physics.h"
#include "Gradient.h"

namespace Core {
    namespace Debug {
        std::uint8_t benchmarkMode = 0;
        std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::_V2::system_clock::duration> updateClock = std::chrono::high_resolution_clock::now();
        std::uint32_t totalFrameTime = 0;
    }

    const int WIDTH = 800;
    const int HEIGHT = 450;

    extern std::uint32_t updateGridTime;
    extern std::uint32_t findCollisionsTime;
    extern std::uint32_t correctTime;

    Game::Game() : _gameRunning(true), _currentScene(0), _barrier(3,  [this]() { 
        Debug::totalFrameTime = (std::chrono::high_resolution_clock::now() - Debug::updateClock).count();
        Debug::updateClock = std::chrono::high_resolution_clock::now();
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
        SceneManager(this);
        SpriteManager::Init();
    }

    Game::~Game() {
        SpriteManager::UnloadAll();
        AudioManager::UnloadAll();
        CLOSE_LOG();
    }

    void Game::SetScene(std::uint8_t index) {
        _scheduledScene = index;
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
            if(IsKeyPressed(KEY_F1)) {
                Debug::benchmarkMode = 0;
            }

            if(IsKeyPressed(KEY_F2)) {
                Debug::benchmarkMode = 1;
            }

            if(IsKeyPressed(KEY_F3)) {
                Debug::benchmarkMode = 2;
            }

            if(IsKeyPressed(KEY_F4)) {
                Debug::benchmarkMode = 3;
            }

            if(IsKeyPressed(KEY_F5)) {
                Debug::benchmarkMode = 4;
            }

            if(IsKeyPressed(KEY_F6)) {
                Debug::benchmarkMode = 5;
            }


            BeginDrawing();
            ClearBackground(BLACK);
            
            ECS::Draw();

            Debug::DrawInfo();

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

    void Debug::DrawBar(const char* name, std::uint16_t y, std::uint32_t time, std::uint32_t totalTime, Color color) {
        float t = static_cast<float>(time) / static_cast<float>(totalTime);
        DrawRectangle(0, y + 1, lerp(0, 80, t), 8, ColorLerp(color, RED, t));
        DrawText(std::format("{}: {}", name, time / 1000).c_str(), 80, y, 10, WHITE);
    }

    void Debug::DrawBlock(const char* name, std::unordered_map<const char *, uint32_t>::iterator begin, std::unordered_map<const char *, uint32_t>::iterator end, std::uint32_t totalTime, std::uint16_t& y) {
        DrawText(name, 0, y, 10, WHITE);
        y += 10;
        std::unordered_map<const char *, uint32_t> arr(begin, end);
        for(auto const pair : arr) {
            Debug::DrawBar(pair.first, y, pair.second, totalTime, GREEN);
            y += 10;
        }
    }

    void Debug::DrawInfo() {
        if(Debug::benchmarkMode == 0) {
            return;
        }

        std::uint16_t size = 80;
        switch (Debug::benchmarkMode)
        {
        case 2: size += Debug::updateTimings.size() * 10; break;
        case 3: size += Debug::updateTimings.size() * 10; break;
        case 4: size += Debug::updateTimings.size() * 10; break;
        case 5: size += Debug::updateTimings.size() * 10 * 3 + 50; break;
        default: break;
        }

        std::uint16_t curHeight = 0;
        DrawRectangle(0, 0, 250, size, Color(0, 0, 0, 180));
        DrawText(std::format("FPS: {}", GetFPS()).c_str(), 0, 0, 10, WHITE);
        DrawText(std::format("entities: {}", ECS::GetEntityCount()).c_str(), 0, 10, 10, WHITE);

        curHeight = 20;
        switch (Debug::benchmarkMode)
        {
        case 2: Debug::DrawBlock("UPDATE:", Debug::updateTimings.begin(), Debug::updateTimings.end(), Debug::totalUpdateTime, curHeight); break;
        case 3: Debug::DrawBlock("PHYSICS:", Debug::physUpdateTimings.begin(), Debug::physUpdateTimings.end(), Debug::totalPhysicsTime, curHeight); break;
        case 4: Debug::DrawBlock("DRAW:", Debug::drawTimings.begin(), Debug::drawTimings.end(), Debug::totalDrawTime, curHeight); break;
        case 5: 
            Debug::DrawBlock("UPDATE:", Debug::updateTimings.begin(), Debug::updateTimings.end(), Debug::totalUpdateTime, curHeight);
            curHeight += 10;
            Debug::DrawBlock("PHYSICS:", Debug::physUpdateTimings.begin(), Debug::physUpdateTimings.end(), Debug::totalPhysicsTime, curHeight);
            curHeight += 10;
            Debug::DrawBlock("DRAW:", Debug::drawTimings.begin(), Debug::drawTimings.end(), Debug::totalDrawTime, curHeight);
            break;
        default: curHeight -= 10; break;
        }

        curHeight += 10;
        Debug::DrawBar("Update", curHeight, totalUpdateTime, Debug::totalFrameTime, GREEN);

        curHeight += 10;
        Debug::DrawBar("Physics", curHeight, totalPhysicsTime, Debug::totalFrameTime, YELLOW);

        curHeight += 10;
        Debug::DrawBar("Draw", curHeight, totalDrawTime, Debug::totalFrameTime, MAGENTA);
    }
}