#pragma once
#include "Vec2.h"
#include <vector>
#include "SpriteManager.h"
#include "System.h"
#include "ECS.h"
#include "Timer.h"
#include "Drawer.h"

struct Animation {
    bool loop;
    float frameTime;
    std::vector<SpriteID> frames;

    Animation() = default;
    Animation(const TextureID sprite, const Vec2 size, const Vec2 start, const int framesCount);
    Animation(const TextureID sprite, const Vec2 size, const Vec2 start[]);
    Animation(const SpriteID start[]);
};

struct AnimationPlayer {
    int currentFrame;
    Timer time;
    Animation* currentAnimation;

    AnimationPlayer() = default;
    AnimationPlayer(Animation* animation);

    void Play(Animation* animation);
};

class AnimationPlayerSystem : public Core::System {
public:
    AnimationPlayerSystem();

    void Update(float dt);
    static void PlayAnimation(AnimationPlayer& player, Animation* animation);
};
