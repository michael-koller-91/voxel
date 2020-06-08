#pragma once

#include <bitset>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "component_map.h"
#include "entity.h"
#include "type.h"

class EntityManager
{
public:
    EntityManager();

    Entity CreateEntity();
    void DestroyEntity(Entity);
    ComponentBitField GetBitField(Entity);
    int GetExistingEntitiesCount();
    std::map<Entity, ComponentBitField>& GetEntities();

    // good old debugging via printing...
    void PrintComponentTypeIdMapper();
    void PrintEntityComponentBitField();

    template <typename T>
    void RegisterComponent()
    {
        const std::string& type_name = typeid(T).name();

        // assert that component type has not already been registered
        assert(component_type_id_mapper_.find(type_name) == component_type_id_mapper_.end() && "Component type already registered.");
        // register by assigning an ID to the new component type
        component_type_id_mapper_.insert({type_name, next_component_type_id_});

        // make (a pointer to) a map which can hold components of the new component type
        component_map_.insert({next_component_type_id_, std::make_shared<ComponentMap<T>>()});

        ++next_component_type_id_;
    }

    // get the ID assigned to a component type
    template <typename T>
    ComponentTypeId TypeIdOf()
    {
        return component_type_id_mapper_[typeid(T).name()];
    }

    template <typename T>
    void AddComponent(Entity entity, T component)
    {
        auto ecm = GetComponentMap<T>();
        // assert that entity does not already have this component type
        assert(ecm->entity_component_map_.find(entity) == ecm->entity_component_map_.end() && "Entity already has this component.");
        ecm->entity_component_map_.insert({entity, component});
        // set the bit corresponding to the component type to indicate that this entity now "has" the component
        entity_component_bitfield_[entity].set(TypeIdOf<T>());
    }

    template <typename T>
    T& GetComponent(Entity entity)
    {
        // assert that entity has component T (by checking if the corresponding bit is set)
        assert(entity_component_bitfield_[entity][TypeIdOf<T>()] && "Entity does not have this component.");
        return GetComponentMap<T>()->entity_component_map_[entity];
    }

    // convert the given component types to a bit field (where the corresponding bits are set)
    template <typename... T>
    typename std::enable_if<sizeof...(T) == 0, ComponentBitField>::type ComponentBitFieldOf()
    {
        return ComponentBitField();
    }

    // get a bit field where the bits corresponding to the template arguments are set
    template <typename T, typename... Args>
    ComponentBitField ComponentBitFieldOf()
    {
        ComponentBitField cbf = ComponentBitFieldOf<Args...>();
        cbf.set(TypeIdOf<T>());
        return cbf;
    }

    template <typename T>
    void RemoveComponent(Entity entity, T component)
    {
        RemoveComponent<T>(entity);
    }

    template <typename T>
    void RemoveComponent(Entity entity)
    {
        // assert that entity hast this component type
        assert(entity_component_bitfield_[entity][TypeIdOf<T>()] && "Entity does not have this component.");
        // reset the bit corresponding to the component type to indicate that this entity no longer "has" the component
        entity_component_bitfield_[entity].reset(TypeIdOf<T>());
        GetComponentMap<T>()->entity_component_map_.erase(entity);
    }

    // cast from base class (IComponentMap) to derived class (ComponentMap)
    template <typename T>
    std::shared_ptr<ComponentMap<T>> GetComponentMap()
    {
        return std::static_pointer_cast<ComponentMap<T>>(component_map_[TypeIdOf<T>()]);
    }

private:
    int existing_entities_count_;
    EntityIdType next_entity_id_;
    ComponentTypeId next_component_type_id_;
    // queue destroyed entities and reuse them first before creating new ones (when the queue is empty)
    std::queue<Entity> available_entities_;
    std::map<Entity, ComponentBitField> entity_component_bitfield_;
    std::unordered_map<std::string, ComponentTypeId> component_type_id_mapper_;
    std::map<ComponentTypeId, std::shared_ptr<IComponentMap>> component_map_;  // unique_ptr?
};
