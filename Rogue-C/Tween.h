#pragma once
#include <functional>
#include <set>

#include "System.h"
#include "Timer.h"

using TweenID = std::uint32_t;
constexpr TweenID DEFAULT_TWEENID = MAX_ENTITIES;

struct ITween {
    TweenID id;
    Timer m_timer;
    float delay;

    std::function<void(void)> onStart;
    std::function<void(void)> onComplete;
    std::function<void(void)> onKill;

    virtual void Update() = 0;
};

template <typename T>
struct Tween : public ITween {
    T* source;
    T start;
    T target;

    std::function<T(T, T, float)> lerp;

    std::function<void(T)> onUpdate;

    Tween(T& source, T target, T (*lerpFunc)(T, T, float), float duration)
        : start(source), target(target), source(&source), lerp(lerpFunc) {
        m_timer = Timer(duration);
        onKill = []() -> void {};
        onComplete = []() -> void {};
        onStart = []() -> void {};
    }

    void Update() override {
        *source = lerp(start, target, m_timer.GetProgress());
    }
};

class TweenSystem : public System {
private:
    std::array<ITween*, MAX_ENTITIES> _tweeners;
    std::queue<TweenID> _availableIDs;
    std::mutex _mutex;

    std::set<TweenID> _scheduledToStop;

    static TweenSystem* _instance;

public:
    TweenSystem();

    template <typename T>
    static TweenID To(T& what, T where, T (*lerp)(T, T, float), float duration) {
        std::unique_lock<std::mutex> lock(_instance->_mutex);
        Tween<T>* tween = new Tween<T>(what, where, lerp, duration);
        tween->id = _instance->_availableIDs.front();
        LOG(std::format("started tween ID {}", tween->id));
        tween->m_timer.Start();

        _instance->_availableIDs.pop();
        _instance->_tweeners[tween->id] = tween;
        return tween->id;
    };

    static void Kill(TweenID tween);

    void Update(float dt) override;
    void Sync() override;

private:
    static void Complete(ITween* tween);
    static void Stop(ITween* tween);
};
