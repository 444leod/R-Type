/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** SparseSet.hpp
*/

#ifndef SPARSESET_HPP
#define SPARSESET_HPP

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "Entity.hpp"
#include "Family.hpp"

class ISparseSet {
public:
    virtual ~ISparseSet() = default;

    virtual bool contains(Entity entity) const = 0;
    virtual void erase(Entity entity) = 0;
    virtual void clear() = 0;
    friend std::ostream& operator<<(std::ostream& os, const ISparseSet& sparse) { return os; }
};

template <typename T>
class SparseSet final : public ISparseSet
{
public:
    SparseSet() = default;
    ~SparseSet() override = default;

    void set(Entity entity, const T& component) { this->_map[entity] = component; }
    void erase(Entity entity) override { this->_map.erase(entity); }

    [[nodiscard]] bool contains(Entity entity) const { return this->_map.contains(entity); }

    T &get(Entity entity) { return this->_map.at(entity); }
    const T &get(Entity entity) const { return this->_map.at(entity); }

    void clear() override { this->_map.clear(); }

    [[nodiscard]] std::vector<Entity> getEntities() const
    {
        std::vector<Entity> entities;
        for (const auto &[entity, _] : _map)
            entities.push_back(entity);
        return entities;
    }

    void display()
    {
        for (const auto &[id, ptr] : _map)
            std::cout << id << ": " << ptr << std::endl;
    }

    friend std::ostream &operator<<(std::ostream &os, const SparseSet &sparse)
    {
        os << "\tSparse for component " << Family::type<T>() << ": " << std::endl;
        for (const auto &[id, ptr] : sparse._map)
            os << "\t\t" << id << ": " << *ptr << std::endl;
        return os;
    }

private:
    std::map<Entity, T> _map = {};
};

#endif //SPARSESET_HPP
