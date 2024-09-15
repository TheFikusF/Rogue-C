#include "./include/core/rendering/ESRendering.h"

// TODO: Change callback towards storage of SpriteIDs and MTransforms and call it something like a "draw call"
using namespace Rendering;

static ESRenderer renderer;

std::uint8_t ESRenderer::GetNextQueue() {
    return ((renderer.currentQueue + 1) >= 2) ? 0 : renderer.currentQueue;
}

void ESRenderer::PushRenderFunc(float order, SpriteID sprite, const MTransform &tr, Color tint) {
    renderer.renderQueues[renderer.currentQueue][order].emplace_back(sprite, tr, tint);
}

void ESRenderer::Draw() {
    for(auto const& [order, queue] : renderer.renderQueues[renderer.GetNextQueue()]) {
        for(auto const& thing : queue) {
            const Sprite& sprite = SpriteManager::GetSprite(thing.what);
            const Texture2D& tex = SpriteManager::GetTexture(sprite.texture);

            Vector2 scale(thing.where.scale.x / tex.width, thing.where.scale.y / tex.height);
            
            //BeginShaderMode(SpriteManager::GetShader(drawer.shader));
            DrawTexturePro(tex, sprite.rect, 
                { thing.where.position.x, thing.where.position.y, thing.where.scale.x * 2.0f, thing.where.scale.y * 2.0f },
                { thing.where.scale.x, thing.where.scale.y }, thing.where.rotation,  thing.tint);
            
            //EndShaderMode();
        }
    }
}

void ESRenderer::Sync() {
    renderer.currentQueue = renderer.GetNextQueue();
    renderer.renderQueues[renderer.currentQueue].clear();
}