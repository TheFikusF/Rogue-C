#include "Input.h"
#include "./include/raylib/raylib.h"

Input Input::_input;

void Input::Process(Vec2 center, float ds) {
    _input.movementAxis = Vec2(0, 0);
    if(IsKeyDown(KEY_A)) {
        _input.movementAxis.x = -1;
    }

    if(IsKeyDown(KEY_D)) {
        _input.movementAxis.x = 1;
    }

    if(IsKeyDown(KEY_W)) {
        _input.movementAxis.y = -1;
    }

    if(IsKeyDown(KEY_S)) {
        _input.movementAxis.y = 1;
    }

    _input.shootingAxis = Vec2(GetMouseX(), GetMouseY()) - center;
    _input.shootPressed = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    if(GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X) != 0 || GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y) != 0) {
        _input.shootingAxis = Vec2(GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X), GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y));
        _input.shootPressed = true;
    }
    
    if(_input.movementAxis.x == 0 && _input.movementAxis.y == 0) {
        _input.movementAxis = Vec2(GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X), GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y));
    }

    _input.movementAxis = _input.movementAxis.GetNormalized();
    _input.shootingAxis = _input.shootingAxis.GetNormalized();

}

const Vec2 Input::GetMovementAxis() {
    return _input.movementAxis;
}

const Vec2 Input::GetShootingAxis() {
    return _input.shootingAxis;
}

const bool Input::IsShooting() {
    return _input.shootPressed;
}
