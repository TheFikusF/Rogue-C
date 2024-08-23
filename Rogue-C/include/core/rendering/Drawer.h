#pragma once
#include "./include/core/ecs/System.h"
#include "./include/core/rendering/SpriteManager.h"
#include "./include/raylib/raylib.h"

namespace Rendering {
    enum class DrawOrder {
        YAscending,
        YDescending,
        CustomOrder,
    };

    struct Drawer {
        Color color;
        SpriteID sprite;
        ShaderID shader;
        float order;

        Drawer();
        Drawer(const Color& color);
        Drawer(const SpriteID& sprite);
        Drawer(const SpriteID& sprite, const Color& color);
    };

    class DrawerSystem : public Core::System {
    public:
        DrawOrder drawOrder;
        std::uint32_t drawTime;

    public:
        DrawerSystem();
        
        void Draw() override;

    private:
        std::vector<Entity> _entitiesTemp;

    };

    bool SortYAsc(Entity a, Entity b);
    bool SortYDesc(Entity a, Entity b);
    bool SortCustom(Entity a, Entity b);
}
