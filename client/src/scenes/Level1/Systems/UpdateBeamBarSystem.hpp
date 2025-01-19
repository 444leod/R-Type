/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** UpdateBeamBarSystem
*/

#ifndef UPDATE_BEAM_BAR_SYSTEM_HPP_
#define UPDATE_BEAM_BAR_SYSTEM_HPP_

#include <Engine/Systems/ASystem.hpp>
#include <algorithm>
#include <memory>
#include <SFML/System/Clock.hpp>

#include "PremadeComponents/Shapes/Rectangle.hpp"
#include "../Component/BeamBar.hpp"

#include "Config.hpp"


class UpdateBeamBarSystem final : public engine::ASystem
{
  public:
    explicit UpdateBeamBarSystem() : ASystem("UpdateBeamBarSystem") {}

    void execute(std::shared_ptr<sf::Clock> spaceClock) const
    {
        std::uint32_t pourcentage = 0;

        if (spaceClock != nullptr) {
            pourcentage = std::min(static_cast<int>(spaceClock->getElapsedTime().asSeconds() * 100), 100);
            std::cout << "pourcentage" << pourcentage * SCALE << std::endl;
        }
        _registry.view<shape::Rectangle, BeamBar>().each(
            [pourcentage](auto& rectangle, auto& beamBar) {
                rectangle.width = pourcentage * SCALE;
                rectangle.fillColor = {
                    static_cast<sf::Uint8>(31 + pourcentage),
                    static_cast<sf::Uint8>(81 + pourcentage),
                    255,
                    255
                };            }
        );
    };

};

#endif /* !UPDATE_BEAM_BAR_SYSTEM_HPP_ */
