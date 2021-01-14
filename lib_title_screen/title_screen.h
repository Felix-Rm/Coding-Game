#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../lib_window/window.h"
#include "../lib_window/button.h"
#include "../lib_window/style.h"

#include "../lib_level_screen/level_screen.h"

class TitleScreen : public Window {
   private:
    std::vector<Button*> buttons;
    sf::Text framerate_display;
    sf::Clock clock;

    float framerate = 0;

    void setup() override;
    void render() override;

    static bool runCampaign(sf::Event& event, void* data);

   public:
    TitleScreen();
};
