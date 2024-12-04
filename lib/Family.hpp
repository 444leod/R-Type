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

/*
    This code is a simple implementation of a family class that generates a unique identifier for each type.
     sadly, this code is not working everywhere, so I will use a different approach.
*/

/*
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
*/

/**
 * @brief Preprocessor definitions for API export/import.
 *
 * These preprocessors ensure that the types can be used across different
 * compilers (clang, gnuc, or MSCV) and are not OS dependent.
 */
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

/**
 * @brief Computes a hash value for a given C-string.
 *
 * This function calculates a hash value for the input C-string using a simple
 * hashing algorithm. The algorithm multiplies the current hash value by 101
 * and adds the ASCII value of the current character.
 *
 * @param str The input C-string to be hashed.
 *
 * @return The computed hash value as a std::size_t.
 */
constexpr std::size_t hash(const char* str) {
    std::size_t hash = 0;
    while (*str) {
        hash = hash * 101 + static_cast<unsigned char>(*str++);
    }
    return hash;
}

/**
 * @brief A generator structure that provides a unique incrementing value.
 *
 * This structure contains a static method `next` that returns a unique
 * incrementing value each time it is called. The value starts from 0 and
 * increments by 1 with each call.
 */
struct GENERATOR_API generator {
    /**
     * @brief Returns the next unique incrementing value.
     *
     * This static method returns a unique value that increments by 1 each time
     * it is called. The initial value is 0.
     *
     * @return The next unique incrementing value as a std::size_t.
     */
    static std::size_t next() {
        static std::size_t value{};
        return value++;
    }
};

/**
 * @brief A structure to provide a unique identifier for a given type.
 *
 * This structure uses the `hash` function to generate a unique identifier
 * for the given type. If `GENERATOR_PRETTY_FUNCTION` is defined, it uses
 * the pretty function name to generate the hash.
 *
 * @tparam Type The type for which the unique identifier is generated.
 */
template<typename Type>
struct GENERATOR_API final_type {
#if defined GENERATOR_PRETTY_FUNCTION
    /**
     * @brief Returns the unique identifier for the type.
     *
     * This static constexpr method returns a unique identifier for the type
     * using the `hash` function on the pretty function name.
     *
     * @return The unique identifier as a std::size_t.
     */
    static constexpr std::size_t id() {
        constexpr auto value = hash(GENERATOR_PRETTY_FUNCTION);
        return value;
    }
#else
    /**
     * @brief Returns the unique identifier for the type.
     *
     * This static method returns a unique identifier for the type using the
     * `generator::next` function.
     *
     * @return The unique identifier as a std::size_t.
     */
    static std::size_t id() {
        static const std::size_t value = generator::next();
        return value;
    }
#endif
};

/**
 * @brief A structure to provide the unique id of a given raw type.
 *
 * This structure will provide the unique id of a given raw type,
 * (const T, T& and T id will be the same).
 *
 * @tparam T The raw type for which the unique identifier is generated.
 */
template<typename T>
struct GENERATOR_API type {
    using Type = std::decay_t<T>;
    /**
     * @brief Returns the unique identifier for the raw type.
     *
     * This static constexpr method returns a unique identifier for the raw type
     * using the `final_type` structure.
     *
     * @return The unique identifier as a std::size_t.
     */
    static constexpr std::size_t id() {
        return final_type<Type>::id();
    }
};

#endif //FAMILY_HPP