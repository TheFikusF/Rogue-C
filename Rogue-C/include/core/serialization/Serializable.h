#pragma once
#include <string>

struct Serializable {
    virtual void Read(std::string name, void* value) = 0;
    virtual std::string Write() = 0;
};

template<typename T>
T ReadTyped(void* value) {
    T result;
    //formatter_type
}