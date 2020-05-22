#pragma once

#include <typeinfo>
#include <unordered_map>

#include "type.h"

class ComponentManager
{
public:
    ComponentManager();
    ComponentType get_next_component_type();

    void print_component_types();

    template <typename T>
    void register_component()
    {
        componentTypeMap_.insert({typeid(T).name(), nextComponentType_});
        ++nextComponentType_;
    }

private:
    ComponentType nextComponentType_;
    std::unordered_map<const char*, ComponentType> componentTypeMap_;
};
