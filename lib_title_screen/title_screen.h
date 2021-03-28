#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "lib_window/window.h"
#include "lib_window/text_button.h"
#include "lib_window/style.h"

#include "../lib_stage_screen/stage_screen.h"
#include "../lib_sandbox_screen/sandbox_screen.h"

class TitleScreen : public Window {
   private:
    std::vector<TextButton> buttons;

    void setup() override;
    void render() override;

    static bool runCampaign(sf::Event &event, void *data);
    static bool runSandbox(sf::Event &event, void *data);

   public:
    TitleScreen();
};
