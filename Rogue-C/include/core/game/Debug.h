#pragma once
#include "./include/core/serialization/Serializable.h"

namespace Serialization::Debug {
    void PrintHierarchy();

    void PPrint(int x, int& y, const Serialization::Node* whatToPrint, Entity entity);

    bool IMGUIButton(int x, int y, const char* text);
}