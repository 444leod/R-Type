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
class View
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
    ~View() = default;

    Iterator begin() {
        return Iterator(_entities.begin(), _sparse_sets);
    }

    Iterator end() {
        return Iterator(_entities.end(), _sparse_sets);
    }

    // std::vector<std::pair<Entity, std::tuple<Component, Others...>>> get()
    // {
    //     std::vector<std::tuple<Component, Others...>> components;
    //     std::vector<Entity> entities;
    //
    //     for (const auto &component : _type_ids)
    //     {
    //         if (!_sparse_sets.contains(component))
    //         {
    //             std::cerr << "Component " << component << " is not in the registry" << std::endl;
    //             return {};
    //         }
    //     }
    //
    //     const auto first = _type_ids[0];
    //     const auto sparse = dynamic_cast<SparseSet<Component> *>(_sparse_sets.at(first));
    //
    //     auto entitiesList = sparse->getEntities();
    //     for (const auto &entity : sparse->getEntities())
    //     {
    //         std::tuple<Component, Others...> tuple;
    //         Component &res = sparse->get(entity);
    //         std::get<Component>(tuple) = sparse->get(entity);
    //         if constexpr (sizeof...(Others) > 0)
    //         {
    //             if (_updateComponentsTuple<Others...>(entity, tuple))
    //             {
    //                 components.push_back(tuple);
    //                 entities.push_back(entity);
    //             }
    //         }
    //         else
    //         {
    //             components.push_back(tuple);
    //             entities.push_back(entity);
    //         }
    //     }
    //
    //     std::vector<std::pair<Entity, std::tuple<Component, Others...>>> result;
    //     for (Entity i = 0; i < components.size(); i++)
    //     {
    //         result.push_back(std::make_pair(entities[i], components[i]));
    //     }
    //     return result;
    // }

    friend std::ostream &operator<<(std::ostream &os, const View &view)
    {
        std::vector<Entity> identifiers = view._type_ids;
        if (identifiers.empty())
            return os;
        os << identifiers[0];
        identifiers.erase(identifiers.begin());
        for (const auto id : identifiers)
            os << ", " << id;
        return os;
    }

private:
    std::map<std::size_t, ISparseSet *> _sparse_sets;
    std::vector<std::size_t> _type_ids;
    std::vector<Entity> _entities;

    // template <typename T>
    // bool _updateComponentTuple(Entity entity, std::tuple<Component, Others...> &tuple)
    // {
    //     const auto id = type<T>::id();
    //     auto sparse = dynamic_cast<SparseSet<T> *>(_sparse_sets.at(id));
    //     std::get<T>(tuple) = sparse->get(entity);
    //     return true;
    // }
    //
    // template <typename T, typename... Remaining>
    // bool _updateComponentsTuple(Entity entity, std::tuple<Component, Others...> &tuple)
    // {
    //     const auto id = type<T>::id();
    //     auto sparse = dynamic_cast<SparseSet<T> *>(_sparse_sets.at(id));
    //     if (!sparse->contains(entity))
    //         return false;
    //     std::get<T>(tuple) = sparse->get(entity);
    //     if constexpr (sizeof...(Remaining) > 0)
    //     {
    //         return _updateComponentTuple<Remaining...>(entity, tuple);
    //     }
    //     return true;
    // }

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
