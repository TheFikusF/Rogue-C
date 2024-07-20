#pragma once
#include "Vec2.h"
#include <vector>
#include "SpriteManager.h"
#include "System.h"
#include "ECS.h"
#include "Timer.h"
#include "Drawer.h"

struct Frame {
    Vec2 size;
    Vec2 position;
    Sprite sprite;
};

struct Animation {
    bool loop;
    float frameTime;
    std::vector<Frame> frames;

    Animation() = default;
    Animation(const Sprite& sprite, const Vec2& size, const Vec2& start, const int framesCount);
    Animation(const Sprite& sprite, const Vec2& size, const Vec2 start[]);
    Animation(const Frame start[]);
};

struct AnimationPlayer {
    int currentFrame;
    Timer time;
    Animation* currentAnimation;

    AnimationPlayer() = default;
    AnimationPlayer(Animation* animation);

    void Play(Animation* animation);
};

class AnimationPlayerSystem : public System {
public:
    AnimationPlayerSystem();

    void Update(float dt);
    static void PlayAnimation(AnimationPlayer& player, Animation* animation);
};
