#pragma once
#include "./include/core/ecs/Entity.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <unordered_map>
#include <cstdarg>

#if defined(_MSC_VER)
#else
    #include <cxxabi.h>
    #include <memory>
#endif

//:~
////[]::::====-------------------------------------
//[0]::==---------------
//[ GET DRUNK, FIND GUN ahh type serialization ]::===:{#}~
//[0]:=-------
///[]:::===-----------------------
//:~

namespace Serialization {
    struct Node {
        bool isArray;
        Node* parent;
        std::string name;
        std::string value;

        std::vector<Node> children;

        Node() : parent(nullptr), name("root"), isArray(false) { }
        Node(Node* parent) : parent(parent), isArray(false) { }
        Node(Node* parent, std::string name, std::string value) : parent(parent), name(name), value(value), isArray(false) { }

        Node* AddChild(std::string name = "", std::string value = "");

        void Print(std::ostream& stream, int identing = 0, bool isArrayElement = false) const;

        template<typename T>
        T Read() const;

        template<typename T>
        void ReadVector(std::vector<T>& target) const;

        void ReadUntyped(std::string type, void* where) const;
    };

    template<typename T>
    void Read(const Node* current, T& target) { }
    
    template<typename T>
    void Write(Node* parent, const T& from) { }

    struct SerializedEntity {
        Entity id;

        SerializedEntity() : id(0) { }
        SerializedEntity(Entity id) : id(id) { }
    };

#pragma region TYPE_REGISTERING
    using ReadFunction = void (*)(const Node*, void*);
    using WriteFunction = void (*)(Node*, void const*);

    std::string demangle(const char* name);

    void SetTypeSerializationData(std::string typeName, std::size_t hash, std::size_t size, ReadFunction readFunc, WriteFunction writeFunc);
    
    template<typename T>
    void RegisterType() {
        std::string name = demangle(typeid(T).name());
        std::cout << name << std::endl;
        SetTypeSerializationData(name, typeid(T).hash_code(), sizeof(T), 
            [](const Node* node, void* where) -> void { (*((T*)where)) = node->Read<T>(); },
            [](Node* node, void const* what) -> void { Serialization::Write(node, *((T const*)what)); });
    }
#pragma endregion

    template<typename T>
    inline T Node::Read() const {
        T result;
        for (auto const& child : children) {
            Serialization::Read(&child, result);
        }
        return result;
    }

    template<typename T>
    inline void Node::ReadVector(std::vector<T>& target) const {
        for(auto const& child : children) {
            T result = child.Read<T>();
            target.emplace_back(result);
        }
    }

    void ConstructNodes(Node& root, const char* path);

    template<typename T>
    inline T Deserialize(const char* path) {
        Node root;
        root.name = "root";

        ConstructNodes(root, path);

        root.Print(std::cout);
        return root.children[0].Read<T>();
    }
}
