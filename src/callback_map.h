#pragma once

#include <functional>
#include <map>

#include "type.h"

class ICallbackMap
{
public:
    virtual ~ICallbackMap() = default;
};

template <typename T>
class CallbackMap : public ICallbackMap
{
public:
    std::map<ProcessIdType, std::function<void(T)>> map_;
};
