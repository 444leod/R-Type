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
    [[nodiscard]] bool contains(const ecs::Entity& entity) const noexcept override
    {
        if (entity >= this->_sparse.size()) {
            return false;
        }

        const auto sparseIndex = this->_sparse[entity];
        if (sparseIndex >= this->_dense.size()) {
            return false;
        }

        const auto denseEntity = this->_dense[sparseIndex];
        if (entity != denseEntity) {
            if (entity == 6 || entity == 7) {
                std::cout << "Entity " << entity << " mismatch in dense array:" << std::endl;
                std::cout << "Sparse[" << entity << "] = " << sparseIndex << std::endl;
                std::cout << "Dense[" << sparseIndex << "] = " << denseEntity << std::endl;
                std::cout << "Dense size: " << this->_dense.size() << std::endl;
                std::cout << "Components size: " << this->_components.size() << std::endl;
            }
            return false;
        }

        return true;
    }

    /**
     * @brief Gets the component linked to an entity
     * @param entity The entity to get the component of
     * @return A reference to an entiy
     */
    [[nodiscard]] T& at(const ecs::Entity& entity)
    {
        if (entity >= this->_sparse.size()) {
            throw std::out_of_range("Entity " + std::to_string(entity) + " is out of range for set " + Family<T>::name());
        }

        if (!this->contains(entity)) {
            throw std::out_of_range("Entity " + std::to_string(entity) + " is not in the set " + Family<T>::name());
        }

        const auto index = this->_sparse[entity];
        if (index >= this->_components.size()) {
            throw std::out_of_range("Invalid component index for entity " + std::to_string(entity) + " in set " + Family<T>::name());
        }

        return this->_components[index];
    }

    /**
     * @brief Sets the component of a entity
     * @param entity The entity to set the component to
     * @param component The component
     */
    T& set(const ecs::Entity& entity, T component)
    {
        std::cout << "Setting component for entity " << entity << " in set " << Family<T>::name() << std::endl;

        if (entity >= this->_sparse.size()) {
            const std::size_t newSize = this->_compute_resize(entity);
            std::cout << "Resizing sparse array from " << this->_sparse.size() << " to " << newSize << std::endl;
            this->_sparse.resize(newSize, this->_dense.size());
        }

        if (this->contains(entity))
        {
            std::cout << "Entity " << entity << " already exists in set, updating component" << std::endl;
            this->_components[this->_sparse[entity]] = component;
        }
        else
        {
            std::cout << "Adding new entity " << entity << " to set" << std::endl;
            const std::size_t size = this->_dense.size();
            this->_dense.push_back(entity);
            this->_components.push_back(component);
            this->_sparse[entity] = size;
            std::cout << "Added entity " << entity << " at index " << size << std::endl;
        }
        return this->_components[this->_sparse[entity]];
    }

    /**
     * @brief Remove an entity from a set
     * @param entity The entity to remove from the set
     */
    void remove(const ecs::Entity& entity) override
    {
        std::cout << "Attempting to remove entity " << entity << " from set " << Family<T>::name() << std::endl;
        if (!this->contains(entity)) {
            std::cout << "Entity " << entity << " not found in set" << std::endl;
            return;
        }

        const auto currentIndex = this->_sparse[entity];
        const auto lastIndex = this->_dense.size() - 1;

        std::cout << "Removing entity " << entity << " from index " << currentIndex << std::endl;

        if (currentIndex != lastIndex) {
            Entity lastEntity = this->_dense[lastIndex];
            std::cout << "Moving last entity " << lastEntity << " from index " << lastIndex << " to " << currentIndex << std::endl;

            this->_dense[currentIndex] = lastEntity;
            this->_components[currentIndex] = this->_components[lastIndex];
            this->_sparse[lastEntity] = currentIndex;
        }

        this->_dense.pop_back();
        this->_components.pop_back();

        if (entity < this->_sparse.size()) {
            this->_sparse[entity] = this->_dense.size() + 1;
            std::cout << "Set invalid index " << this->_dense.size() + 1 << " for removed entity " << entity << std::endl;
        }
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
        std::cout << "Adding entity " << entity << " to sparse array with value " << value << std::endl;
        if (this->_sparse.capacity() <= entity)
        {
            std::size_t newSize = this->_compute_resize(entity);
            std::cout << "Resizing sparse array from " << this->_sparse.capacity() << " to " << newSize << std::endl;

            auto oldSize = this->_sparse.size();
            auto oldValues = this->_sparse;

            this->_sparse.resize(newSize, this->_dense.size() + 1);

            for (size_t i = 0; i < oldSize; ++i) {
                this->_sparse[i] = oldValues[i];
            }
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
