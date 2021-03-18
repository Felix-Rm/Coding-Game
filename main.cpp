#include "lib_window/window.h"
#include "lib_title_screen/title_screen.h"
#include "lib_level_screen/level_screen.h"
#include "lib_window/style.h"

#include <unistd.h>
#include <iostream>
#include <stdio.h>

int main() {
    GameStyle::game_font.loadFromFile("assets/font3.ttf");

    //TitleScreen window{};
    //StageScreen window{sf::VideoMode::getDesktopMode(), "Campaign", sf::Style::Default};

    std::string level_path = "assets/stages/0/";
    LevelScreen window{
        sf::VideoMode::getDesktopMode(),
        "Level #",
        sf::Style::Default,
        0, level_path};

    while (window.run())
        ;

    return 0;
}