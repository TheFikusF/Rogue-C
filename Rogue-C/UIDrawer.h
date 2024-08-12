#pragma once
#include <unordered_set>
#include <Drawer.h>
#include "ECS.h"

struct IPointerDown {
    virtual void PointerDown(Entity entity) = 0;
};

struct IPointerUp {
    virtual void PointerUp(Entity entity) = 0;
};

struct IPointerHover {
    virtual void PointerHover(Entity entity) = 0;
};

struct UIDrawer : public Drawer {
    bool clickable;
    std::unordered_set<IPointerDown*> pointerDowns;
    std::unordered_set<IPointerHover*> pointerHovers;
    std::unordered_set<IPointerUp*> pointerUps;

    UIDrawer() : Drawer() {}
    UIDrawer(Color color) : Drawer(color) {}
};

template<typename T>
class PointerCallbackSystem : public System {
public:
    PointerCallbackSystem() {
        signature.set(ECS::GetComponentType<T>());
    }

    void OnEntityAdded(Entity entity) override {
        if(ECS::HasComponent<T>(entity) == false) {
            return;
        }

        UIDrawer& drawer = ECS::GetComponent<UIDrawer>(entity);
        if(std::derived_from<T, IPointerDown>) {
            T& pointerDown = ECS::GetComponent<T>(entity);
            drawer.pointerDowns.insert((IPointerDown*)&pointerDown);
        }

        if(std::derived_from<T, IPointerUp>) {
            T& pointerDown = ECS::GetComponent<T>(entity);
            drawer.pointerUps.insert((IPointerUp*)&pointerDown);
        }

        if(std::derived_from<T, IPointerHover>) {
            T& pointerDown = ECS::GetComponent<T>(entity);
            drawer.pointerHovers.insert((IPointerHover*)&pointerDown);
        }
    }
    
    void OnEntityRemoved(Entity entity) override {
        if(ECS::HasComponent<T>(entity) == false) {
            return;
        }

        UIDrawer& drawer = ECS::GetComponent<UIDrawer>(entity);
        if(std::derived_from<T, IPointerDown>) {
            T& pointerDown = ECS::GetComponent<T>(entity);
            drawer.pointerDowns.erase((IPointerDown*)&pointerDown);
        }

        if(std::derived_from<T, IPointerUp>) {
            T& pointerDown = ECS::GetComponent<T>(entity);
            drawer.pointerUps.erase((IPointerUp*)&pointerDown);
        }

        if(std::derived_from<T, IPointerHover>) {
            T& pointerDown = ECS::GetComponent<T>(entity);
            drawer.pointerHovers.erase((IPointerHover*)&pointerDown);
        }
    }
};

class UIDrawerSystem : public System {
public:
    UIDrawerSystem();

    void Update(float dt) override;
    void Draw() override;
};
