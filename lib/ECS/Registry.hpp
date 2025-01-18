/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Registry.hpp
*/

#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include "Entity.hpp"
#include "Family.hpp"
#include "SparseSet.hpp"
#include "View.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <vector>

namespace ecs
{

/**
 * @brief The class representation of the  Registry.
 */
class Registry
{
  public:
    Registry() = default;
    ~Registry()
    {
        for (const auto sparse : this->_sparse_sets | std::views::values)
            delete sparse;
    }

    /**
     * @brief Gives a view of the entities with a set of components
     * @tparam First The first kind of component to query
     * @tparam ...Components Additional kind of components to query
     * @return A new view with entities queried.
     */
    template <typename First, typename... Components> View<First, Components...> view() { return View<First, Components...>(_sparse_sets); }

    /**
     * @brief Spawns a new Entity, used to attach components to
     * @return An new unique Entity
     */
    unsigned int create()
    {
        const auto entity = Registry::_get_new_entity_id();

        _instances++;
        _entities.emplace_back(entity);
        return entity;
    }

    /**
     * @brief Deletes an Entity and all attached components
     * @param entity The Entity to delete
     */
    void remove(const Entity entity)
    {
        _deletions++;
        _queue_remove.push_back(entity);
    }

    /**
     * @brief Call the registry flush
     */
    void flush()
    {
        for (auto entity : _queue_remove)
        {
            for (const auto& sparse : _sparse_sets | std::views::values)
                if (sparse->contains(entity))
                    sparse->remove(entity);
            _entities.erase(std::ranges::find(_entities, entity));
        }
        _queue_remove.clear();
    }

    /**
     * @brief Removes all the Entities
     */
    void clear()
    {
        for (auto const& [id, sparse] : _sparse_sets)
            sparse->clear();
        _deletions += _entities.size();
        _entities.clear();
    }

    /**
     * @brief Clears all the components of a given type
     *
     * @tparam Component The type of component to clear
     * @tparam  OtherComponents Additional types of components to clear
     */
    template <typename Component, typename... OtherComponents> void clear()
    {
        for (const auto types = {Family<Component>::id(), Family<OtherComponents>::id()...}; auto id : types)
        {
            if (_sparse_sets.contains(id))
                _sparse_sets.at(id)->clear();
        }
    }

    /**
     * @brief Attaches a Component to an Entity
     * @tparam T The type of component to attach. Can be deduced by the parameter
     * @param entity The Entity to attach to
     * @param component The Component information to attach
     */
    template <typename T> T& addComponent(Entity entity, const T& component)
    {
        const auto id = Family<T>::id();
        SparseSet<T>* set = nullptr;

        if (!this->_sparse_sets.contains(id))
        {
            set = new SparseSet<T>();
            this->_sparse_sets[id] = set;
        }
        else
            set = dynamic_cast<SparseSet<T>*>(this->_sparse_sets.at(id));
        set->set(entity, component);
        return set->at(entity);
    }

    /**
     * @brief Removes a component from an Entity
     * @tparam T The type of component to remove
     * @param entity The Entity to remove from
     */
    template <typename T> void removeComponent(Entity entity)
    {
        const auto id = Family<T>::id();

        if (!this->_sparse_sets.contains(id))
        {
            return;
        }
        this->_sparse_sets.at(id)->remove(entity);
    }

    /**
     * @brief Used as debug to print out all the sparse-sets data
     */
    void displaySparse() const
    {
        std::cout << "There is a sparse array for the following components: " << std::endl;
        for (const auto [id, set] : _sparse_sets)
        {
            std::cout << id << std::endl;
            set->display();
        }
    }

    /**
     * @brief Checks if an Entity has a component
     *
     * @tparam Component The type of component to check
     * @tparam OtherComponents Additional types of components to check
     *
     * @param entity The Entity to check
     *
     * @return True if the Entity has the component, false otherwise
     */
    template <typename Component, typename... OtherComponents> bool has_all_of(const ecs::Entity& entity)
    {

        for (const auto types = {Family<Component>::id(), Family<OtherComponents>::id()...}; auto id : types)
        {
            if (!_sparse_sets.contains(id))
                return false;
            if (!_sparse_sets.at(id)->contains(entity))
                return false;
        }
        return true;
    }

    /**
     * @brief Checks if an Entity has any of the components
     *
     * @tparam Component The type of component to check
     * @tparam OtherComponents Additional types of components to check
     *
     * @param entity The Entity to check
     *
     * @return True if the Entity has any of the components, false otherwise
     */
    template <typename Component, typename... OtherComponents> bool has_any_of(const ecs::Entity& entity)
    {

        for (const auto types = {Family<Component>::id(), Family<OtherComponents>::id()...}; auto id : types)
        {
            if (_sparse_sets.contains(id) && _sparse_sets.at(id)->contains(entity))
                return true;
        }
        return false;
    }

    /**
     * @brief Gets a component from an Entity
     *
     * @tparam T The type of component to get
     *
     * @param entity The Entity to get the component from
     *
     * @return The component
     */
    template <typename T> T& get(const Entity entity)
    {
        const auto id = Family<T>::id();

        if (!this->_sparse_sets.contains(id))
        {
            throw std::out_of_range("No such component in the registry:" + Family<T>::name());
        }

        const auto set = dynamic_cast<SparseSet<T>*>(this->_sparse_sets.at(id));

        if (!set->contains(entity))
        {
            throw std::out_of_range(entity + " does not have the component " + Family<T>::name());
        }

        return set->at(entity);
    }


    const std::map<std::size_t, ISparseSet *>& sparseSets() const noexcept { return this->_sparse_sets; }
    const std::vector<Entity>& entities() const noexcept { return this->_entities; }
    uint32_t instances() const noexcept { return this->_instances; }
    uint32_t deletions() const noexcept { return this->_deletions; }

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

    std::vector<Entity> _queue_remove = {};
    std::vector<Entity> _entities = {};
    std::map<std::size_t, ISparseSet*> _sparse_sets = {};

    uint32_t _instances = 0;
    uint32_t _deletions = 0;
};
} // namespace ecs

#endif // REGISTRY_HPP
