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
#include "Family.hpp"
#include "Entity.hpp"
#include "SparseSet.hpp"
#include "View.hpp"

namespace ecs {

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
        template <typename First, typename... Components>
        View<First, Components...> view()
        {
            return View<First, Components...>(_sparse_sets);
        }

        /**
         * @brief Spawns a new Entity, used to attach components to
         * @return An new unique Entity
         */
        unsigned int create()
        {
            const auto entity = Registry::_get_new_entity_id();

            _entities.emplace_back(entity);
            return entity;
        }

        /**
         * @brief Deletes an Entity and all attached components
         * @param entity The Entity to delete
         */
        void remove(const Entity entity)
        {
            _queue_remove.push_back(entity);
        }

        /**
         * @brief Call the registry flush
         */
        void flush()
        {
            for (auto entity: _queue_remove)
            {
                for (const auto &sparse : _sparse_sets | std::views::values)
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
            for (const auto &sparse : _sparse_sets | std::views::values)
                sparse->clear();
            _entities.clear();
        }

        /**
         * @brief Attaches a Component to an Entity
         * @tparam T The type of component to attach. Can be deduced by the parameter
         * @param entity The Entity to attach to
         * @param component The Component informations to attach
         */
        template <typename T>
        void addComponent(Entity entity, const T &component)
        {
            const auto id = Family<T>::id();
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

        /**
         * @brief Removes a component from an Entity
         * @tparam T The type of component to remove
         * @param entity The Entity to remove from
         */
        template <typename T>
        void removeComponent(Entity entity)
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
            for (const auto [id, set] : _sparse_sets) {
                std::cout << id << std::endl;
                set->display();
            }
        }

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
        std::map<std::size_t, ISparseSet *> _sparse_sets = {};
    };
}

#endif //REGISTRY_HPP
