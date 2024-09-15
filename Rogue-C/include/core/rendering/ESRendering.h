#pragma once
#include "./include/core/rendering/SpriteManager.h"
#include "./include/core/Transform.h"
#include <cstdint>
#include <vector>
#include <array>
#include <map>

namespace Rendering {
    struct ThingToRender {
        SpriteID what;
        MTransform where;
        Color tint;

        ThingToRender(SpriteID id, const MTransform& tr, Color tint) : what(id), where(tr), tint(tint) { }
    };

    class ESRenderer {
    private:
        std::uint8_t currentQueue = 0;
        std::array<std::map<float, std::vector<ThingToRender>>, 2> renderQueues;

        std::uint8_t GetNextQueue();

    public:
        static void PushRenderFunc(float order, SpriteID sprite, const MTransform& transform, Color tint);
        static void Draw();
        static void Sync();
    };
}