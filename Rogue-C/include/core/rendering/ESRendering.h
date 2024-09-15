#pragma once
#include "./include/core/rendering/SpriteManager.h"
#include "./include/core/Transform.h"
#include <cstdint>
#include <vector>
#include <array>
#include <map>

// ===============================================================~
// [ ES in ESRenderer primarily stands for Esoteric Scenes ] ::: =~
// ===============================================================~
//
// -/=-------------------------------------=\-
// -|=|[=============[ VII ]=============]|=|-
// -|      |*  *. '  _ / \ _ . * ' * |      |-
// -| *    |=.----''  /^W^\  ''----.=| `    |-
// -|    ` | \      ,(=====),      / |   *  |-
// -|  .   | |     ,(|o . o|),     | |      |-
// -|      | \     ,/\  -  /\,     / | *    |-
// -|    * ^^^  /""/=\\` `//=\""\  ^^^      |-
// -|      ||  /* /#/==^x^==\#\ *\  ||   .  |-
// -| .  ` ||  |/#||   [_]   ||#\|  ||      |-
// -|      ||  /=\||  ((+))  ||/=\  || `    |-
// -|   *  ||  | | |         | | |  ||      |-
// -| `    ||  | | [::=======] | |  ||  `   |-
// -|      ||  #7  // :::\  \\  7#  ||      |-
// -|   == /------// //  \::::]------\ ==   |-
// -|  /:| |/     ,~~_              \| |:\  |- 
// -|  |=| |      |/\ =_ _ ~         | |=|  |-
// -|  |:| |       _( )_( )\~~       | |:|  |-
// -| [|=|=|       \,\  _|\ \~~~     |=|=|] |-
// -|  |:| |\         \`   \        /| |:|  |-
// -|  |=|  \_         `    `      _/  |=|  |-
// -|  \:|    '\:==-----------==:/'    |:/  |-
// -|   ==                             ==   |-
// -:=-------------------------------------=:-
// -|=|[======[ SCYTHIAN  CHARIOT ]======]|=|-
// -\=-------------------------------------=/-
//
// ===========================================~

namespace Rendering {
    struct ThingToRender {
        SpriteID what;
        MTransform where;
        Color tint;

        ThingToRender() { }
        ThingToRender(SpriteID id, const MTransform& tr, Color tint) : what(id), where(tr), tint(tint) { }
    };

    class ESRenderer {
    private:
        std::uint8_t currentQueue = 0;
        std::array<std::map<float, std::vector<ThingToRender>>, 2> renderQueues;

        void Draw();
        std::uint8_t GetNextQueue();

    public:
        static void Push(float order, SpriteID sprite, const MTransform& transform, Color tint);
        static void PushUI(float order, SpriteID sprite, const MTransform& transform, Color tint);
        static void DrawAll();
        static void Sync();
    };
}