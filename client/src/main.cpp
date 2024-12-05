/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** main
*/
#include "Registry.hpp"
#include <SFML/Graphics.hpp>

int main(void)
{
    Registry registry;

    auto entity = registry.create();
    auto entity2 = registry.create();

    registry.addComponent<int>(entity, 42);
    registry.addComponent<int>(entity2, 21);

    // testing that views update correctly when modification in registry, with the entity id
    registry.view<int>().each([](const Entity& entity, int& value) {
        std::cout << "Entity: " << entity << ", Value: " << value << std::endl;
        value++;
    });

    // testing that views update correctly when modification in registry, without the entity id
    registry.view<int>().each([](int& value) {
        std::cout << "Value: " << value << std::endl;
        value++;
    });

    auto view = registry.view<int>();

    // testing that views update correctly when modification in registry
    registry.remove(entity);
    auto entity3 = registry.create();
    registry.addComponent<int>(entity3, 84);

    // testing that views update correctly when modification in registry, using iterators
    for (auto [entity, tuple] : view) {
        auto& [value] = tuple;
        std::cout << "Entity: " << entity << ", Value: " << value << std::endl;
    }

    return 0;
}
