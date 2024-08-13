#include "Tween.h"

TweenSystem* TweenSystem::_instance;

TweenSystem::TweenSystem() {
    for (TweenID id = 0; id < MAX_ENTITIES; id++) {
        _availableIDs.push(id);
        _tweeners[id] = nullptr;
    }

    _instance = this;
}

void TweenSystem::Complete(ITween* tween) {
    if (tween == nullptr) {
        return;
    }

    tween->onComplete();
    tween->onComplete = []() -> void {};
    Stop(tween);
}

void TweenSystem::Kill(TweenID tween) {
    if (tween == DEFAULT_TWEENID || _instance->_tweeners[tween] == nullptr) {
        return;
    }

    _instance->_tweeners[tween]->onKill();
    Stop(_instance->_tweeners[tween]);
}

void TweenSystem::Update(float dt) {
    for (ITween* tween : _tweeners) {
        if (tween == nullptr) {
            continue;
        }

        if (tween->m_timer.InvCheck(dt)) {
            tween->Update();
            continue;
        }
        tween->Update();
        Complete(tween);
    }
}

void TweenSystem::Sync() {
    for(TweenID id : _scheduledToStop) {
        _instance->_availableIDs.push(id);
        delete _instance->_tweeners[id];
        _instance->_tweeners[id] = nullptr;
    }

    _scheduledToStop.clear();
}

void TweenSystem::Stop(ITween* tween) {
    _instance->_scheduledToStop.insert(tween->id);
}
