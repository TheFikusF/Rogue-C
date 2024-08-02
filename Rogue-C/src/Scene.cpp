#include "Scene.h"

Scene::Scene(std::function<void(void)> onStart) : onStart(onStart), started(false) { }

Scene::~Scene() {
    if(started) {
        Clear();
    }
}

void Scene::Start() {
    started = true;
    ECS::Init();
    onStart();
}

void Scene::Clear() {
    ECS::Clear();
}
