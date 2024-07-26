#include "Game.h"

const int WIDTH = 800;
const int HEIGHT = 450;

auto barrier_function = []() { ECS::FreeBin(); };

Game::Game() : _gameRunning(true), _currentScene(0), _scenesCount(0), _barrier(3,  barrier_function) {
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

    delete[] _scenes;
}

void Game::AddScenes(Scene* scenes, int scenesCount) {
    _scenesCount = scenesCount;
    _scenes = scenes;
}

void Game::Run() {
    if(_scenesCount == 0) {
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
        //drawerSystem->Update();
        //particleSystem->Draw();
        
#ifdef DEBUG_PANEL
        uint32_t sum = physicsSystem->findTime + physicsSystem->resolveTime + physicsSystem->correctTime;
        uint32_t totalT = std::max(std::max(sum, total), drawerSystem->drawTime);
        totalT = std::max(totalT, 1u);
        total = std::max(total, 1u);
        sum = std::max(sum, 1u);

        DrawRectangle(0, 0, 100, 130, Color(0, 0, 0, 80));
        DrawText(std::format("entityCount: {}", ECS::GetEntityCount()).c_str(), 0, 10, 10, WHITE);
        DrawText(std::format("player: {}%, {}", playerTime * 100 / total, (bulletClock - playerClock).count()).c_str(), 0, 20, 10, WHITE);
        DrawText(std::format("bullet: {}%, {}", bulletTime * 100 / total, (spheresClock - bulletClock).count()).c_str(), 0, 30, 10, WHITE);
        DrawText(std::format("spheres: {}%, {}", spheresTime * 100 / total, (enemyClock - spheresClock).count()).c_str(), 0, 40, 10, WHITE);
        DrawText(std::format("enemy: {}%, {}", enemyTime * 100 / total, (endClock - enemyClock).count()).c_str(), 0, 50, 10, WHITE);
        DrawText(std::format("total: {}%, {}", total * 100 / totalT, total).c_str(), 0, 60, 10, WHITE);
        DrawText("----------------", 0, 70, 10, WHITE);
        DrawText(std::format("drawer: {}%, {}", drawerSystem->drawTime * 100 / totalT, drawerSystem->drawTime).c_str(), 0, 80, 10, WHITE);
        DrawText("----------------", 0, 90, 10, WHITE);
        uint32_t a = physicsSystem->findTime * 100 / sum;
        uint32_t b = physicsSystem->resolveTime * 100 / sum;
        uint32_t c = physicsSystem->correctTime * 100 / sum;
        DrawText(std::format("find: {}%, {}", a, physicsSystem->findTime).c_str(), 0, 100, 10, WHITE);
        DrawText(std::format("resolve: {}%, {}", b, physicsSystem->resolveTime).c_str(), 0, 110, 10, WHITE);
        DrawText(std::format("correct: {}%, {}", c, physicsSystem->correctTime).c_str(), 0, 120, 10, WHITE);
        DrawText(std::format("total: {}%, {}", sum * 100 / totalT , sum).c_str(), 0, 130, 10, WHITE);
        DrawText("----------------", 0, 140, 10, WHITE);
#endif

        //const Player& playerComp = ECS::GetComponent<Player>(player);
        DrawText(std::format("FPS: {}", GetFPS()).c_str(), 0, 0, 10, WHITE);
        //DrawText(std::format("H: {}|{}", playerComp.health.current, playerComp.health.max).c_str(), 0, 10, 30, WHITE);
        EndDrawing();
        _barrier.arrive_and_wait();
    }
    
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
        
        //Input::Process(ECS::GetComponent<MTransform>(0).position, mainDt);
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