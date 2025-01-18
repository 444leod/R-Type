/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ResourcesManager.hpp
*/

#ifndef RESOURCES_MANAGER_HPP
#define RESOURCES_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class ResourcesManager {
  public:
    ResourcesManager() = default;
    ~ResourcesManager() = default;

    std::map<std::string, sf::Texture>& textures() { return this->_textures; }
    std::map<std::string, sf::Font>& fonts() { return this->_fonts; }

  private:
    std::map<std::string, sf::Texture> _textures;
    std::map<std::string, sf::Font> _fonts;
};

#endif // RESOURCES_MANAGER_HPP
