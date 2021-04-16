#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "../lib_campaign_screen/campaign_screen.h"
#include "../lib_sandbox_screen/sandbox_screen.h"
#include "../lib_window/style.h"
#include "../lib_window/text_button.h"
#include "../lib_window/window.h"

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
