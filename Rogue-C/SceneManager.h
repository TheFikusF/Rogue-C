#pragma once
#include "Game.h"

class SceneManager
{
private:
    static Game* _game;
    SceneManager(Game* game);

public:
    static void SetScene(std::uint8_t index);

    friend class Game;
};
