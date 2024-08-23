#include "./include/core/systems/CameraControl.h"
#include "./include/core/MathF.h"
#include "./include/core/Transform.h"

float _smoothTime = 0.1f;
Entity _target;
Camera2D _camera;

void CameraContorl::Init() {
    _smoothTime = 0;
    _target = MAX_ENTITIES;

    float width = GetRenderWidth();
    float height = GetRenderHeight();

    _camera = { 0 };
    _camera.zoom = 1.0f;
    _camera.rotation = 0.0f;
    _camera.target = { width / 2, height / 2 };
    _camera.offset = { width / 2, height / 2 };
}

void CameraContorl::Stop() {
    _target = MAX_ENTITIES;
}

void CameraContorl::SetSmooth(float smoothTime) {
    _smoothTime = smoothTime;
}

void CameraContorl::SetTarget(Entity target) { 
    _target = target; 
}

void CameraContorl::SetPosition(Vec2 position) {
    _camera.target = { position.x , position. y };
}

void CameraContorl::SetZoom(float zoom) {
    _camera.zoom = zoom;
}

void CameraContorl::SetRotation(float rotation) {
    _camera.rotation = rotation;
}

Vec2 CameraContorl::GetWorldToScreen(Vec2 position) { 
    Vector2 pos = GetWorldToScreen2D({ position.x, position. y}, _camera);
    return Vec2(pos.x, pos.y); 
}

Vec2 CameraContorl::GetScreenToWorld(Vec2 position) { 
    Vector2 pos = GetScreenToWorld2D({ position.x, position. y}, _camera);
    return Vec2(pos.x, pos.y); 
}

Camera2D CameraContorl::GetCurrent() { 
    return _camera;
}

void CameraContorl::CameraSystem::Update(float dt) {
    if(_target >= MAX_ENTITIES) {
        return;
    }

    MTransform const& tr = Core::ECS::GetComponent<MTransform>(_target);
    Vec2 cameraPos(_camera.target.x, _camera.target.y);
    Vec2 pos = Vec2::MoveTowards(cameraPos, tr.position, (Vec2::Distance(tr.position, cameraPos) / _smoothTime) * dt);
    _camera.target = { pos.x, pos.y };
}
