#include "Scene.h"

Scene::Scene(std::function<void(void)> onStart) : onStart(onStart) {
}

Scene::~Scene() {
    if(started) {
        Clear();
    }
}

void Scene::Start() {
    started = true;
    currentECS = new ECS();
    onStart();


}

void Scene::Clear() {
    delete currentECS;
}
