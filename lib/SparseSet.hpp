/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** SparseSet
*/

#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>
#include "Entity.hpp"

/// @brief Class representation of a Sparse Set
/// @tparam T The type of data to be held
template<typename T>
class SparseSet
{
public:
    SparseSet() = default;
    ~SparseSet() = default;

    /// @brief Check if the entity is contained in the set
    /// @param entity The entity to check for
    /// @return `true` if the entity is in the set. `false` otherwise
    bool contains(const Entity& entity) const noexcept
    {
        return entity < this->_sparse.size()
            && this->_sparse[entity] < this->_dense.size()
            && entity == this->_dense[this->_sparse[entity]];
    }

    /// @brief Gets the component linked to an entity
    /// @param entity The entity to get the component of
    /// @return A reference to an entiy
    T& at(const Entity& entity)
    {
        if (!this->contains(entity))
            throw std::out_of_range("Entity not present in the set.");
        return this->_components[this->_sparse[entity]];
    }

    /// @brief Sets the component of a entity
    /// @param entity The entity to set the component to
    /// @param component The component
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
    }

    /// @brief Remove an entity from a set
    /// @param entity The entity to remove from the set
    void remove(const Entity& entity)
    {
        T& last_c = this->_components.back();
        std::size_t last_e = this->_dense.back();
        // Swap the Entity and Component to the end
        this->_swap(this->_dense.back(), this->_dense[this->_sparse[entity]]);
        this->_swap(this->_components.back(), this->_components[this->_sparse[entity]]);
        // Swap their IDs in the sparse
        this->_swap(this->_sparse[last_e], this->_sparse[entity]);
        // Delete in dense lists
        this->_dense.pop_back();
        this->_components.pop_back();
    }

    /// @brief Gets the amount of components registered
    /// @return std::size_t
    std::size_t size() const noexcept { return this->_dense.size(); }
    /// @brief Gets the capacity of the set
    /// @return std::size_t
    std::size_t capacity() const noexcept { return this->_sparse.capacity(); }

private:

    /// @brief Adds an entity in the Sparse if it can, resize otherwise
    /// @param entity The entity to add to
    /// @param value The value to set to the entity id
    void _add_in_sparse(const Entity& entity, std::size_t value)
    {
        const std::size_t cap = this->_sparse.capacity();
        if (cap <= entity)
            this->_sparse.resize(cap * 2);
        this->_sparse[entity] = value;
    }

    /// @brief Swaps two values
    /// @tparam U The type of the values passed as parameter
    /// @param a The first value to swap
    /// @param b The second value to swap
    template<typename U>
    void _swap(U& a, U& b)
    {
        U tmp = a;
        a = b;
        b = tmp;
    }

    std::vector<std::size_t> _sparse = { 0 };
    std::vector<std::size_t> _dense = {};
    std::vector<T> _components = {};
};
