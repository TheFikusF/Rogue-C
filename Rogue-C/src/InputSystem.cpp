#include "./include/core/systems/InputSystem.h"
#include "./include/core/systems/CameraControl.h"
#include "./include/core/game/SceneManager.h"
#include "./include/raylib/raylib.h"

using namespace Core;

void InputSystem::Update(float ds) {
    _movementAxis = Vec2(0, 0);
    if(IsKeyDown(KEY_A)) {
        _movementAxis.x = -1;
    }

    if(IsKeyDown(KEY_D)) {
        _movementAxis.x = 1;
    }

    if(IsKeyDown(KEY_W)) {
        _movementAxis.y = -1;
    }

    if(IsKeyDown(KEY_S)) {
        _movementAxis.y = 1;
    }

    if(IsKeyDown(KEY_ONE)) {
        SceneManager::SetScene(1);
    }

    if(IsKeyDown(KEY_ZERO)) {
        SceneManager::SetScene(0);
    }

    const MTransform& tr = ECS::GetComponent<MTransform>(_player);
    _shootingAxis = Vec2(GetMouseX(), GetMouseY()) - CameraContorl::GetWorldToScreen(tr.position);
    _shootPressed = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    if(GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X) != 0 || GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y) != 0) {
        _shootingAxis = Vec2(GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X), GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y));
        _shootPressed = true;
    }
    
    if(_movementAxis.x == 0 && _movementAxis.y == 0) {
        _movementAxis = Vec2(GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X), GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y));
    }

    _movementAxis = _movementAxis.GetNormalized();
    _shootingAxis = _shootingAxis.GetNormalized();
}

const Vec2 InputSystem::GetMovementAxis() {
    return ECS::GetSystem<InputSystem>().lock()->_movementAxis;
}

const Vec2 InputSystem::GetShootingAxis() {
    return ECS::GetSystem<InputSystem>().lock()->_shootingAxis;
}

const bool InputSystem::IsShooting() {
    return ECS::GetSystem<InputSystem>().lock()->_shootPressed;
}

void InputSystem::SetPlayer(Entity player) {
    ECS::GetSystem<InputSystem>().lock()->_player = player;
}
