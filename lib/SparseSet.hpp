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

    virtual bool contains(entity_id entity) const = 0;
    virtual void erase(entity_id entity) = 0;
    friend std::ostream& operator<<(std::ostream& os, const ISparseSet& sparse) { return os; }
};

template <typename T>
class SparseSet final : public ISparseSet
{
public:
    SparseSet() = default;
    ~SparseSet() override = default;

    void set(entity_id entity, const T& component) { this->_map[entity] = component; }
    void erase(entity_id entity) override { this->_map.erase(entity); }

    [[nodiscard]] bool contains(entity_id entity) const { return this->_map.contains(entity); }

    T &get(entity_id entity) { return this->_map.at(entity); }
    const T &get(entity_id entity) const { return this->_map.at(entity); }

    [[nodiscard]] std::vector<entity_id> getEntities() const
    {
        std::vector<entity_id> entities;
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
    std::map<entity_id, T> _map = {};
};

#endif //SPARSESET_HPP
