#pragma once
#include "./include/core/serialization/Serializable.h"
#include "./include/core/ecs/System.h"
#include "./include/core/rendering/SpriteManager.h"
#include "./include/raylib/raylib.h"

namespace Rendering {
    enum class DrawOrder {
        YAscending,
        YDescending,
        CustomOrder,
    };

    struct SerializedColor : public Serialization::Serializable {
        std::uint8_t r, g, b, a;

        SerializedColor() : r(0), g(0), b(0), a(0) { }
        SerializedColor(Color color) : r(color.r), g(color.g), b(color.b), a(color.a) { }

        Color ToRLColor();

        void Read(const Serialization::Node* current) override;
        void Write(Serialization::Node* current) override;
    };

    struct Drawer : public Serialization::Serializable {
        Color color;
        SpriteID sprite;
        ShaderID shader;
        float order;

        Drawer();
        Drawer(const Color& color);
        Drawer(const SpriteID& sprite);
        Drawer(const SpriteID& sprite, const Color& color);

        void Read(const Serialization::Node* current) override;
        void Write(Serialization::Node* current) override;
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
