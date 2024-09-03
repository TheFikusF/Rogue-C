#include "./include/core/serialization/Serializable.h"
#include "./include/core/ecs/ECS.h"

using namespace Serialization;

std::unordered_map<std::string, ReadFunction> readFunctionsMap;
std::unordered_map<std::string, std::size_t> typeHashesMap;
std::unordered_map<std::string, std::size_t> typeSizeMap;

Node* Node::AddChild(std::string name, std::string value) {
    children.emplace_back(Node(this, name, value));
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

void Serialization::SerializedEntity::Read(std::string name, std::string value, const Node* curent) {
    if(name.compare("id") == 0) {
        id = Core::ECS::CreateEntity();
        LOG("creating entity {}", id);
        return;
    } 
    
    if (name.compare("parent") == 0) {
        Core::ECS::SetParent(id, std::stoul(value));
        return;
    }

    std::cout << "reading: " << name << " " << value << std::endl;
    void* data = malloc(typeSizeMap[name]);
    curent->ReadUntyped(name, data);
    Core::ECS::AddComponent(id, typeHashesMap[name], data);
    std::free(data);
}

void Serialization::SerializedEntity::Write(Node* curent) { }

void Node::ReadUntyped(std::string type, void* where) const{
    ReadFunction fun = readFunctionsMap[type];
    (*fun)(this, where);
}

void Serialization::SetTypeSerializationData(std::string typeName, std::size_t hash, std::size_t size, ReadFunction func) {
    readFunctionsMap[typeName] = func;
    typeHashesMap[typeName] = hash;
    typeSizeMap[typeName] = size;
}

std::string Serialization::demangle(const char* name) {
#if defined(_MSC_VER)
    int substring = 6
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
                    current = array->AddChild();
                    finishedName = true;
                    finishedIndenting = true;
                    continue;
                }

                if (lastIndenting == indenting && !start) {
                    current = current->parent->AddChild();
                } else if (lastIndenting < indenting || start) {
                    current = current->AddChild();
                } else if (lastIndenting > indenting) {
                    for (int dif = (lastIndenting - indenting) / 2; dif >= 0;
                         dif--) {
                        current = current->parent;
                        if (dif == 0) {
                            current = current->AddChild();
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