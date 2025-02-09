/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** InputActivateSystem.hpp
*/

#ifndef INPUTEVENTSYSTEM_HPP
#define INPUTEVENTSYSTEM_HPP

#include "PremadeComponents/Displayable/Input.hpp"

#include <PremadeComponents/Transform.hpp>
#include <PremadeSystems/Abstracts/AEventSystem.hpp>

class InputActivateSystem final : public AEventSystem
{
public:
    explicit InputActivateSystem() : AEventSystem("InputActivateSystem") {}

    void execute(sf::Event& event) override
    {
        _registry.view<Input, Transform>().each(
            [&](Input& input, const Transform& transform)
            {
                if (input.disabled)
                {
                    input.active = false;
                    return;
                }
                if (event.mouseButton.x < transform.x || event.mouseButton.x > transform.x + input.box.width || event.mouseButton.y < transform.y || event.mouseButton.y > transform.y + input.box.height)
                    input.active = false;
                else
                    input.active = true;
            });
    }
}; 

#endif //INPUTEVENTSYSTEM_HPP
