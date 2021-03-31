#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "lib_level_editor_screen/level_editor_screen.h"
#include "lib_level_screen/level_screen.h"
#include "lib_selection_screen/selection_screen.h"

class SandboxScreen : public SelectionScreen {
   private:
    void setup() override;
    void render() override;

    static bool onMouseScroll(sf::Event &event, void *data);
    static bool onEditToggle(sf::Event &event, void *data);
    static bool run_level(sf::Event &event, void *data);
    bool edit_activated = false;

    ImageButton btn_exit, btn_edit;

   public:
    SandboxScreen(sf::VideoMode video_mode, std::string title, sf::Uint32 style);
};
