#include "./include/core/rendering/ESRendering.h"
#include "./include/core/systems/CameraControl.h"

using namespace Rendering;

static ESRenderer renderer;
static ESRenderer uiRenderer;

std::uint8_t ESRenderer::GetNextQueue() {
    return ((renderer.currentQueue + 1) >= 2) ? 0 : renderer.currentQueue;
}

void ESRenderer::Push(float order, SpriteID sprite, const MTransform &tr, Color tint) {
    renderer.renderQueues[renderer.currentQueue][order].emplace_back(sprite, tr, tint);
}

void ESRenderer::PushUI(float order, SpriteID sprite, const MTransform &tr, Color tint) {
    uiRenderer.renderQueues[uiRenderer.currentQueue][order].emplace_back(sprite, tr, tint);
}

void ESRenderer::Draw() {
    for(auto& [order, queue] : renderQueues[GetNextQueue()]) {
        std::sort(queue.begin(), queue.end(), [](const ThingToRender& a, const ThingToRender& b) -> bool {
            return a.where.position.y < b.where.position.y;
        });

        for(auto const& thing : queue) {
            const Sprite& sprite = SpriteManager::GetSprite(thing.what);
            const Texture2D& tex = SpriteManager::GetTexture(sprite.texture);

            Vector2 scale(thing.where.scale.x / tex.width, thing.where.scale.y / tex.height);
            
            DrawTexturePro(tex, sprite.rect, 
                { thing.where.position.x, thing.where.position.y, thing.where.scale.x * 2.0f, thing.where.scale.y * 2.0f },
                { thing.where.scale.x, thing.where.scale.y }, thing.where.rotation,  thing.tint);
            
        }
    }
}

void ESRenderer::DrawAll() {
    ClearBackground(BLACK);
    BeginMode2D(CameraContorl::GetCurrent());
    renderer.Draw();
    EndMode2D();

    uiRenderer.Draw();
}

void ESRenderer::Sync() {
    renderer.currentQueue = renderer.GetNextQueue();
    renderer.renderQueues[renderer.currentQueue].clear();

    uiRenderer.currentQueue = uiRenderer.GetNextQueue();
    uiRenderer.renderQueues[uiRenderer.currentQueue].clear();
}