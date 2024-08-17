#pragma once
#include <algorithm>
#include <limits>

namespace MathF {
    constexpr float Sign(float a) {
        if(a == 0) {
            return 0;
        }
        
        if(a > 0) {
            return 1;
        }

        return -1;
    }

    constexpr float Lerp(float a, float b, float t) {
        return a + (b - a) * t;
    }

    constexpr float InvLerp(float a, float b, float value) {
        return (value - a) / (b - a);
    }

    constexpr float Remap(float fromMin, float fromMax, float toMin, float toMax, float value) {
        return Lerp(toMin, toMax, InvLerp(fromMin, fromMax, value));
    }

    constexpr float MoveTowards(float current, float target, float maxDelta) {
        if (std::abs(target - current) <= maxDelta) {
            return target;
        }

        return current + Sign(target - current) * maxDelta;
    }

    //FIXME: NOT WORKING, NEED TO BE REMADE LATER
    constexpr float SmoothDamp(
        float current, float target, float& currentVelocity, float smoothTime,
        float deltaTime, float maxSpeed = INFINITY) {

        smoothTime = std::max(0.0001f, smoothTime);
        float omega = 2.0f / smoothTime;

        float x = omega * deltaTime;
        float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
        float change = current - target;
        float originalTo = target;

        // Clamp maximum speed
        float maxChange = maxSpeed * smoothTime;
        change = std::clamp(change, -maxChange, maxChange);
        target = current - change;

        float temp = (currentVelocity + omega * change) * deltaTime;
        currentVelocity = (currentVelocity - omega * temp) * exp;
        float output = target + (change + temp) * exp;

        // Prevent overshooting
        if ((originalTo - current > 0.0F) == (output > originalTo)) {
            output = originalTo;
            currentVelocity = (output - originalTo) / deltaTime;
        }

        return output;
    }
}