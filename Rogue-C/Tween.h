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

    virtual void Start() = 0;

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

    void Start() override {
        m_timer.Start();
        start = *source;
        onStart();
    }

    void Update() override {
        *source = lerp(start, target, std::max(std::min(m_timer.GetProgress(), 1.0f), 0.0f));
    }
};

struct Sequence : public ITween {
    Sequence();
    ~Sequence();

    template <typename T>
    Tween<T>* Append(T& what, T where, T (*lerpFunc)(T, T, float), float duration) {
        m_timer.time += duration;
        m_timer.Start();
        Tween<T>* tween = new Tween<T>(what, where, lerpFunc, duration);
        tween->m_timer.Start();
        tweeners.push_back(tween);
        return tween;
    }


    void AppendInterval(float duration);
    void AppendCallback(void (*callback)(void));
    void Start() override;
    void Update() override;
    
private:
    std::vector<ITween*> tweeners;
    float timeSum;
    std::uint16_t currentTween;
    float intervalTemp;
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
        tween->Start();

        _instance->_availableIDs.pop();
        _instance->_tweeners[tween->id] = tween;
        return tween->id;
    };

    static Sequence* MakeSequence();

    static void Kill(TweenID tween);

    void Update(float dt) override;
    void Sync() override;

private:
    static void Complete(ITween* tween);
    static void Stop(ITween* tween);
};
