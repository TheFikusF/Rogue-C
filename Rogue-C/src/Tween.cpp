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

Sequence* TweenSystem::MakeSequence() {
    std::unique_lock<std::mutex> lock(_instance->_mutex);
    Sequence* tween = new Sequence();
    tween->id = _instance->_availableIDs.front();
    tween->Start();

    _instance->_availableIDs.pop();
    _instance->_tweeners[tween->id] = tween;
    return tween;
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
        LOG("COMPLETED");
        Complete(tween);
    }
}

void TweenSystem::Sync() {
    for (TweenID id : _scheduledToStop) {
        _instance->_availableIDs.push(id);
        delete _instance->_tweeners[id];
        _instance->_tweeners[id] = nullptr;
    }

    _scheduledToStop.clear();
}

void TweenSystem::Stop(ITween* tween) {
    _instance->_scheduledToStop.insert(tween->id);
}

Sequence::Sequence() : currentTween(0), timeSum(0), intervalTemp(666) {
    m_timer = Timer(0);
    onStart = []() -> void {};
    onComplete = []() -> void {};
    onKill = []() -> void {};
}

Sequence::~Sequence() {
    for (std::uint16_t i = 0; i < tweeners.size(); ++i) {
        delete tweeners[i];
    }
}

void Sequence::AppendInterval(float duration) {
    Append<float>(
        intervalTemp, 0.0f,
        [](float a, float b, float t) -> float { return 0; }, duration);
}

void Sequence::AppendCallback(void (*callback)(void)) {
    Append<float>(
        intervalTemp, 0.0f,
        [](float a, float b, float t) -> float { return 0; }, 0)
        ->onComplete = callback;
}

void Sequence::Start() {
    m_timer.Start();
    onStart();
}

void Sequence::Update() {
    if(tweeners.size() == 0 || currentTween >= tweeners.size()) {
        return;
    }
    
    tweeners[currentTween]->m_timer.currentTime = m_timer.currentTime - timeSum;
    tweeners[currentTween]->Update();
    while (m_timer.currentTime - timeSum > tweeners[currentTween]->m_timer.time) {
        timeSum += tweeners[currentTween]->m_timer.time;
        tweeners[currentTween]->onComplete();
        
        currentTween++;

        if(currentTween >= tweeners.size()) {
            return;
        }

        tweeners[currentTween]->Start();
        tweeners[currentTween]->m_timer.currentTime = m_timer.currentTime - timeSum;
        tweeners[currentTween]->Update();
    }
}
