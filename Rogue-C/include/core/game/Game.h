#pragma once
#include "./include/core/rendering/SpriteManager.h"
#include "./include/core/audio/AudioManager.h"
#include "./include/core/game/Scene.h"
#include "./include/core/ecs/ECS.h"
#include "./include/raylib/raylib.h"
#include <barrier>
#include <thread>

namespace Core{
    namespace Debug {
        void DrawBar(const char* name, std::uint16_t y, std::uint32_t time, std::uint32_t totalTime, Color color);
        void DrawBlock(const char* name, 
            std::unordered_map<const char *, uint32_t>::iterator begin, std::unordered_map<const char *, uint32_t>::iterator end, 
            std::uint32_t totalTime, std::uint16_t& y);
        
        void DrawInfo();
    }

    class Game
    {
    private:
        float _mainDt = 1;
        float _physicsDt = 1;
        std::atomic<bool> _gameRunning;
        std::barrier<void (*)(void) noexcept> _barrier;

        std::vector<Scene> _scenes;
        int _currentScene;
        int _scheduledScene;

    public:
        Game();
        ~Game();

        void Run();
        void AddScenes(std::vector<Scene> scenes);
        void SetScene(std::uint8_t index);
        void Sync() noexcept;

    private:
        static void ProcessMain(Game* instance);
        static void ProcessPhysics(Game* instance);

    };
}
