#include "Animation.h"
#include "LOG.h"
#include <iterator>

Animation::Animation(const Sprite& sprite, const Vec2& size, const Vec2& start, const int framesCount) : loop(true), frameTime(1.0f / 24.0f) {
    for(int i = 0; i < framesCount; i++) {
        frames.emplace_back(size, Vec2(start.x + i * size.x, start.y), sprite);
    }
}

Animation::Animation(const Sprite& sprite, const Vec2& size, const Vec2 start[]) : loop(true), frameTime(1.0f / 24.0f) {
    for(int i = 0; i < (sizeof(start)/sizeof(*start)); i++) {
        frames.emplace_back(size, start[i], sprite);
    }
}

Animation::Animation(const Frame start[]) : loop(true), frameTime(1.0f / 24.0f)  {
    for(int i = 0; i < (sizeof(start)/sizeof(*start)); i++) {
        frames.emplace_back(start[i]);
    }
}

AnimationPlayer::AnimationPlayer(Animation* animation) {
    AnimationPlayerSystem::PlayAnimation(*this, animation);
}

AnimationPlayerSystem::AnimationPlayerSystem() {
    signature.set(ECS::GetComponentType<Drawer>());
    signature.set(ECS::GetComponentType<AnimationPlayer>());
}

void AnimationPlayerSystem::Update(float dt) {
    for(auto const& entity : Entities) {
        AnimationPlayer& player = ECS::GetComponent<AnimationPlayer>(entity);
        Drawer& drawer = ECS::GetComponent<Drawer>(entity);

        if(player.time.Check(dt)) {
            player.currentFrame++;
            if(player.currentFrame >= player.currentAnimation->frames.size()) {
                if(player.currentAnimation->loop) {
                    player.currentFrame = 0;
                } else {
                    player.currentFrame = player.currentAnimation->frames.size() - 1;
                    continue;
                }
            }

            const Frame& currentFrame = player.currentAnimation->frames[player.currentFrame];
            
            drawer.sourceRect = { currentFrame.position.x, currentFrame.position.y, currentFrame.size.x, currentFrame.size.y };
            drawer.sprite = currentFrame.sprite;
            player.time.Start();
        }
    }
}

void AnimationPlayerSystem::PlayAnimation(AnimationPlayer& player, Animation* animation) {
    player.currentFrame = 0;
    player.time = animation->frameTime;
    player.currentAnimation = animation;
    player.time.Start();
}
