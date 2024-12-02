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
#include <memory>
#include "Family.hpp"
#include "SparseSet.hpp"

inline std::ostream &operator<<(std::ostream &os, const std::vector<std::size_t> &vec)
{
    os << "Entities: ";
    for (const auto &id : vec)
    {
        os << id << ", ";
    }
    return os;
}

template <typename Component, typename... Others>
class View
{
public:
    View(const std::map<std::size_t, ISparseSet *> &sparse_sets) : _identifiers{Family::type<Component>(), Family::type<Others>()...}, _sparse_sets{sparse_sets} {
        // for (const auto &component : _identifiers)
        // {
        //     if (!_sparse_sets.contains(component))
        //     {
        //         std::cerr << "Component " << component << " is not in the registry" << std::endl;
        //         return;
        //     }
        // }
        //
        // const auto first = _identifiers[0];
        // const auto sparse = dynamic_cast<SparseSet<Component> *>(_sparse_sets.at(first));
        //
        // auto entitiesList = sparse->getEntities();
        //
        //
        // if constexpr (sizeof...(Others) > 0)
        // {
        //     for (const auto &entity : entitiesList)
        //     {
        //         if (!_entityContainComponents<Others...>(entity))
        //         {
        //             entitiesList.erase(std::remove(entitiesList.begin(), entitiesList.end(), entity), entitiesList.end());
        //         }
        //     }
        // }
        //
        // _entities = entitiesList;
    }
    ~View() = default;

    std::vector<std::pair<std::size_t, std::tuple<Component, Others...>>> get()
    {
        std::vector<std::tuple<Component, Others...>> components;
        std::vector<std::size_t> entities;

        for (const auto &component : _identifiers)
        {
            if (!_sparse_sets.contains(component))
            {
                // std::cerr << "Component " << component << " is not in the registry" << std::endl;
                return {};
            }
        }

        const auto first = _identifiers[0];
        const auto sparse = dynamic_cast<SparseSet<Component> *>(_sparse_sets.at(first));

        for (const auto &entity : sparse->getEntities())
        {
            std::tuple<Component, Others...> tuple;
            std::get<Component>(tuple) = sparse->get(entity);
            if constexpr (sizeof...(Others) > 0)
            {
                if (_updateComponentsTuple<Others...>(entity, tuple))
                {
                    components.push_back(tuple);
                    entities.push_back(entity);
                }
            }
            else
            {
                components.push_back(tuple);
                entities.push_back(entity);
            }
        }

        std::vector<std::pair<std::size_t, std::tuple<Component, Others...>>> result;
        for (std::size_t i = 0; i < components.size(); i++)
        {
            result.push_back(std::make_pair(entities[i], components[i]));
        }
        return result;
    }

    friend std::ostream &operator<<(std::ostream &os, const View &view)
    {
        std::vector<std::size_t> identifiers = view._identifiers;
        if (identifiers.empty())
            return os;
        os << identifiers[0];
        identifiers.erase(identifiers.begin());
        for (const auto id : identifiers)
            os << ", " << id;
        return os;
    }

private:
    std::vector<std::size_t> _identifiers;
    std::map<std::size_t, ISparseSet *> _sparse_sets;

    template <typename T>
    bool _updateComponentTuple(std::size_t entity, std::tuple<Component, Others...> &tuple)
    {
        const auto id = Family::type<T>();
        if (!_sparse_sets.contains(id))
        {
            return false;
        }
        auto sparse = dynamic_cast<SparseSet<T> *>(_sparse_sets.at(id));
        std::get<T>(tuple) = sparse->get(entity);
        return true;
    }

    template <typename T, typename... Remaining>
    bool _updateComponentsTuple(std::size_t entity, std::tuple<Component, Others...> &tuple)
    {
        const auto id = Family::type<T>();
        auto sparse = dynamic_cast<SparseSet<T> *>(_sparse_sets.at(id));
        if (!sparse->contains(entity))
            return false;
        std::get<T>(tuple) = sparse->get(entity);
        if constexpr (sizeof...(Remaining) > 0)
        {
            return _updateComponentTuple<Remaining...>(entity, tuple);
        }
        return true;
    }
};

#endif //VIEW_HPP
