#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <unordered_map>

#include "../lib_window/window.h"
#include "../lib_window/button.h"
#include "../lib_window/style.h"

#include "stage.h"

class StageScreen : public Window
{
private:
    sf::Vector2i stage_size, original_stage_size;
    float stage_scaling = 1;

    std::unordered_map<int, Stage *> stages;
    std::vector<bool> stage_loaded;

    int num_stages = 0;
    int stage_cursor = 0;

    int y_scroll = 0;

    void calculateStagePositions();
    void constrainScroll();

    void setup() override;
    void render() override;

    static bool onMouseScroll(sf::Event &event, void *data);

public:
    StageScreen(sf::VideoMode v, std::string title, sf::Uint32 style);
};
