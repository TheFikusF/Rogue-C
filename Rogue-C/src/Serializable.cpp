#include "./include/core/serialization/Serializable.h"

using namespace Serialization;

std::unordered_map<std::string, ReadFunction> readFunctionsMap;

Node* Node::AddChild(std::string name, std::string value) {
    children.emplace_back(Node(this, name, value));
    return &(children.back());
}

void Node::Print(std::ostream& stream, int identing, bool isArrayElement = false) const {
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

void Node::ReadUntyped(std::string type, void* where) {
    ReadFunction fun = readFunctionsMap[type];
    (*fun)(this, where);
}

void Serialization::SetReadFunction(std::string typeName, ReadFunction func) {
    readFunctionsMap[typeName] = func;
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
