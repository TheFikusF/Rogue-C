#include "./include/core/serialization/Serializable.h"
#include "./include/core/Transform.h"
#include "./include/core/ecs/ECS.h"

using namespace Serialization;

std::unordered_map<std::string, ReadFunction> readFunctionsMap;
std::unordered_map<std::string, WriteFunction> writeFunctionsMap;
std::unordered_map<std::string, std::size_t> typeHashesMap;
std::unordered_map<std::string, std::size_t> typeSizeMap;


Node* Node::AddChildEmpty(std::string name, std::string value) {
    children.emplace_back(Node(this, name, value));
    return &(children.back());
}

template<>
Node* Node::AddChild(const int& value, std::string name) {
    children.emplace_back(Node(this, name, std::to_string(value)));
    children.back().type = typeid(int).hash_code();
    return &(children.back());
}

template<>
Node* Node::AddChild(const float& value, std::string name) {
    children.emplace_back(Node(this, name, std::to_string(value)));
    children.back().type = typeid(float).hash_code();
    return &(children.back());
}

template<>
Node* Node::AddChild(const std::string& value, std::string name) {
    children.emplace_back(Node(this, name, value));
    children.back().type = typeid(std::string).hash_code();
    return &(children.back());
}

template<>
Node* Node::AddChild(const bool& value, std::string name) {
    children.emplace_back(Node(this, name, std::to_string(value)));
    children.back().type = typeid(bool).hash_code();
    return &(children.back());
}

template<>
Node* Node::AddChild(const std::uint8_t& value, std::string name) {
    children.emplace_back(Node(this, name, std::to_string(value)));
    children.back().type = typeid(std::uint8_t).hash_code();
    return &(children.back());
}

template<>
Node* Node::AddChild(const std::uint16_t& value, std::string name) {
    children.emplace_back(Node(this, name, std::to_string(value)));
    children.back().type = typeid(std::uint16_t).hash_code();
    return &(children.back());
}

template<>
Node* Node::AddChild(const std::uint32_t& value, std::string name) {
    children.emplace_back(Node(this, name, std::to_string(value)));
    children.back().type = typeid(std::uint32_t).hash_code();
    return &(children.back());
}

template<>
Node* Node::AddChild(const std::uint64_t& value, std::string name) {
    children.emplace_back(Node(this, name, std::to_string(value)));
    children.back().type = typeid(std::uint64_t).hash_code();
    return &(children.back());
}

void Node::Print(std::ostream& stream, int identing, bool isArrayElement) const {
    for (int i = 0; i < identing; i++) {
        stream << "  ";
    }

    if (isArrayElement) {
        stream << "-" << value << std::endl;
    } else if (isArray) {
        stream << name << ":[" << std::endl;
    } else if (value.length()) {
        stream << name << ":\"" << value << "\"" << std::endl;
    } else {
        stream << name << ":" << std::endl;
    }

    for (auto& node : children) {
        node.Print(stream, identing + 1, isArray);
    }

    if (isArray) {
        for (int i = 0; i < identing; i++) {
            stream << "  ";
        }
        stream << "]" << std::endl;
    }
}

template<>
void Serialization::Read(const Node* current, SerializedEntity& target) { 
    if (current->name.compare("id") == 0) {
        target.id = Core::ECS::CreateEntity();
        LOG("creating entity {}", target.id);
        return;
    }

    if (current->name.compare("parent") == 0) {
        Core::ECS::SetParent(target.id, std::stoul(current->value));
        return;
    }

    void* data = nullptr;
    current->ReadUntyped(current->name, &data);
    Core::ECS::AddComponent(target.id, typeHashesMap[current->name], data);
    std::free(data);
}

template<>
void Serialization::Write(Node* parent, const SerializedEntity& from) { 
    parent->AddChild(from.id, "id");

    if (Core::ECS::GetParent(from.id) < MAX_ENTITIES) {
        parent->AddChild(Core::ECS::GetParent(from.id), "parent");
    }

    for (auto const& hash : typeHashesMap) {
        auto component = Core::ECS::GetComponent(from.id, hash.second);
        if (component != nullptr) {
            WriteFunction fun = writeFunctionsMap[hash.first];
            (*fun)(parent->AddChildEmpty(hash.first), component);
        }
    }
}

void Node::ReadUntyped(std::string type, void** where) const {
    ReadFunction fun = readFunctionsMap[type];
    (*fun)(this, where);
}

void Serialization::SetTypeSerializationData(std::string typeName, std::size_t hash, std::size_t size, ReadFunction readFunc, WriteFunction writeFunc) {
    readFunctionsMap[typeName] = readFunc;
    writeFunctionsMap[typeName] = writeFunc;
    typeHashesMap[typeName] = hash;
    typeSizeMap[typeName] = size;
}

std::string Serialization::demangle(const char* name) {
#if defined(_MSC_VER)
    int substring = 6;
    if (name[0] == 's') {
        substring = 7;
    }
    std::string result = std::string(name).substr(substring);
#else
    int status = -1;
    std::unique_ptr<char, void(*)(void*)> res{
        abi::__cxa_demangle(name, NULL, NULL, &status),
        std::free
    };
    std::string result = (status == 0) ? res.get() : name;
#endif
    std::replace(result.begin(), result.end(), ':', '_');
    return result;
}

void Serialization::ConstructNodes(Node& root, const char* path) {
    Node* current = &root;
    Node* array = nullptr;

    std::ifstream file(path);
    std::string line;

    bool start = true;
    bool bracesOpened = false;
    bool arrayStarted = false;
    bool arrayClosed = false;
    std::uint8_t lastIndenting = 0;
    std::string name;
    std::string value;

    while (std::getline(file, line)) {
        bool finishedIndenting = false;
        bool finishedName = false;
        bool hasValue = false;

        name = "";
        value = "";
        std::uint8_t indenting = 0;
        for (std::uint32_t i = 0; i < line.size(); i++) {
            if (line[i] == '\"') {
                bracesOpened = !bracesOpened;
                continue;
            }

            if (line[i] == ' ' && bracesOpened == false) {
                if (finishedIndenting == false) {
                    indenting++;
                }
                continue;
            }

            if (line[i] == ']') {
                arrayClosed = true;
                arrayStarted = false;
                continue;
            }

            if (finishedIndenting == false) {
                if (arrayStarted && line[i] == '-') {
                    current = array->AddChildEmpty();
                    finishedName = true;
                    finishedIndenting = true;
                    continue;
                }

                if (lastIndenting == indenting && !start) {
                    current = current->parent->AddChildEmpty();
                } else if (lastIndenting < indenting || start) {
                    current = current->AddChildEmpty();
                } else if (lastIndenting > indenting) {
                    for (int dif = (lastIndenting - indenting) / 2; dif >= 0;
                         dif--) {
                        current = current->parent;
                        if (dif == 0) {
                            current = current->AddChildEmpty();
                        }
                    }
                }
            }

            finishedIndenting = true;

            if (line[i] == ':') {
                finishedName = true;
                continue;
            }

            if (line[i] == '[') {
                arrayStarted = true;
                array = current;
                current->isArray = true;
                continue;
            }

            if (finishedName) {
                value.push_back(line[i]);
                hasValue = true;
            } else {
                name.push_back(line[i]);
            }
        }

        if (arrayClosed) {
            arrayClosed = false;
        } else {
            current->value = value;
            current->name = name;
        }

        lastIndenting = indenting;
        start = false;
    }

    file.close();
}