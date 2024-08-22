#pragma once
#include "./include/core/game/Game.h"

namespace Core {
    class SceneManager
    {
    private:
        static Game* _game;
        SceneManager(Game* game);

    public:
        static void SetScene(std::uint8_t index);

        friend class Game;
    };
}
