#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "lib_window/window.h"
#include "lib_window/button.h"
#include "lib_window/style.h"

#include "../lib_stage_screen/stage_screen.h"

class TitleScreen : public Window {
   private:
    std::vector<Button> buttons;

    void setup() override;
    void render() override;

    static bool runCampaign(sf::Event &event, void *data);

   public:
    TitleScreen();
};
