/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** View.hpp
*/

#ifndef VIEW_HPP
#define VIEW_HPP

#include "Entity.hpp"
#include "Family.hpp"
#include "SparseSet.hpp"

#include <cstdint>
#include <iostream>
#include <map>
#include <ranges>
#include <tuple>
#include <vector>

inline std::ostream& operator<<(std::ostream& os, const std::vector<ecs::Entity>& vec)
{
    if (vec.empty())
        return os << "[]";
    os << "[" << vec[0];
    for (size_t i = 1; i < vec.size(); ++i)
        os << ", " << vec[i];
    return os << "]";
}

namespace ecs
{

/**
 * @brief Trait to check if T is in the list of types.
 */
template <typename T, typename... Types> struct is_part_of : std::false_type
{
};

/**
 * @brief Recursive case: T is part of the list if it matches the first type or any of the remaining types.
 */
template <typename T, typename First, typename... Rest> struct is_part_of<T, First, Rest...> : std::conditional_t<std::is_same_v<T, First>, std::true_type, is_part_of<T, Rest...>>
{
};

/**
 * @brief Helper variable template to simplify usage.
 */
template <typename T, typename... Types> inline constexpr bool is_part_of_v = is_part_of<T, Types...>::value;

/**
 * @brief Define the part_of concept using the is_part_of struct.
 */
template <typename T, typename... Types>
concept part_of = is_part_of_v<T, Types...>;

/**
 * @brief View class to iterate over entities with specific components.
 *
 * @tparam Component The main component type.
 * @tparam Others Other component types.
 */
template <typename Component, typename... Others> class View
{
  public:
    using ComponentsTuple = std::tuple<Component, Others...>;

    /**
     * @brief Iterator class to iterate over entities and their components.
     */
    class Iterator
    {
      public:
        /**
         * @brief Constructor to initialize the iterator.
         *
         * @param entityIdsIterator Iterator to the entity IDs.
         * @param sparse_sets Reference to the map of sparse sets.
         */
        Iterator(const std::vector<size_t>::iterator entityIdsIterator, std::map<std::size_t, ISparseSet*>& sparse_sets) : _entityIdsIterator{entityIdsIterator}, _sparse_sets{sparse_sets} {}

        /**
         * @brief Dereference operator to get the entity and its components.
         *
         * @return A tuple containing the entity and its components.
         */
        std::tuple<const ecs::Entity&, Component&, Others&...> operator*()
        {
            const ecs::Entity& entity = *_entityIdsIterator;
            return std::tie(entity, this->get<Component>(entity), this->get<Others>(entity)...);
        }

        /**
         * @brief Pre-increment operator.
         *
         * @return Reference to the incremented iterator.
         */
        Iterator& operator++()
        {
            ++_entityIdsIterator;
            return *this;
        }

        /**
         * @brief Post-increment operator.
         *
         * @return Copy of the iterator before incrementing.
         */
        Iterator operator++(int)
        {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        /**
         * @brief Equality comparison operator.
         *
         * @param other The other iterator to compare with.
         *
         * @return True if the iterators are equal, false otherwise.
         */
        bool operator==(const Iterator& other) const { return _entityIdsIterator == other._entityIdsIterator; }

        /**
         * @brief Inequality comparison operator.
         *
         * @param other The other iterator to compare with.
         *
         * @return True if the iterators are not equal, false otherwise.
         */
        bool operator!=(const Iterator& other) const { return _entityIdsIterator != other._entityIdsIterator; }

      private:
        /**
         * @brief Helper function to get a component of an entity.
         *
         * @tparam T The component type.
         * @param entity The entity to get the component from.
         *
         * @return Reference to the component.
         */
        template <typename T> [[nodiscard]] T& get(const ecs::Entity& entity) { return dynamic_cast<SparseSet<T>&>(*_sparse_sets.at(Family<T>::id())).at(entity); }

        std::vector<size_t>::iterator _entityIdsIterator;
        std::map<std::size_t, ISparseSet*>& _sparse_sets;
    };

  public:
    /**
     * @brief Constructor to initialize the view with the sparse sets.
     *
     * @param sparse_sets Reference to the map of sparse sets.
     */
    explicit View(std::map<std::size_t, ISparseSet*>& sparse_sets) : _type_ids{Family<Component>::id(), Family<Others>::id()...}, _sparse_sets{sparse_sets}
    {
        // std::cout << "View types: " << type<Component>::name();
        // ((std::cout << ", " << type<Others>::name()), ...);
        // std::cout << std::endl;
        this->refresh();
    }

    /**
     * @brief Destructor to remove the view as an observer from the sparse sets.
     */
    ~View() = default;

    /**
     * @brief Refresh the view by querying the entities again.
     */
    void refresh() { this->_queryEntities(); }

    /**
     * @brief Return an iterator to the beginning.
     *
     * @return Iterator to the beginning.
     */
    Iterator begin() { return Iterator(_entities.begin(), _view_sets); }

    /**
     * @brief Return an iterator to the end.
     *
     * @return Iterator to the end.
     */
    Iterator end() { return Iterator(_entities.end(), _view_sets); }

    /**
     * @brief Return the number of entities in the view.
     *
     * @return Number of entities.
     */
    [[nodiscard]] std::size_t size() const { return _entities.size(); }

    /**
     * @brief Iterate over each entity and its components, applying the given function.
     *
     * @tparam Func The type of the function to apply.
     * @param func The function to apply to each entity and its components.
     */
    template <typename Func> void each(Func&& func)
    {
        // std::cout << "each with entities: " << _entities << std::endl;
        for (auto entity : _entities)
        {
            if (const auto entity_it = std::ranges::find(_entities, entity); entity_it == _entities.end())
                continue;
            if constexpr (std::is_invocable_v<Func, Entity, Component&, Others&...>)
            {
                func(entity, this->get<Component>(entity), this->get<Others>(entity)...);
            }
            else
            {
                func(this->get<Component>(entity), this->get<Others>(entity)...);
            }
        }
    }

    /**
     * @brief Get a component of an entity.
     *
     * @tparam T The component type (must be part of the view).
     * @param entity The entity to get the component from.
     *
     * @return Reference to the component.
     */
    template <typename T>
        requires part_of<T, Component, Others...>
    [[nodiscard]] T& get(const ecs::Entity& entity)
    {
        return dynamic_cast<SparseSet<T>&>(*_view_sets.at(Family<T>::id())).at(entity);
    }

    /**
     * @brief Return a vector of tuples containing each entity and its components.
     *
     * @return Vector of tuples with entities and their components.
     */
    std::vector<std::tuple<Entity, Component&, Others&...>> each()
    {
        std::vector<std::tuple<Entity, Component&, Others&...>> result;
        for (auto tuple : *this)
        {
            result.push_back(tuple);
        }
        return result;
    }

    /**
     * @brief Return a vector of entities in the view.
     *
     * @return Vector of entities.
     */
    [[nodiscard]] std::vector<Entity> entities() const noexcept { return _entities; }

    /**
     * @brief Output stream operator to print the view.
     *
     * @param os The output stream.
     * @param view The view to print.
     *
     * @return Reference to the output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const View& view)
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

    void displaySets()
    {
        for (const auto& [id, set] : _view_sets)
        {
            std::cout << "Sparse set for component " << id << std::endl;
            set->display();
        }
    }

  private:
    std::map<std::size_t, ISparseSet*>& _sparse_sets;
    std::map<std::size_t, ISparseSet*> _view_sets;
    std::vector<std::size_t> _type_ids;
    std::vector<Entity> _entities;

  private:
    /**
     * @brief Query the entities that have the required components.
     */
    void _queryEntities()
    {
        std::vector<std::vector<Entity>> entitiesList;
        uint8_t minimum = -1;
        uint8_t index = 0;

        for (const auto& id : _type_ids)
        {
            if (!_sparse_sets.contains(id))
            {
                // std::cerr << "Component " << id << " is not in the registry" << std::endl;
                return;
            }
            const auto& set = _sparse_sets.at(id);
            if (!set)
                return;
            _view_sets[id] = set;
            auto list = set->entities();

            if (list.size() < minimum)
            {
                minimum = list.size();
                index = entitiesList.size();
            }

            entitiesList.push_back(list);
        }

        std::vector<Entity> entitiesListIntersection = entitiesList[index];

        // TODO: optimize
        std::vector<Entity> result;

        for (const auto& element : entitiesListIntersection)
        {
            bool foundInAll = true;
            for (const auto& vec : entitiesList)
            {
                if (std::ranges::find(vec, element) == vec.end())
                {
                    foundInAll = false;
                    break;
                }
            }
            if (foundInAll)
            {
                result.push_back(element);
            }
        }

        _entities = result;
    }
};

} // namespace ecs

#endif // VIEW_HPP
