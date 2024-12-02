/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Family.hpp
*/

#ifndef FAMILY_HPP
#define FAMILY_HPP

#include <cstddef>
#include <type_traits>
#include "Entity.hpp"

class Family {
    static entity_id identifier() noexcept {
        static entity_id value = 0;
        return value++;
    }

public:
    template<typename>
    static entity_id type() noexcept {
        static const entity_id value = identifier();
        return value;
    }
};

#ifndef GENERATOR_API
#   if defined GENERATOR_API_EXPORT
#       define GENERATOR_API GENERATOR_EXPORT
#   elif defined GENERATOR_API_IMPORT
#       define GENERATOR_API GENERATOR_IMPORT
#   else /* No API */
#       define GENERATOR_API
#   endif
#endif
#if defined _WIN32 || defined __CYGWIN__ || defined _MSC_VER
#    define GENERATOR_EXPORT __declspec(dllexport)
#    define GENERATOR_IMPORT __declspec(dllimport)
#elif defined __GNUC__ && __GNUC__ >= 4
#    define GENERATOR_EXPORT __attribute__((visibility("default")))
#    define GENERATOR_IMPORT __attribute__((visibility("default")))
#else /* Unsupported compiler */
#    define GENERATOR_EXPORT
#    define GENERATOR_IMPORT
#endif
#if defined _MSC_VER
#   define GENERATOR_PRETTY_FUNCTION __FUNCSIG__
#elif defined __clang__ || (defined __GNUC__)
#   define GENERATOR_PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif

constexpr entity_id hash(const char* str) {
    entity_id hash = 0;
    while (*str) {
        hash = hash * 101 + static_cast<unsigned char>(*str++);
    }
    return hash;
}
struct GENERATOR_API generator {
    static entity_id next() {
        static entity_id value{};
        return value++;
    }
};

template<typename Type>
struct GENERATOR_API final_type {
#if defined GENERATOR_PRETTY_FUNCTION
    static constexpr entity_id id() {
        constexpr auto value = hash(GENERATOR_PRETTY_FUNCTION);
        return value;
    }
#else
    static entity_id id() {
        static const entity_id value = generator::next();
        return value;
    }
#endif
};

template<typename T>
struct GENERATOR_API type {
    using Type = std::decay_t<T>;
    static constexpr entity_id id() {
        return final_type<Type>::id();
    }
};

#endif //FAMILY_HPP
