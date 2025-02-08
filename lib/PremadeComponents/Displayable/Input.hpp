//
// Created by nicolas on 2/8/25.
//

#ifndef INPUT_HPP
#define INPUT_HPP

#include <PremadeComponents/Displayable/Text.hpp>
#include <PremadeComponents/Shapes/Rectangle.hpp>
#include <string>

struct Input {
    shape::Rectangle box;
    std::uint32_t maxLength;
    std::string placeHolder;
    std::string value;
    Color focused;
    Color notFocused;
    Text text;
    bool active;
};

#endif //INPUT_HPP
