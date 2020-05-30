#pragma once

#include <bitset>
#include <map>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

#include "component_map.h"
#include "entity.h"
#include "type.h"

class EntityManager
{
public:
    EntityManager();
    Entity CreateEntity();
    void DestroyEntity(Entity);
    int GetExistingEntitiesCount();
    void PrintComponentTypeIdMapper();
    void PrintEntityComponentBitfield();

    template <typename T>
    void RegisterComponent()
    {
        const char* type_name = typeid(T).name();
        // TODO:
        // check if component type is already known

        // assign an ID to the new component type
        component_type_id_mapper_.insert({type_name, next_component_type_id_});
        // make (a pointer to) a map which can hold components of the new component type
        component_map_.insert({type_name, std::make_shared<ComponentMap<T>>()});

        ++next_component_type_id_;
    }

    // get the ID assigned to a component type
    template <typename T>
    ComponentTypeId TypeIdOf(T component)
    {
        return component_type_id_mapper_[typeid(component).name()];
    }

    template <typename T>
    void AddComponent(Entity entity, T component)
    {
        // set the bit corresponding to the component type to indicate that this entity now "has" the component
        entity_component_bitfield_[entity.ID_].set(TypeIdOf(component), 1);
    }

    template <typename T>
    void RemoveComponent(Entity entity, T component)
    {
        // set the bit corresponding to the component type to 0 to indicate that this entity no longer "has" the component
        entity_component_bitfield_[entity.ID_].set(TypeIdOf(component), 0);
    }

private:
    int existing_entities_count_;
    EntityIdType next_entity_id_;
    ComponentTypeId next_component_type_id_;

    // queue destroyed entities and reuse them first before creating new ones (when the queue is empty)
    std::queue<Entity> available_entities_;
    std::map<EntityIdType, ComponentBitField> entity_component_bitfield_;
    std::unordered_map<const char*, ComponentTypeId> component_type_id_mapper_;
    std::map<const char*, std::shared_ptr<IComponentMap>> component_map_;  // unique_ptr?
};
