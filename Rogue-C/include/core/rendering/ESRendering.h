#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include <map>

namespace Rendering {
    class ESRenderer {
    private:
        std::uint8_t currentQueue = 0;
        std::array<std::map<float, std::vector<void (*) (void)>>, 2> renderQueues;

        static std::uint8_t GetNextQueue();

    public:
        static void PushRenderFunc(float order, void (*func) (void));
        static void Draw();
        static void Sync();
    };
}