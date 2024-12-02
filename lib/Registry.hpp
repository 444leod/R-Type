/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Registry.hpp
*/

#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include <iostream>
#include <map>
#include <vector>
#include <cassert>
#include <algorithm>
#include "Entity.hpp"
#include "Family.hpp"
#include "SparseSet.hpp"
#include "View.hpp"

inline entity_id get_new_entity_id()
{
    static entity_id value = 0;
    return value++;
}

class Registry
{
public:
    Registry() = default;
    ~Registry() = default;

    template <typename First, typename... Components>
    View<First, Components...> view()
    {
        return View<First, Components...>(_sparse_sets);
    }

    unsigned int create()
    {
        const auto entity = get_new_entity_id();

        _entities.emplace_back(entity);
        return entity;
    }

    void remove(entity_id entity)
    {
        for (auto const &[id, sparse] : _sparse_sets)
            if (sparse->contains(entity))
                sparse->erase(entity);
        _entities.erase(std::find(_entities.begin(), _entities.end(), entity));
    }

    template <typename T>
    void addComponent(entity_id entity, const T &component)
    {
        const auto id = Family::type<T>();
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

    void displaySparse() const
    {
        std::cout << "There is a sparse array for the following components: " << std::endl;
        for (auto const &[id, sparse] : _sparse_sets)
            std::cout << id << std::endl;
    }

private:
    std::vector<entity_id> _entities = {};
    std::map<entity_id, ISparseSet *> _sparse_sets = {};
};

#endif //REGISTRY_HPP
