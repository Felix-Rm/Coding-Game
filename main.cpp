#include "lib_window/window.h"
#include "lib_title_screen/title_screen.h"
#include "lib_level_screen/level_screen.h"

#include <unistd.h>
#include <iostream>
#include <stdio.h>

sf::Font game_font = sf::Font();

int main()
{
    game_font.loadFromFile("assets/font3.ttf");

    TitleScreen window{};
    //LevelScreen window{sf::VideoMode::getDesktopMode(), "Campaign", sf::Style::Default};

    while (window.run())
        ;

    return 0;
}