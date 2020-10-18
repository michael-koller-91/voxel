# Voxel
A voxel-based game.

<!-- ## Getting Started -->
<!-- ### Prerequisites -->
<!-- ### Installing -->

## Implementation Details
### Entity Manager
The entity manager takes care of entities and their components.
Every entity is an ID (nonnegative integer) and represents an object in the game.
Components consist of data only and are used to assign properties to entities.
Component examples are Position, Direction, or Healthbar.
For instance, if a Healthbar component is assigned to an entity,
the object in the game which is represented by the entity now has a health bar.

Every component type (like Position or Direction) is mapped to an ID bijectively.
We might for example have the assignment:
- 0 <-> Position
- 1 <-> Direction
- 2 <-> Healthbar

For every entity, a component bit field exists.
The number of bits in this bit field corresponds to the number of components that are used in the game (three in the example with Position, Direction, Healthbar).
The ID assigned to a component type is used as an index into the bit fields.
A newly created entity will have the component bit field 000.
If, for instance, a Healthbar component is assigned to the entity,
the corresponding bit field will change to 100 to reflect this.

For every component type, we have a std::map with entities as keys and components as values.
For instance, we might have the entity component maps:
- std::map<Entity, Position>
- std::map<Entity, Direction>
- std::map<Entity, Healthbar>

In order not to have distinct variables for all of these entity component maps,
there is another std::map with component type IDs as keys and shared pointers to the above maps as values.
This is possible by wrapping every entity component map in a class ComponentMap which
inherits from a common interface IComponentMap.
This way, the shared pointers can point to the interface type but we can access the
entity component maps by casting to the derived class.

### Process Manager
The process manager takes care of all processes (aka systems).
Examples for processes might be RenderProcess, WindowProcess, or PhysicsProcess.
Processes need to be registered with a priority (an integer).
This priority on the one hand serves as a process's unique identifier and on the other hand determines the order in which processes are called.
Every process needs to implement an update method and the process manager's update method calls all processes' update methods according to the given priority.

There exists a standard map with priorities as keys and processes as values:
- std::map<int, std::shared_ptr\<IProcess>>

This is used to access registered processes, for example to call the update methods.

## Versions
### 0.3
- First working process manager
### 0.2
- First working entity manager
### 0.1
- CMakeLists configured: building and running tests works
