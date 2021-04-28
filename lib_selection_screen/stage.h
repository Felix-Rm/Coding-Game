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
    Stage(Window *window, std::string load_path, std::string save_path, int stage_number, float scale, Window::event_handler_fnk_t event_handler);
    ~Stage();

    void render() override;

    void setPosition(float x, float y) override;
    void shiftPosition(float dx, float dy) override{};

    int getNumberOfLevels() const { return num_buttons; };

    static bool onMouseMove(sf::Event &event, void *);
    static bool onMousePress(sf::Event &event, void *);

    void activateEditMode(bool a = true);

    std::string load_path, save_path;

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

    Dialog *context = nullptr;

    bool edit_mode_activated = false;
    static bool contextMenu(sf::Event &event, void *data);
    static bool deleteLevel(sf::Event &event, void *data);
    static bool addLevel(sf::Event &event, void *data);
    static bool addLevelLeft(sf::Event &event, void *data);
    static bool addLevelRight(sf::Event &event, void *data);

    static bool run_level(sf::Event &event, void *data);

    void load();

    void shiftLevels(int start, int end, int by);

    Window::event_handler_fnk_t level_creation_provider;

    sf::FloatRect stage_size;
    int num_rows, num_columns;
    float button_spacing_x, button_spacing_y,
        button_width, button_height,
        button_text_size, button_outline_size;

    static sf::Color progress_colors[4];

    void placeButtons();
};