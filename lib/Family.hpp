/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Family.hpp
*/

#ifndef FAMILY_HPP
#define FAMILY_HPP

#include <cstddef>

class Family {
    static std::size_t identifier() noexcept {
        static std::size_t value = 0;
        return value++;
    }

public:
    template<typename>
    static std::size_t type() noexcept {
        static const std::size_t value = identifier();
        return value;
    }
};

#endif //FAMILY_HPP
