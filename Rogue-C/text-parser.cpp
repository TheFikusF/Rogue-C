//NEEDS TO BE INTEGRATED INTO THE PROJECT

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

struct Node {
    Node* parent;
    std::string name;
    std::string value;

    std::vector<Node> children;

    Node() : parent(nullptr), name("root") { }
    Node(Node* parent) : parent(parent) { }
    Node(Node* parent, std::string name, std::string value) : parent(parent), name(name), value(value) { }

    Node* AddChild(std::string name = "", std::string value = "") {
        children.emplace_back(Node(this, name, value));
        return &(children.back());
    }

    void Print(std::ostream& stream, int identing = 0) {
        for (int i = 0; i < identing; i++) {
            stream << "  ";
        }
        stream << name << ":\"" << value << "\"" << std::endl;
        for (auto& node : children) {
            node.Print(stream, identing + 1);
        }
    }

    template<typename T>
    T Read() const;
};

#pragma region TYPE_REGISTERING
using pfunc = void (*)(const Node*, void*);
std::unordered_map<std::string, pfunc> map;

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

template<typename T>
void RegisterType() {
    std::string name = demangle(typeid(T).name());
    std::cout << name << std::endl;
    map[name] = [](const Node* node, void* where) -> void { (*((T*)where)) = node->Read<T>(); };
}

void ReadUntyped(std::string type, const Node* node, void* where) {
    pfunc fun = map[type];
    (*fun)(node, where);
}
#pragma endregion

struct Serializable {
    virtual void Read(std::string name, std::string value, const Node* current) = 0;
    virtual void Write(Node* root) = 0;
};

struct Vec2 : public Serializable {
    float x, y;

    void Read(std::string name, std::string value, const Node* current) override {
        if (name.compare("x") == 0) x = std::stof(value);
        if (name.compare("y") == 0) y = std::stof(value);
    }

    void Write(Node* root) override {
        auto xNode = root->AddChild("x", std::to_string(x));
        auto yNode = root->AddChild("y", std::to_string(y));
    }

    void Print() {
        std::cout << "x:" << x << ", y:" << y << std::endl;
    }
};

struct A : public Serializable {
    std::string text;
    Vec2 a;

    void Read(std::string name, std::string value, const Node* current) override {
        if (name.compare("text") == 0) text = value;
        //if (name.compare("a") == 0) a = current->Read<Vec2>();
        if (name.compare("a") == 0) ReadUntyped("Vec2", current, &a);
    }

    void Write(Node* root) override {
        auto xNode = root->AddChild("text", text);
        auto yNode = root->AddChild("a");
        a.Write(yNode);
    }

    void Print() {
        std::cout << text << " here is my vector a:" << std::endl;
        a.Print();
    }
};

struct B : public Serializable {
    A hi;
    A helo;

    void Read(std::string name, std::string value, const Node* current) override {
        if (name == "hi") hi = current->Read<A>();
        if (name == "helo") helo = current->Read<A>();
    }

    void Write(Node* root) override {
        auto xNode = root->AddChild("hi");
        hi.Write(xNode);
        auto yNode = root->AddChild("helo");
        helo.Write(yNode);
    }

    void Print() {
        std::cout << "here is my var hi:" << std::endl;
        hi.Print();

        std::cout << std::endl << "here is my var helo:" << std::endl;
        helo.Print();
    }
};

template<typename T>
T Deserialize(const char* path) {
    Node root;
    root.name = "root";

    Node* current = &root;

    std::ifstream file(path);
    std::string line;

    bool start = true;
    bool bracesOpened = false;
    std::uint8_t lastIndenting = 0;
    std::string name;
    std::string value;

    while (std::getline(file, line)) {
        //std::cout << line << std::endl;

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

            if (finishedIndenting == false) {
                if (lastIndenting == indenting && !start) {
                    current = current->parent->AddChild();
                }

                if (lastIndenting < indenting || start) {
                    current = current->AddChild();
                }

                if (lastIndenting > indenting) {
                    for (int dif = (lastIndenting - indenting) / 2; dif >= 0; dif--) {
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

            if (finishedName) {
                value.push_back(line[i]);
                hasValue = true;
            }
            else {
                name.push_back(line[i]);
            }
        }

        current->value = value;
        current->name = name;

        lastIndenting = indenting;
        start = false;
    }
    file.close();

    root.Print(std::cout);
    return root.children[0].Read<T>();
}

template<typename T>
T Node::Read() const {
    static_assert(std::is_base_of<Serializable, T>::value);
    T result;
    for (auto const& child : children) {
        result.Read(child.name, child.value, &child);
    }
    return result;
}

int main() {
    RegisterType<A>();
    RegisterType<B>();
    RegisterType<Vec2>();
    B root = Deserialize<B>("test.txt");

    std::cout << std::endl;

    root.Print();
 
    std::cout << std::endl;

    Node node;
    root.Write(&node);

    std::ofstream output("output.txt");
    node.Print(output);
    output << "." << std::endl;
    output.close();
}
