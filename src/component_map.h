#pragma once

#include <map>

#include "entity.h"

class IComponentMap
{
public:
    virtual ~IComponentMap() = default;
};

template <typename T>
class ComponentMap : public IComponentMap
{
public:
    std::map<Entity, T> entity_component_map_;
};
