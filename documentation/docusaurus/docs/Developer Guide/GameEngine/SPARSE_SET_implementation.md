---
sidebar_position: 3
id: 'SparseSetImplementation'
title: 'Sparse Set'
---

# Sparse Set

A SparseSet is an optimized data structure designed for efficient entity-component management in game engines and entity-component systems (ECS).

### Implementation

```cpp
#ifndef SPARSESET_HPP
#define SPARSESET_HPP

#include <vector>
#include <cstdint>
#include <stdexcept>
#include "Entity.hpp"

class ISparseSetObserver {
public:
    virtual ~ISparseSetObserver() = default;

    virtual void onEntitySet(const Entity& entity) = 0;
    virtual void onEntityErased(const Entity& entity) = 0;
};

class ISparseSet {
public:
    virtual ~ISparseSet() = default;

    [[nodiscard]] virtual bool contains(const Entity& entity) const noexcept = 0;
    virtual void remove(const Entity& entity) = 0;
    virtual void clear() = 0;
    [[nodiscard]] virtual const std::vector<Entity>& entities() const noexcept = 0;
    [[nodiscard]] virtual std::size_t size() const noexcept = 0;
    [[nodiscard]] virtual std::size_t capacity() const noexcept = 0;
    virtual void addObserver(ISparseSetObserver *observer) noexcept = 0;
    virtual void removeObserver(ISparseSetObserver *observer) noexcept = 0;
    friend std::ostream& operator<<(std::ostream& os, const ISparseSet& sparse) { return os; }
};

/**
 * @brief Class representation of a Sparse Set
 * @tparam T The type of data to be held
 */
template<typename T>
class SparseSet final : public ISparseSet
{
public:
    SparseSet() = default;
    ~SparseSet() override = default;

public:
    /**
     * @brief Check if the entity is contained in the set
     * @param entity The entity to check for
     * @return `true` if the entity is in the set. `false` otherwise
     */
    [[nodiscard]] bool contains(const Entity& entity) const noexcept override
    {
        return entity < this->_sparse.size()
            && this->_sparse[entity] < this->_dense.size()
            && entity == this->_dense[this->_sparse[entity]];
    }

    /**
     * @brief Gets the component linked to an entity
     * @param entity The entity to get the component of
     * @return A reference to an entiy
     */
    [[nodiscard]] T& at(const Entity& entity)
    {
        if (!this->contains(entity))
            throw std::out_of_range("Entity not present in the set.");
        return this->_components[this->_sparse[entity]];
    }

    /**
     * @brief Sets the component of a entity
     * @param entity The entity to set the component to
     * @param component The component
     */
    void set(const Entity& entity, T component)
    {
        if (this->contains(entity)) {
            this->_components[this->_sparse[entity]] = component;
        } else {
            const std::size_t size = this->_dense.size();
            this->_dense.push_back(entity);
            this->_components.push_back(component);
            this->_add_in_sparse(entity, size);
        }

        for (auto observer : this->_observers)
            observer->onEntitySet(entity);
    }

    /**
     * @brief Remove an entity from a set
     * @param entity The entity to remove from the set
     */
    void remove(const Entity& entity) override
    {
        if (!this->contains(entity))
            return;

        T& last_c = this->_components.back();
        Entity last_e = this->_dense.back();
        // Swap the Entity and Component to the end
        this->_swap(this->_dense.back(), this->_dense[this->_sparse[entity]]);
        this->_swap(this->_components.back(), this->_components[this->_sparse[entity]]);
        // Swap their IDs in the sparse
        this->_swap(this->_sparse[last_e], this->_sparse[entity]);
        // Delete in dense lists
        this->_dense.pop_back();
        this->_components.pop_back();

        for (auto observer : this->_observers)
            observer->onEntityErased(entity);
    }

    /**
     * @brief Clears the entire set
     */
    void clear() noexcept override
    {
        this->_sparse.clear();
        this->_dense.clear();
        this->_components.clear();
    }

    /**
     * @brief Add an observer to this set
     * @param observer The observer to be added
     */
    void addObserver(ISparseSetObserver *observer) noexcept override { this->_observers.push_back(observer); }
    /**
     * @brief Remove an observer from this set
     * @param observer The observer to be removed
     */
    void removeObserver(ISparseSetObserver *observer) noexcept override { std::erase(this->_observers, observer); }

    /**
     * @brief Returns the list of entities contained in the set
     * @return A reference to an std::vector of entities
     */
    [[nodiscard]] const std::vector<Entity>& entities() const noexcept override { return this->_dense; }
    /**
     * @brief Gets the amount of components registered
     * @return std::size_t
     */
    [[nodiscard]] std::size_t size() const noexcept override { return this->_dense.size(); }
    /**
     * @brief Gets the capacity of the set
     * @return std::size_t
     */
    [[nodiscard]] std::size_t capacity() const noexcept override { return this->_sparse.capacity(); }

private:
    /**
     * @brief Adds an entity in the Sparse if it can, resize otherwise
     * @param entity The entity to add to
     * @param value The value to set to the entity id
     */
    void _add_in_sparse(const Entity& entity, std::size_t value)
    {
        if (this->_sparse.capacity() <= entity) {
            this->_sparse.resize(this->_compute_resize(entity));
        }
        this->_sparse[entity] = value;
    }

    /**
     * @brief O1 method to compute the necessary size a vector should have (2^n)
     * @param k The minimum size
     */
    [[nodiscard]] static std::size_t _compute_resize(std::size_t k)
    {
        if (k == 0)
            return 1;
        k |= k >> 1;
        k |= k >> 2;
        k |= k >> 3;
        k |= k >> 4;
        k |= k >> 8;
        k |= k >> 16;
        k |= k >> 32;
        return k + 1;
    }

    /**
     * @brief Swaps two values
     * @tparam U The type of the values passed as parameter
     * @param a The first value to swap
     * @param b The second value to swap
     */
    template<typename U>
    static void _swap(U& a, U& b)
    {
        U tmp = a;
        a = b;
        b = tmp;
    }

private:
    std::vector<std::size_t> _sparse = { };
    std::vector<Entity> _dense = {};
    std::vector<T> _components = {};
    std::vector<ISparseSetObserver *> _observers = {};
};

#endif //SPARSESET_HPP
```

### Explanation
The primary goals of a sparse set primary goals are:

1. **Memory Efficiency**
   The SparseSet uses a compact memory representation that minimizes memory usage and fragmentation. It consists of three main vectors:
   - `_sparse`: A mapping that allows O(1) lookups
   - `_dense`: A compact array of entities
   - `_components`: A corresponding array of component data

2. **Fast Operations**
   The SparseSet provides O(1) complexity for key operations:
   - Checking if an entity exists
   - Adding components
   - Removing components
   - Accessing components

3. **Dynamic Entity Management**
   It allows dynamic addition and removal of entities and their associated components with minimal computational overhead.

Here's a breakdown of its key features:

**Memory Layout**
```
sparse: [0] -> 2   # Maps entity ID to index in dense/components
sparse: [1] -> 0
sparse: [2] -> 1
dense:  [0] -> 1   # Actual entity IDs
dense:  [1] -> 2
dense:  [2] -> 0
components: [0] -> ComponentData1
components: [1] -> ComponentData2
components: [2] -> ComponentData3
```

**Key Methods**
- `contains(entity)`: Quickly checks if an entity exists in O(1) time
- `set(entity, component)`: Adds or updates a component for an entity
- `remove(entity)`: Removes an entity and its component efficiently
- `at(entity)`: Retrieves a component for a specific entity

**Observer Pattern**
The SparseSet also implements an observer pattern, allowing other parts of the system to be notified when entities are added or removed.

**Performance Characteristics**
- Insertion: O(1)
- Removal: O(1)
- Lookup: O(1)
- Memory: Proportional to the number of entities with components

### Example

```cpp
SparseSet<Position> positionSet;

// Add components
positionSet.set(player1, {10, 20});
positionSet.set(player2, {30, 40});

// Check if entity exists
bool exists = positionSet.contains(player1); // true

// Remove an entity
positionSet.remove(player1);
```

The design is particularly clever because:
1. It uses a sparse-dense array technique to provide fast lookups
2. It minimizes memory fragmentation
3. It provides constant-time operations for most common operations
4. It's type-safe and templated, allowing use with any component type

This implementation is a fundamental building block in modern game engine architectures, providing an efficient way to manage entities and their components with minimal overhead.

:::info
This implementation was inspired by the logic explain in this article [Sparse Set](https://skypjack.github.io/2020-08-02-ecs-baf-part-9/).
:::