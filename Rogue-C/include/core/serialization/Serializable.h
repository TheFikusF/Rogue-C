#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
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

        Node() : parent(nullptr), name("root") { }
        Node(Node* parent) : parent(parent) { }
        Node(Node* parent, std::string name, std::string value) : parent(parent), name(name), value(value) { }

        Node* AddChild(std::string name = "", std::string value = "");

        void Print(std::ostream& stream, int identing = 0, bool isArrayElement = false) const;

        template<typename T>
        T Read() const;

        template<typename T>
        T ReadVector(std::vector<T>& target) const;

        void ReadUntyped(std::string type, void* where) const;
    };

    struct Serializable {
        virtual void Read(std::string name, std::string value, const Node* curent) = 0;
        virtual void Write(Node* parent) = 0;
    };

    struct SerializedEntity : public Serializable {
        Entity id;

        SerializedEntity() : id(0) { }

        void Read(std::string name, std::string value, const Node* curent) override;
        void Write(Node* curent) override;
    };

#pragma region TYPE_REGISTERING
    using ReadFunction = void (*)(const Node*, void*);

    std::string demangle(const char* name) {
#if defined(_MSC_VER)
        if (name[0] == 'c') {
            return std::string(name).substr(6);
        } else {
            return std::string(name).substr(7);
        }
#else
        int status = -1;
        std::unique_ptr<char, void(*)(void*)> res{
            abi::__cxa_demangle(name, NULL, NULL, &status),
            std::free
        };
        return (status == 0) ? res.get() : name;
#endif
    }

    void SetTypeSerializationData(std::string typeName, std::size_t hash, std::size_t size, ReadFunction func);
    
    template<typename T>
    inline void RegisterType() {
        std::string name = demangle(typeid(T).name());
        std::cout << name << std::endl;
        SetTypeSerializationData(name, [](const Node* node, void* where) -> void { (*((T*)where)) = node->Read<T>(); });
    }
#pragma endregion

    template<typename T>
    inline T Node::Read() const {
        static_assert(std::is_base_of<Serializable, T>::value);
        T result;
        for (auto const& child : children) {
            result.Read(child.name, child.value, &child);
        }
        return result;
    }

    template<typename T>
    inline T Node::ReadVector(std::vector<T>& target) const {
        for(auto const& child : children) {
            target.emplace_back(child.Read<T>());
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
