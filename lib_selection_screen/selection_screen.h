#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <unordered_map>

#include "lib_window/window.h"
#include "lib_window/text_button.h"
#include "lib_window/style.h"

#include "stage.h"

class SelectionScreen : public Window {
   private:
    void calculateStagePositions();
    void constrainScroll();

   protected:
    sf::Vector2i stage_size, original_stage_size;
    float stage_scaling = 1;

    std::unordered_map<int, Stage *> stages;
    std::vector<bool> stage_loaded;

    int num_stages = 0;
    int stage_cursor = 0;

    int y_scroll = 0;

    std::string stage_path;

    Window::event_handler_fnk_t onOpenLevel;

    void setup_selection();
    void render_selection();

    static bool onMouseScroll_selection(sf::Event &event, void *data);

   public:
    SelectionScreen(sf::VideoMode video_mode, std::string title, sf::Uint32 style);
};