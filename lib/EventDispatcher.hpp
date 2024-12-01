/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EventDispatcher
*/

#pragma once

#include <map>
#include <vector>
#include "Events.hpp"
#include "Family.hpp"

/// @brief A class used to register events listeners and call events based on a type
class EventDispatcher
{
public:
    EventDispatcher() = default;
    ~EventDispatcher() = default;

    /// @brief Link an event to a receiver
    /// @tparam T The type of event to link to
    /// @param handler The receiver object
    template<typename T>
    void link(IEventHandler *handler)
    {
        const std::size_t type = Family::type<T>();
        auto find = this->_links.find(type);
        if (find == this->_links.end())
            this->_links[type] = {};
        this->_links[type].push_back(handler);
    }

    /// @brief Unlink an event to a receiver
    /// @tparam T The type of event to unlink to
    /// @param handler The receiver object
    template<typename T>
    void unlink(IEventHandler *handler)
    {
        const std::size_t type = Family::type<T>();
        auto find = this->_links.find(type);
        if (find != this->_links.end())
            this->_links.erase(find);
    }

    /// @brief Call all the even listeners
    /// @tparam T The type of event to boradcast
    /// @param event The value to pass as event
    template<typename T>
    void broadcast(const T& event)
    {
        const std::size_t type = Family::type<T>();
        auto destinees = this->_links[type];
        for (auto destinee: destinees) {
            auto handler = dynamic_cast<EventHandler<T> *>(destinee);
            handler->receive(event);
        }
    }

protected:
private:
    std::map<std::size_t, std::vector<IEventHandler *>> _links = {};
};
