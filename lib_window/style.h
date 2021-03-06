#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class GameStyle {
   public:
    enum Icon {
        PLAY,
        PAUSE,
        STOP,
        EXIT,
        EDIT,
        EDITOR,
        _COUNT
    };

    static bool loaded;
    static sf::Font game_font;
    static sf::Texture icons[_COUNT];

    static const sf::Color DARK_GRAY;
    static const sf::Color GRAY;
    static const sf::Color LIGHT_GRAY;
    static const sf::Color RED;
    static const sf::Color ORANGE;
    static const sf::Color YELLOW;
    static const sf::Color GREEN;
    static const sf::Color CYAN;
    static const sf::Color BLUE;
    static const sf::Color VIOLET;
    static const sf::Color BRONZE;
    static const sf::Color SILVER;
    static const sf::Color GOLD;
    static const sf::Color BLACK;
    static const sf::Color WHITE;

    static bool setup();
};
