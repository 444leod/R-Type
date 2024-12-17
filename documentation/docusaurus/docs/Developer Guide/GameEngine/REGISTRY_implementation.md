---
sidebar_position: 2
id: 'RegistryImplementation'
title: 'Registry'
---

# Registry

The registry is a core component of an Entity Component System (ECS) architecture. It serves as a central hub for managing entities and their associated components. The registry is responsible for creating, tracking, and destroying entities, as well as attaching, removing, and querying components.

### Implementation

```cpp
#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include <iostream>
#include <map>
#include <vector>
#include <cassert>
#include <algorithm>
#include "Family.hpp"
#include "Entity.hpp"
#include "SparseSet.hpp"
#include "View.hpp"

/**
 * @brief The class representation of the  Registry.
 */
class Registry
{
public:
    Registry() = default;
    ~Registry()
    {
        for (auto [_, sparse] : this->_sparse_sets)
            delete sparse;
    }

    /**
     * @brief Gives a view of the entities with a set of components
     * @tparam First The first kind of component to query
     * @tparam ...Components Additional kind of components to query
     * @return A new view with entities queried.
     */
    template <typename First, typename... Components>
    View<First, Components...> view()
    {
        return View<First, Components...>(_sparse_sets);
    }

    /**
     * @brief Spawns a new Entity, used to attach components to
     * @return An new unique Entity
     */
    unsigned int create()
    {
        const auto entity = Registry::_get_new_entity_id();

        _entities.emplace_back(entity);
        return entity;
    }

    /**
     * @brief Deletes an Entity and all attached components
     * @param entity The Entity to delete
     */
    void remove(const Entity entity)
    {
        for (auto const &[id, sparse] : _sparse_sets)
            if (sparse->contains(entity))
                sparse->remove(entity);
        _entities.erase(std::find(_entities.begin(), _entities.end(), entity));
    }

    /**
     * @brief Removes all the Entities
     */
    void clear()
    {
        for (auto const &[id, sparse] : _sparse_sets)
            sparse->clear();
        _entities.clear();
    }

    /**
     * @brief Attaches a Component to an Entity
     * @tparam T The type of component to attach. Can be deduced by the parameter
     * @param entity The Entity to attach to
     * @param component The Component informations to attach
     */
    template <typename T>
    void addComponent(Entity entity, const T &component)
    {
        const auto id = type<T>::id();
        SparseSet<T> *set = nullptr;

        if (!this->_sparse_sets.contains(id))
        {
            set = new SparseSet<T>();
            this->_sparse_sets[id] = set;
        }
        else
            set = dynamic_cast<SparseSet<T> *>(this->_sparse_sets.at(id));
        set->set(entity, component);
    }

    /**
     * @brief Removes a component from an Entity
     * @tparam T The type of component to remove
     * @param entity The Entity to remove from
     */
    template <typename T>
    void removeComponent(Entity entity)
    {
        const auto id = type<T>::id();

        if (!this->_sparse_sets.contains(id))
        {
            return;
        }
        this->_sparse_sets.at(id)->erase(entity);
    }

    /**
     * @brief Used as debug to print out all the sparse-sets data
     */
    void displaySparse() const
    {
        std::cout << "There is a sparse array for the following components: " << std::endl;
        for (auto const &[id, sparse] : _sparse_sets)
            std::cout << id << std::endl;
    }

private:
    /**
     * @brief Gets a new available Entity ID
     * @return A new unique Entity ID
     */
    static Entity _get_new_entity_id()
    {
        static Entity value = 0;
        return value++;
    }

    std::vector<Entity> _entities = {};
    std::map<std::size_t, ISparseSet *> _sparse_sets = {};
};

#endif //REGISTRY_HPP
```

### Explanation

A registry is a core management system in an Entity Component System (ECS) architecture that serves several critical purposes:

1. **Entity Management**
   The registry is responsible for creating, tracking, and destroying entities. In this implementation, an entity is essentially a unique identifier (an unsigned integer) that can have various components attached to it. The registry ensures that each entity has a unique ID and maintains a collection of all active entities in the game or application.

2. **Component Storage and Organization**
   The registry provides an efficient way to store and manage components for each entity. It uses a sparse set data structure to optimize memory usage and access speed. Each type of component is stored in its own sparse set, allowing for quick lookup and manipulation of components associated with specific entities.

3. **Dynamic Component Attachment**
   The registry allows for runtime addition and removal of components to entities. This provides incredible flexibility in game design. For example, you could dynamically add a "flying" component to a character, change their health component, or remove a "weapon" component mid-game.

4. **Querying and Filtering**
   Through the `view()` method, the registry enables efficient querying of entities based on their component types. This is crucial for systems that need to process only entities with specific component combinations, like rendering all entities with a "renderable" component or updating all entities with a "physics" component.

5. **Memory Management**
   The registry handles memory allocation and deallocation of components. When an entity is created, components can be added dynamically. When an entity is removed, all its associated components are automatically cleaned up, preventing memory leaks.

### Example

```cpp
Registry registry;

// Create a player entity
Entity player = registry.create();
registry.addComponent<Position>(player, {100, 200});
registry.addComponent<Health>(player, {100});
registry.addComponent<Weapon>(player, {"Sword"});

// Create an enemy entity
Entity enemy = registry.create();
registry.addComponent<Position>(enemy, {300, 400});
registry.addComponent<Health>(enemy, {50});

// Query all entities with Position and Health components
auto view = registry.view<Position, Health>();
for (auto entity : view) {
    // Process entities with both Position and Health
}
```

In this example, the registry allows you to:
- Create entities dynamically
- Attach different types of components to entities
- Easily query and iterate over entities with specific component types

The design promotes:
- Composition over inheritance
- Data-oriented design
- High performance and flexibility in game object management

This particular implementation provides a lightweight, template-based approach to creating a flexible entity component system that can be used in game development, simulations, or any other software that benefits from a component-based architecture.

### More

You might have noticed that the registry implementation uses a sparse set data structure to store components efficiently. Sparse sets are a key optimization technique in ECS architectures, allowing for fast component access and iteration while minimizing memory usage. More on our sparse set implementation can be found in the [SparseSet](./SPARSE_SET_implementation) documentation.

It also uses a `View` class to provide a convenient way to query entities based on their component types. The `View` class is a critical component in ECS systems, enabling systems to process entities efficiently based on their component composition. More on how views work can be found in the [View](./VIEW_implementation) documentation.