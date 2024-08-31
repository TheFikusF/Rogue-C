#include "./include/core/serialization/Serializable.h"

using namespace Serialization;

std::unordered_map<std::string, ReadFunction> readFunctionsMap;

Node* Node::AddChild(std::string name, std::string value) {
    children.emplace_back(Node(this, name, value));
    return &(children.back());
}

void Node::Print(std::ostream& stream, int identing) const {
    for (int i = 0; i < identing; i++) {
        stream << "  ";
    }
    stream << name << ":\"" << value << "\"" << std::endl;
    for (auto& node : children) {
        node.Print(stream, identing + 1);
    }
}

void Node::ReadUntyped(std::string type, void* where) {
    ReadFunction fun = readFunctionsMap[type];
    (*fun)(this, where);
}

void Serialization::SetReadFunction(std::string typeName, ReadFunction func) {
    readFunctionsMap[typeName] = func;
}
