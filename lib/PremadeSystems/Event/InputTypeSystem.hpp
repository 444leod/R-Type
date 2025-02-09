//
// Created by nicolas on 2/8/25.
//

#ifndef INPUTTYPESYSTEM_HPP
#define INPUTTYPESYSTEM_HPP

inline std::string keyCodeToString(const sf::Keyboard::Key key)
{
    switch (key)
    {
    case sf::Keyboard::A: return "A";
    case sf::Keyboard::B: return "B";
    case sf::Keyboard::C: return "C";
    case sf::Keyboard::D: return "D";
    case sf::Keyboard::E: return "E";
    case sf::Keyboard::F: return "F";
    case sf::Keyboard::G: return "G";
    case sf::Keyboard::H: return "H";
    case sf::Keyboard::I: return "I";
    case sf::Keyboard::J: return "J";
    case sf::Keyboard::K: return "K";
    case sf::Keyboard::L: return "L";
    case sf::Keyboard::M: return "M";
    case sf::Keyboard::N: return "N";
    case sf::Keyboard::O: return "O";
    case sf::Keyboard::P: return "P";
    case sf::Keyboard::Q: return "Q";
    case sf::Keyboard::R: return "R";
    case sf::Keyboard::S: return "S";
    case sf::Keyboard::T: return "T";
    case sf::Keyboard::U: return "U";
    case sf::Keyboard::V: return "V";
    case sf::Keyboard::W: return "W";
    case sf::Keyboard::X: return "X";
    case sf::Keyboard::Y: return "Y";
    case sf::Keyboard::Z: return "Z";
    case sf::Keyboard::Numpad0:
    case sf::Keyboard::Num0: return "0";
    case sf::Keyboard::Numpad1:
    case sf::Keyboard::Num1: return "1";
    case sf::Keyboard::Numpad2:
    case sf::Keyboard::Num2: return "2";
    case sf::Keyboard::Numpad3:
    case sf::Keyboard::Num3: return "3";
    case sf::Keyboard::Numpad4:
    case sf::Keyboard::Num4: return "4";
    case sf::Keyboard::Numpad5:
    case sf::Keyboard::Num5: return "5";
    case sf::Keyboard::Numpad6:
    case sf::Keyboard::Num6: return "6";
    case sf::Keyboard::Numpad7:
    case sf::Keyboard::Num7: return "7";
    case sf::Keyboard::Numpad8:
    case sf::Keyboard::Num8: return "8";
    case sf::Keyboard::Numpad9:
    case sf::Keyboard::Num9: return "9";
    case sf::Keyboard::Space: return " ";
    case sf::Keyboard::Comma:
    case sf::Keyboard::Period: return ".";
    default: return "";
    }
}

class InputTypeSystem final : public AEventSystem
{
  public:
    explicit InputTypeSystem() : AEventSystem("InputTypeSystem") {}

    void execute(sf::Event& event) override
    {
        _registry.view<Input>().each(
            [&](Input& input)
            {
                if (!input.active)
                    return;

                if (event.key.code == sf::Keyboard::Key::Backspace && !input.value.empty())
                    input.value.pop_back();
                else if (event.key.code == sf::Keyboard::Key::Enter)
                    input.active = false;
                else if (event.key.code == sf::Keyboard::Key::Escape)
                {
                    input.active = false;
                    input.value.clear();
                }
                else if (input.value.length() == input.maxLength)
                    return;
                else
                    input.value += keyCodeToString(event.key.code);
            });
    }
};

#endif //INPUTTYPESYSTEM_HPP
