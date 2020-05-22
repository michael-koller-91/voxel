#include <iostream>

#include "component_manager.h"
#include "type.h"

ComponentManager::ComponentManager()
{
    nextComponentType_ = 0;
}

ComponentType ComponentManager::get_next_component_type()
{
    return nextComponentType_;
}

void ComponentManager::print_component_types()
{
    for (auto const& i : componentTypeMap_)
    {
        std::cout << "type: " << i.first << std::endl;
        std::cout << "type ID: " << i.second << std::endl;
    }
}
