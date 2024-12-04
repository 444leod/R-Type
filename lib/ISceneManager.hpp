/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** ISceneManager.hpp
*/

#ifndef ISCENEMANAGER_HPP
#define ISCENEMANAGER_HPP

#include <string>

class ISceneManager {
public:
    ISceneManager() = default;
    virtual ~ISceneManager() = default;

    virtual void stop() noexcept = 0;
    virtual void load(const std::string& name) = 0;
};

#endif //ISCENEMANAGER_HPP
