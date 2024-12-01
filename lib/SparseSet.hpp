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
#include "Family.hpp"

class ISparseSet {
public:
    virtual ~ISparseSet() = default;

    virtual void erase(std::size_t entity) = 0;
    friend std::ostream& operator<<(std::ostream& os, const ISparseSet& sparse) { return os; }
};

template <typename T>
class SparseSet final : public ISparseSet
{
public:
    SparseSet() = default;
    ~SparseSet() = default;

    void set(std::size_t entity, const T& component) { this->_map[entity] = component; }
    void erase(std::size_t entity) override { this->_map.erase(entity); }

    bool contains(std::size_t entity) const { return this->_map.find(entity) != this->_map.end(); }

    T &get(std::size_t entity) { return this->_map.at(entity); }
    const T &get(std::size_t entity) const { return this->_map.at(entity); }

    std::vector<std::size_t> getEntities() const
    {
        std::vector<std::size_t> entities;
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
    std::map<std::size_t, T> _map = {};
};

#endif //SPARSESET_HPP
