#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../lib_window/window.h"
#include "../lib_window/button.h"

class TitleScreen : public Window {
   private:
    std::vector<Button> buttons;
    sf::Text framerate_display;
    sf::Clock clock;

    float framerate = 0;

   public:
    TitleScreen();

    void setup() override;
    bool run() override;
};
