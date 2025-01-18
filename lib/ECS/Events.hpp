/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EventHandler
*/

#pragma once

namespace ecs {

/**
 * @brief Serves as a tag for any kind of structure that aims to be an event
 */
struct IEvent {};

/**
 *  @brief The base of all event handlers, used a truthful union without types
 */
class IEventHandler {
  public:
    virtual ~IEventHandler() = default;
};

/**
 * @brief An interfac to implement one event receiver method
 * @tparam T The type of event that the class wants to receive
 */
template <typename T> class EventHandler : public IEventHandler {
  public:
    virtual void receive(const T&) = 0;
};

} // namespace ecs