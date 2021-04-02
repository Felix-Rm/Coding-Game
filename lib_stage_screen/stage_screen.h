#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "../lib_selection_screen/selection_screen.h"

class StageScreen : public SelectionScreen {
   private:
    void setup() override;
    void render() override;

    static bool onMouseScroll(sf::Event &event, void *data);
    static bool run_level(sf::Event &event, void *data);

    ImageButton btn_exit;

   public:
    StageScreen(sf::VideoMode video_mode, std::string title, sf::Uint32 style);
};
