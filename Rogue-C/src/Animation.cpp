#include "./include/core/systems/Animation.h"
#include "./include/core/LOG.h"
#include <iterator>

using namespace Core;
using namespace Rendering;

Animation::Animation(const TextureID texture, const Vec2 size, const Vec2 start, int framesCountX, int framesCountY) : loop(true), frameTime(1.0f / 24.0f) {
    for(int y = 0; y < framesCountY; y++) {
        for(int x = 0; x < framesCountX; x++) {
            SpriteID id = SpriteManager::RegisterSprite(texture, { start.x + x * size.x, start.y + y * size.y, size.x, size.y });
            frames.push_back(id);
        }
    }
}

Animation::Animation(const TextureID texture, const Vec2 size, const Vec2 start[]) : loop(true), frameTime(1.0f / 24.0f) {
    for(int i = 0; i < (sizeof(start)/sizeof(*start)); i++) {
        SpriteID id = SpriteManager::RegisterSprite(texture, { start[i].x + i * size.x, start[i].y, size.x, size.y });
        frames.emplace_back(id);
    }
}

Animation::Animation(const SpriteID start[]) : loop(true), frameTime(1.0f / 24.0f)  {
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

            drawer.sprite = player.currentAnimation->frames[player.currentFrame];
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
