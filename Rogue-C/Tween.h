#pragma once
#include <functional>

#include "System.h"
#include "Timer.h"

using TweenID = std::uint32_t;

struct ITween {
    TweenID id;
    Timer duration;
    float delay;

    std::function<void(void)> onStart;
    std::function<void(void)> onComplete;
    std::function<void(void)> onKill;

    virtual void Update(float dt) = 0;
};

template <typename T>
struct Tween : public ITween {
    T* source;
    T start;
    T target;

    std::function<T(T, T, float)> lerp;

    std::function<void(T)> onUpdate;

    Tween(T& source, T target, T (*lerp)(T, T, float), float duration);

    void Update(float dt) override;
};

class TweenSystem : public System {
private:
    std::array<ITween*, MAX_ENTITIES> _tweeners;
    std::queue<TweenID> _availableIDs;
    std::mutex _mutex;

    static TweenSystem* _instance;

public:
    TweenSystem();

    template <typename T>
    static Tween<T>* To(T& what, T where, T (*lerp)(T, T, float), float duration) {
        std::unique_lock<std::mutex> lock(_instance->_mutex);
        Tween<T>* tween = new Tween<T>(what, where, lerp, duration);
        tween->id = _instance->_availableIDs.front();
        tween->duration.Start();

        _instance->_availableIDs.pop();
        _instance->_tweeners[tween->id] = tween;
        return tween;
    };

    template <typename T>
    static void Complete(Tween<T>* tween) {
        if (tween == nullptr) {
            return;
        }

        tween->onComplete();
        Stop(tween);
    }

    template <typename T>
    static void Kill(Tween<T>* tween) {
        if (tween == nullptr) {
            return;
        }

        tween->onKill();
        Stop(tween);
    }

    void Update(float dt) override;

private:
    template <typename T>
    static void Stop(Tween<T>* tween) {
        _instance->_availableIDs.push(tween->id);
        _instance->_tweeners[tween->id] = nullptr;
        delete tween;
    }
};
