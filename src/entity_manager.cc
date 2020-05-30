#pragma once

#include <iostream>
#include "entity.h"
#include "entity_manager.h"

EntityManager::EntityManager()
{
    existing_entities_count_ = 0;
    next_entity_id_ = 0;
    next_component_type_id_ = 0;
}

Entity EntityManager::CreateEntity()
{
    ++existing_entities_count_;
    if (available_entities_.empty()) {
        Entity entity;
        ComponentBitField component_bitfield;
        entity.id_ = next_entity_id_;
        entity_component_bitfield_.insert({entity.id_, component_bitfield});
        ++next_entity_id_;
        return entity;
    } else {
        return available_entities_.front();
    }
}

void EntityManager::DestroyEntity(Entity entity)
{
    entity_component_bitfield_[entity.id_].reset();
    available_entities_.push(entity);
    --existing_entities_count_;
}

ComponentBitField EntityManager::GetBitField(Entity entity)
{
    return entity_component_bitfield_[entity.id_];
}

int EntityManager::GetExistingEntitiesCount()
{
    return existing_entities_count_;
}

void EntityManager::PrintComponentTypeIdMapper()
{
    std::cout << "-----component_type_id_mapper_\n";
    for (auto const& i : component_type_id_mapper_)
    {
        std::cout << "component type: " << i.first << std::endl;
        std::cout << "id of component type: " << i.second << std::endl;
    }
}

void EntityManager::PrintEntityComponentBitfield()
{
    std::cout << "-----entity_component_bitfield\n";
    for (auto const& i : entity_component_bitfield_)
    {
        std::cout << "entity id: " << i.first << std::endl;
        std::cout << "entity bit field: " << i.second << std::endl;
    }
}