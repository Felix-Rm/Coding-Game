#pragma once

#include "lib_level_screen/level_screen.h"
#include "lib_level_screen/tile.h"

class LevelEditorScreen : public LevelScreen {
   private:
    sf::RectangleShape frame;
    sf::RectangleShape cursor_outline;
    sf::Sprite cursor;
    int tile_type = 1;

    struct
    {
        unsigned int tile_pos_x;
        unsigned int tile_pos_y;
        int tile_type = 1;
    } last_tile_placement;

    void setup() override;
    void render() override;

    void updateOverlay();
    bool placeTileAtMousePos();

    void save();

    static bool onMouseButton(sf::Event& event, void* data);
    static bool onMouseMove(sf::Event& event, void* data);
    static bool onKeyEvent(sf::Event& event, void* data);

   public:
    LevelEditorScreen(sf::VideoMode video_mode, std::string title, sf::Uint32 style, int level_num, std::string& path);
};