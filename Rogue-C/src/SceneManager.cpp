#include "./include/core/game/SceneManager.h"

namespace Core {
    Game* SceneManager::_game;

    SceneManager::SceneManager(Game* game) {
        _game = game;
    }

    void SceneManager::SetScene(std::uint8_t index) {
        _game->SetScene(index);
    }
}