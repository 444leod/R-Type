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

template <typename Component, typename... Others>
class View final : public ISparseSetObserver
{
    using ComponentsTuple = std::tuple<Component, Others...>;

    class Iterator {
    public:
        Iterator(std::vector<size_t>::iterator entityIdsIterator, std::map<std::size_t, ISparseSet *> &sparse_sets)
            : _entityIdsIterator{entityIdsIterator}, _sparse_sets{sparse_sets} {}

        struct EntityComponents {
            Entity entity;
            ComponentsTuple components;
        };

        EntityComponents operator*() {
            size_t entity = *_entityIdsIterator;
            return {entity, std::tie(dynamic_cast<SparseSet<Component>&>(*_sparse_sets.at(type<Component>::id())).get(entity),
                                     dynamic_cast<SparseSet<Others>&>(*_sparse_sets.at(type<Others>::id())).get(entity)...)};
        }

        Iterator& operator++() {
            ++_entityIdsIterator;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator& other) const {
            return _entityIdsIterator == other._entityIdsIterator;
        }

        bool operator!=(const Iterator& other) const {
            return _entityIdsIterator != other._entityIdsIterator;
        }

    private:
        std::vector<size_t>::iterator _entityIdsIterator;
        std::map<std::size_t, ISparseSet *> &_sparse_sets;
    };

public:
    View(const std::map<std::size_t, ISparseSet *> &sparse_sets) : _type_ids{type<Component>::id(), type<Others>::id()...}, _sparse_sets{sparse_sets} {
        _queryEntities();
        for (auto id : _type_ids)
            dynamic_cast<SparseSet<Component> *>(_sparse_sets.at(id))->addObserver(this);
    }
    ~View() override {
        for (auto id : _type_ids)
            dynamic_cast<SparseSet<Component> *>(_sparse_sets.at(id))->removeObserver(this);
    }

    void onEntityErased(const Entity& entity) override {
        erase_if(_entities, [entity](const Entity& e) { return e == entity; });
    }

    void onEntitySet(const Entity &entity) override {
        this->refresh(); // not optimized at all, to change
    }

    void refresh() {
        _queryEntities();
    }

    Iterator begin() {
        return Iterator(_entities.begin(), _sparse_sets);
    }

    Iterator end() {
        return Iterator(_entities.end(), _sparse_sets);
    }

    template <typename Func>
    void each(Func&& func) {
        for (auto entity : _entities) {
            std::tuple<Component&, Others&...> tuple = std::tie(dynamic_cast<SparseSet<Component>&>(*_sparse_sets.at(type<Component>::id())).get(entity),
                                     dynamic_cast<SparseSet<Others>&>(*_sparse_sets.at(type<Others>::id())).get(entity)...);
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

        auto entitiesList = sparse->getEntities();

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
