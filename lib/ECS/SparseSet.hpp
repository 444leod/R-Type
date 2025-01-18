/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** SparseSet
*/

#ifndef SPARSESET_HPP
#define SPARSESET_HPP

#include "Entity.hpp"
#include "Family.hpp"

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace ecs
{

class ISparseSet
{
  public:
    virtual ~ISparseSet() = default;

    [[nodiscard]] virtual bool contains(const ecs::Entity& entity) const noexcept = 0;
    virtual void remove(const ecs::Entity& entity) = 0;
    virtual void clear() = 0;
    [[nodiscard]] virtual const std::vector<Entity>& entities() const noexcept = 0;
    [[nodiscard]] virtual std::size_t size() const noexcept = 0;
    [[nodiscard]] virtual std::size_t capacity() const noexcept = 0;
    [[nodiscard]] virtual std::string type() const noexcept = 0;
    virtual void display() const = 0;
    friend std::ostream& operator<<(std::ostream& os, const ISparseSet& sparse) { return os; }
};

/**
 * @brief Class representation of a Sparse Set
 * @tparam T The type of data to be held
 */
template <typename T> class SparseSet final : public ISparseSet
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
    [[nodiscard]] bool contains(const ecs::Entity& entity) const noexcept override { return entity < this->_sparse.size() && this->_sparse[entity] < this->_dense.size() && entity == this->_dense[this->_sparse[entity]]; }

    /**
     * @brief Gets the component linked to an entity
     * @param entity The entity to get the component of
     * @return A reference to an entiy
     */
    [[nodiscard]] T& at(const ecs::Entity& entity)
    {
        if (!this->contains(entity))
            throw std::out_of_range("Entity " + std::to_string(entity) + " is not in the set");
        return this->_components[this->_sparse[entity]];
    }

    /**
     * @brief Sets the component of a entity
     * @param entity The entity to set the component to
     * @param component The component
     */
    void set(const ecs::Entity& entity, T component)
    {
        if (this->contains(entity))
        {
            this->_components[this->_sparse[entity]] = component;
        }
        else
        {
            const std::size_t size = this->_dense.size();
            this->_dense.push_back(entity);
            this->_components.push_back(component);
            this->_add_in_sparse(entity, size);
        }
    }

    /**
     * @brief Remove an entity from a set
     * @param entity The entity to remove from the set
     */
    void remove(const ecs::Entity& entity) override
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

    [[nodiscard]] virtual std::string type() const noexcept override { return Family<T>::name(); }

    void display() const override
    {
        if (this->_dense.empty())
        {
            return;
        }
        std::cout << Family<T>::name() << " set:" << std::endl;
        std::cout << "Index | Dense Index | Dense Content | Component" << std::endl;
        for (std::size_t i = 0; i < this->_dense.size(); i++)
        {
            std::cout << i << "    | sparse[" << i << "] = " << this->_sparse[i] << " | dense[" << i << "] = " << this->_dense[i] << " | component = " << &this->_components[i] << std::endl;
        }
    }

  private:
    /**
     * @brief Adds an entity in the Sparse if it can, resize otherwise
     * @param entity The entity to add to
     * @param value The value to set to the entity id
     */
    void _add_in_sparse(const ecs::Entity& entity, std::size_t value)
    {
        if (this->_sparse.capacity() <= entity)
        {
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
    template <typename U> static void _swap(U& a, U& b)
    {
        U tmp = a;
        a = b;
        b = tmp;
    }

  private:
    std::vector<std::size_t> _sparse = {};
    std::vector<Entity> _dense = {};
    std::vector<T> _components = {};
};

} // namespace ecs

#endif // SPARSESET_HPP
