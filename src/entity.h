#pragma once

#include <cinttypes>

#include "type.h"

class Entity
{
public:
    bool operator <(const Entity& rhs) const
    {
        return id_ < rhs.id_;
    }

    EntityIdType id_;
};
