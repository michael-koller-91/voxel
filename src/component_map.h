#pragma once

#include <map>

#include "type.h"

class IComponentMap
{
public:
    virtual ~IComponentMap() = default;
};

template <typename T>
class ComponentMap : public IComponentMap
{
public:
    std::map<EntityIdType, T> entity_component_map_;
};
