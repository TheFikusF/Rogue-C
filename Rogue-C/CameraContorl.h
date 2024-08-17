#pragma once
#include "./include/raylib/raylib.h"
#include "Vec2.h"
#include "System.h"

namespace CameraContorl {
    void Init();
    void Stop();

    void SetSmooth(float smoothTime);
    void SetTarget(Entity target);
    void SetPosition(Vec2 position);
    void SetZoom(float zoom);
    void SetRotation(float rotation);
    Vec2 GetWorldToScreen(Vec2 position);
    Vec2 GetScreenToWorld(Vec2 position);

    Camera2D GetCurrent();

    class CameraSystem : public Core::System {
        void Update(float dt) override;
    };
}