#include "./include/core/rendering/TileGrid.h"
#include "./include/core/Transform.h"

using namespace Core;

// TODO: I NEED TO LIKE MAKE A DOUBLE BUFFER WITH SORTED HASH_MAP I 
// DUNO AND ALL RENDERERS ARE SUBMITIG TO IT WHAT AND AT WHAT ORDER 
// THEY WANT TO RENDER THE THING AND THEN IT JUST YA KNOW WHAT? 
// TAKES AND RENDERS IT ON THE NEXT FRAME LOLLLLLLLL

TileGridSystem::TileGridSystem() {
    signature.set(Core::ECS::GetComponentType<TileGrid>());
    signature.set(Core::ECS::GetComponentType<MTransform>());
}

void RenderGrid(const TileGrid& grid, const MTransform& tr) {
    //float width = grid.size.x * tr.scale.x;
    for(std::uint16_t y = 0; y < grid.height; y++) {
        for(std::uint16_t x = 0; x < grid.width; x++) {
            const Sprite& sprite = SpriteManager::GetSprite(grid.tiles[(grid.width * y) + x]);
            const Texture2D& tex = SpriteManager::GetTexture(sprite.texture);

            DrawTexturePro(tex, sprite.rect, 
                { tr.position.x + ((float)x * tr.scale.x * 2), tr.position.y + ((float)y * tr.scale.y * 2), tr.scale.x * 2.0f, tr.scale.y * 2.0f },
                { tr.scale.x, tr.scale.y }, 0,  WHITE);
        }
    }
}

void TileGridSystem::Draw() {
    for(Entity entity : Entities) {
        RenderGrid(ECS::GetComponent<TileGrid>(entity), ECS::GetComponent<MTransform>(entity));
    }
}