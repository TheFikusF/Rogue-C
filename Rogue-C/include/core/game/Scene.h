#pragma once
#include "./include/core/serialization/Serializable.h"
#include "./include/core/ecs/ECS.h"
#include <functional>

namespace Core {
    class Scene {
    private:
        bool started;
        std::function<void (void)> onStart;

    public:
        Scene(std::function<void (void)> onStart);
        ~Scene();

        void Start();
        void Clear();

        static void FinishRegistration();
        static void ReadScene(const char* sceneFile);
    };

    class SerializedScene : public Serialization::Serializable {
        void Read(std::string name, std::string value, const Serialization::Node* curent) override;
        void Write(Serialization::Node* curent) override;
    };
}