#include "Tween.h"

TweenSystem* TweenSystem::_instance;

template <typename T>
Tween<T>::Tween(T& source, T target, T (*lerpFunc)(T, T, float), float duration) : start(source), target(target), source(&source), lerp(lerpFunc), duration(Timer(duration)) {

}

template <typename T>
inline void Tween<T>::Update(float dt) {
    if(duration.InvCheck(dt)) {
        *source = lerp(start, target, duration.GetProgress())
        return;
    }

    TweenSystem::Complete(this);
}

TweenSystem::TweenSystem() {
    for(TweenID id = 0; id < MAX_ENTITIES; id++) {
        _availableIDs.push(id);
        _tweeners[id] = nullptr;
    }

    _instance = this;
}

void TweenSystem::Update(float dt) {
    for(ITween* tween : _tweeners) {
        if(tween == nullptr) {
            continue;
        }

        tween->Update(dt);
    }
}
