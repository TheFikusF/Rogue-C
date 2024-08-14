#pragma once
#include <functional>
#include <set>

#include "System.h"
#include "Timer.h"

using TweenID = std::uint32_t;
constexpr TweenID DEFAULT_TWEENID = MAX_ENTITIES;

enum class Ease {
    Linear,
    InSine,
    OutSine,
    InOutSine,
    InCubic,
    OutCubic,
    InOutCubic,
    InExpo,
    OutExpo,
    InOutExpo,
    InBack,
    OutBack,
    InOutBack,
};

constexpr float GetEase(Ease ease, float t) {
    switch (ease)
    {
    case Ease::Linear:      return t;
    case Ease::InSine:      return Vec2::CubicBezier(Vec2(0.12f, 0.0f),  Vec2(0.39f, 0.0f),     t).y;
    case Ease::OutSine:     return Vec2::CubicBezier(Vec2(0.61f, 1.0f),  Vec2(0.88f, 1.0f),     t).y;
    case Ease::InOutSine:   return Vec2::CubicBezier(Vec2(0.37f, 0.0f),  Vec2(0.63f, 1.0f),     t).y;
    case Ease::InCubic:     return Vec2::CubicBezier(Vec2(0.32f, 0.0f),  Vec2(0.67f, 0.0f),     t).y;
    case Ease::OutCubic:    return Vec2::CubicBezier(Vec2(0.33f, 1.0f),  Vec2(0.68f, 1.0f),     t).y;
    case Ease::InOutCubic:  return Vec2::CubicBezier(Vec2(0.65f, 0.0f),  Vec2(0.35f, 1.0f),     t).y;
    case Ease::InExpo:      return Vec2::CubicBezier(Vec2(0.70f, 0.0f),  Vec2(0.84f, 1.0f),     t).y;
    case Ease::OutExpo:     return Vec2::CubicBezier(Vec2(0.16f, 1.0f),  Vec2(0.30f, 1.0f),     t).y;
    case Ease::InOutExpo:   return Vec2::CubicBezier(Vec2(0.87f, 0.0f),  Vec2(0.13f, 1.0f),     t).y;
    case Ease::InBack:      return Vec2::CubicBezier(Vec2(0.36f, 0.0f),  Vec2(0.66f, -0.56f),   t).y;
    case Ease::OutBack:     return Vec2::CubicBezier(Vec2(0.34f, 1.56f), Vec2(0.64f, 1.0f),     t).y;
    case Ease::InOutBack:   return Vec2::CubicBezier(Vec2(0.68f, -0.6f), Vec2(0.32f, 1.6f),     t).y;
    default:                return t;
    }
}

struct ITween {
    TweenID id;
    Timer m_timer;
    float delay;
    Ease ease;

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

    Tween(T& source, T target, T (*lerpFunc)(T, T, float), float duration, Ease ease)
        : start(source), target(target), source(&source), lerp(lerpFunc) {
        m_timer = Timer(duration);
        this->ease = ease;
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
        *source = lerp(start, target, GetEase(ease, std::max(std::min(m_timer.GetProgress(), 1.0f), 0.0f)));
    }
};

struct Sequence : public ITween {
    Sequence();
    ~Sequence();

    template <typename T>
    Tween<T>* Append(T& what, T where, T (*lerpFunc)(T, T, float), float duration, Ease ease = Ease::Linear) {
        m_timer.time += duration;
        m_timer.Start();
        Tween<T>* tween = new Tween<T>(what, where, lerpFunc, duration, ease);
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
    static TweenID To(T& what, T where, T (*lerp)(T, T, float), float duration, Ease ease = Ease::Linear) {
        std::unique_lock<std::mutex> lock(_instance->_mutex);
        Tween<T>* tween = new Tween<T>(what, where, lerp, duration, ease);
        tween->id = _instance->_availableIDs.front();
        tween->Start();

        _instance->_availableIDs.pop();
        _instance->_tweeners[tween->id] = tween;
        return tween->id;
    };

    static Sequence* MakeSequence();

    static void Kill(TweenID tween);
    static bool IsPlaying(TweenID tween);
    static ITween* GetTween(TweenID tween);

    void Update(float dt) override;
    void Sync() override;

private:
    static void Complete(ITween* tween);
    static void Stop(ITween* tween);
};