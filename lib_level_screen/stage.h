#pragma once

#include "../lib_window/drawable.h"
#include "../lib_window/button.h"
#include "../lib_window/style.h"

#include <iostream>
#include <fstream>

class Stage : public Drawable
{
private:
    Window::event_handler_t handler;

    sf::Texture background_tex;
    sf::Sprite background;
    std::vector<Button *> level_buttons;
    int num_buttons = 0;

    std::string path;

public:
    Stage(Window *window, int stage_number, float scale);
    ~Stage();

    void render(Window *window) override;

    void setYOffset(int offset);

    static bool onMouseMove(sf::Event &event, void *);
    static bool onMousePress(sf::Event &event, void *);
};