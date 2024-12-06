/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** View.hpp
*/

#ifndef VIEW_HPP
#define VIEW_HPP

#include <iostream>
#include <map>
#include <vector>
#include <tuple>
#include "Entity.hpp"
#include "Family.hpp"
#include "SparseSet.hpp"

/// @brief Class representation of a view / a query from a register
/// @tparam Component The first component of the query - necessary
/// @tparam ...Others Other components to query - not necessary
template <typename Component, typename... Others>
class View final : public ISparseSetObserver
{
public:
    using ComponentsTuple = std::tuple<Component, Others...>;

    /// @brief An forward iterator used to traverse through a view
    class Iterator {
    public:
        /// @brief View Iterator constructor
        /// @param entityIdsIterator An iterator to go through the entities
        /// @param sparse_sets A reference of the sparse set map from the view
        Iterator(std::vector<size_t>::iterator entityIdsIterator, std::map<std::size_t, ISparseSet *> &sparse_sets)
            : _entityIdsIterator{entityIdsIterator}, _sparse_sets{sparse_sets} {}

        struct EntityComponents {
            Entity entity;
            ComponentsTuple components;
        };

        /// @brief The dereferencing operator of the iterator
        /// @return Returns an entity - components pair
        EntityComponents operator*() {
            size_t entity = *_entityIdsIterator;
            return {entity, std::tie(dynamic_cast<SparseSet<Component>&>(*_sparse_sets.at(type<Component>::id())).at(entity),
                                     dynamic_cast<SparseSet<Others>&>(*_sparse_sets.at(type<Others>::id())).at(entity)...)};
        }

        /// @brief Forward iterating operator
        /// @return A reference to this iterator
        Iterator& operator++() {
            ++_entityIdsIterator;
            return *this;
        }

        /// @brief Forward iterating operator
        /// @param ? ???
        /// @return A reference to this iterator
        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        /// @brief Check if two iterators point to the same place
        /// @param other The other iterator to compare with
        /// @return `true` if they point to the same thing, `false` otherwise
        bool operator==(const Iterator& other) const {
            return _entityIdsIterator == other._entityIdsIterator;
        }

        /// @brief Check if two iterators point to different places
        /// @param other The other iterator to compare with
        /// @return `true` if they point to different things, `false` otherwise
        bool operator!=(const Iterator& other) const {
            return _entityIdsIterator != other._entityIdsIterator;
        }

    private:
        std::vector<size_t>::iterator _entityIdsIterator;
        std::map<std::size_t, ISparseSet *> &_sparse_sets;
    };

public:
    /// @brief Create a view from a list of sparse sets
    /// @param sparse_sets The sparse sets to include in the view
    View(const std::map<std::size_t, ISparseSet *> &sparse_sets) : _type_ids{type<Component>::id(), type<Others>::id()...}, _sparse_sets{sparse_sets} {
        this->refresh();
        for (auto id : _type_ids)
            dynamic_cast<SparseSet<Component> *>(this->_sparse_sets.at(id))->addObserver(this);
    }
    ~View() override {
        for (auto id : _type_ids)
            dynamic_cast<SparseSet<Component> *>(this->_sparse_sets.at(id))->removeObserver(this);
    }

    /// @brief Called from the `ISparseSetObserver` when a sparse set let go of an entity
    /// @param entity The entity to be removed
    void onEntityErased(const Entity& entity) override {
        std::erase_if(_entities, [entity](const Entity& e) { return e == entity; });
    }

    /// @brief Called from the `ISparseSetObserver` when an entity was added in a sparse set
    /// @param entity The entity that was added
    void onEntitySet(const Entity &entity) override {
        for (auto [_, set] : this->_sparse_sets)
            if (!set->contains(entity))
                return;
        this->_entities.push_back(entity);
    }

    /// @brief Refreshes the list of entity
    void refresh() {
        this->_queryEntities();
    }

    /// @brief Get an iterator to the first element of the view
    /// @return A new View::Iterator
    Iterator begin() {
        return Iterator(_entities.begin(), _sparse_sets);
    }

    /// @brief Get an iteraotr to the last + 1 element of the view
    /// @return A new View::Iterator
    Iterator end() {
        return Iterator(_entities.end(), _sparse_sets);
    }

    /// @brief Apply a function to each of the elements in a view (similat to a `foreach`)
    /// @tparam Func Type description of the function applied
    /// @param func The function to be applied
    template <typename Func>
    void each(Func&& func) {
        for (auto entity : _entities) {
            std::tuple<Component&, Others&...> tuple = std::tie(dynamic_cast<SparseSet<Component>&>(*_sparse_sets.at(type<Component>::id())).at(entity),
                                     dynamic_cast<SparseSet<Others>&>(*_sparse_sets.at(type<Others>::id())).at(entity)...);
            if constexpr (std::is_invocable_v<Func, Entity, Component&, Others&...>) {
                func(entity, std::get<Component&>(tuple), std::get<Others&>(tuple)...);
            } else {
                func(std::get<Component&>(tuple), std::get<Others&>(tuple)...);
            }
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const View &view)
    {
        std::vector<Entity> identifiers = view._type_ids;
        if (identifiers.empty())
            return os;
        os << "Template ids:" << std::endl << identifiers[0];
        identifiers.erase(identifiers.begin());
        for (const auto id : identifiers)
            os << ", " << id;
        os << std::endl;
        os << "Entities:" << std::endl;
        auto entities = view._entities;
        if (entities.empty())
            return os;
        os << entities[0];
        entities.erase(entities.begin());
        for (const auto entity : entities)
            os << ", " << entity;
        os << std::endl;
        return os;
    }

private:
    std::map<std::size_t, ISparseSet *> _sparse_sets;
    std::vector<std::size_t> _type_ids;
    std::vector<Entity> _entities;

private:
    /// @brief Query the list of entities that are shared by all sparse set
    void _queryEntities() {
        for (const auto &component : _type_ids)
        {
            if (!_sparse_sets.contains(component))
            {
                std::cerr << "Component " << component << " is not in the registry" << std::endl;
                return;
            }
        }

        const auto first = _type_ids[0];
        const auto sparse = dynamic_cast<SparseSet<Component> *>(_sparse_sets.at(first));

        auto entitiesList = sparse->entities();

        if constexpr (sizeof...(Others) > 0)
        {
            for (const auto &entity : entitiesList)
            {
                if (!_entityContainComponents<Others...>(entity))
                {
                    entitiesList.erase(std::remove(entitiesList.begin(), entitiesList.end(), entity), entitiesList.end());
                }
            }
        }

        _entities = entitiesList;
    }

    /// @brief Check if an entity has a list of component
    /// @tparam T The first (necessary) component
    /// @tparam ...Remaining The others components (could be empty)
    /// @param entity The entity to check for
    /// @return `true` if the entity has all the components needed, `false` otherwise
    template <typename T, typename... Remaining>
    bool _entityContainComponents(const Entity entity)
    {
        const auto id = type<T>::id();
        if (!_sparse_sets.contains(id))
        {
            return false;
        }
        auto sparse = dynamic_cast<SparseSet<T> *>(_sparse_sets.at(id));
        if (!sparse->contains(entity))
            return false;
        if constexpr (sizeof...(Remaining) > 0)
            return _entityContainComponents<Others...>(entity);
        return true;
    }
};

#endif //VIEW_HPP
