#pragma once

#include <fstream>
#include <iostream>

#include "../lib_level_screen/level_screen.h"
#include "../lib_window/drawable.h"
#include "../lib_window/style.h"
#include "../lib_window/text_button.h"
#include "../lib_window/window.h"

class Stage : public Drawable {
   public:
    Stage(Window *window, std::string path, int stage_number, float scale, Window::event_handler_fnk_t event_handler);
    ~Stage();

    void render() override;

    void setPosition(float x, float y) override;
    void shiftPosition(float dx, float dy) override{};

    int getNumberOfLevels() const { return num_buttons; };

    static bool onMouseMove(sf::Event &event, void *);
    static bool onMousePress(sf::Event &event, void *);

    std::string path;

    typedef struct {
        int level_id;
        std::string path;
        Window *selection_window;
        Window **level_window;
    } level_creation_data;

   private:
    sf::Texture background_tex;
    sf::Sprite background;

    std::vector<std::pair<Stage *, int>> level_executor_info;
    std::vector<TextButton> level_buttons;
    int num_buttons = 0;
    int stage_number = 0;

    Window::event_handler_fnk_t level_creation_provider;

    sf::FloatRect stage_size;
    int num_rows, num_columns,
        button_spacing_x, button_spacing_y,
        button_width, button_height,
        button_text_size, button_outline_size;

    static sf::Color progress_colors[4];
    static bool run_level(sf::Event &event, void *data);

    void placeButtons();
};