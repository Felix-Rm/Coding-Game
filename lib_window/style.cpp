#include "style.h"

const sf::Color GameStyle::GRAY = {30, 30, 30};
const sf::Color GameStyle::LIGHT_GRAY = {166, 166, 166};
const sf::Color GameStyle::RED = {255, 74, 74};
const sf::Color GameStyle::ORANGE = {255, 198, 74};
const sf::Color GameStyle::YELLOW = {255, 255, 74};
const sf::Color GameStyle::GREEN = {74, 255, 74};
const sf::Color GameStyle::CYAN = {74, 255, 255};
const sf::Color GameStyle::BLUE = {74, 74, 255};
const sf::Color GameStyle::VIOLET = {255, 74, 255};
const sf::Color GameStyle::BRONZE = {222, 115, 0};
const sf::Color GameStyle::SILVER = {161, 161, 161};
const sf::Color GameStyle::GOLD = {255, 200, 0};
const sf::Color GameStyle::BLACK = {0, 0, 0};
const sf::Color GameStyle::WHITE = {255, 255, 255};

sf::Font GameStyle::game_font = sf::Font();
sf::Texture GameStyle::icons[];
bool GameStyle::loaded = false;

bool GameStyle::setup() {
    if (!game_font.loadFromFile("assets/font3.ttf"))
        return false;

    for (int i = 0; i < Icon::_COUNT; i++) {
        if (!icons[i].loadFromFile("assets/textures/icons/" + std::to_string(i) + ".png"))
            return false;
    }

    loaded = true;
    return true;
}