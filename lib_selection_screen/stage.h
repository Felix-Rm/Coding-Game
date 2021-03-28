#pragma once

#include "lib_window/window.h"
#include "lib_window/drawable.h"
#include "lib_window/button.h"
#include "lib_window/style.h"

#include "lib_level_screen/level_screen.h"

#include <iostream>
#include <fstream>

class Stage : public Drawable {
   private:
    sf::Texture background_tex;
    sf::Sprite background;

    std::vector<std::pair<Stage *, int>> level_executor_info;
    std::vector<Button> level_buttons;
    int num_buttons = 0;

    static bool run_level(sf::Event &event, void *data);

   public:
    Stage(Window *window, std::string path, int stage_number, float scale, Window::event_handler_fnk_t event_handler);
    ~Stage();

    void render() override;

    void setPosition(float x, float y) override;
    void shiftPosition(float dx, float dy) override{};

    static bool onMouseMove(sf::Event &event, void *);
    static bool onMousePress(sf::Event &event, void *);

    std::string path;
};