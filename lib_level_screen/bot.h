#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "level_screen.h"
#include "lib_helpers/helpers.h"
#include "lib_window/drawable.h"
#include "tile.h"

class LevelScreen;

class Bot : public Drawable {
   public:
    enum option {
        clockwise,
        counterclockwise,
        forward,
        backward
    };

    Bot(LevelScreen *window, sf::Vector2f pos, sf::Vector2u tile_pos);
    void setPosition(float x, float y) override;
    void setTilePosition(unsigned int x, unsigned int y);
    void shiftPosition(float dx, float dy) override;
    void setScale(float s);

    void updatePosition();

    bool rotate(option dir);
    bool drive(option dir);

    void render() override;

    void update();

    static void loadTextures();

   private:
    enum movement_state {

        DONE,
        MOVING,
        JUST_DONE
    };

    static std::string path;
    static bool textures_loaded;

    static constexpr int num_textures = 1;
    static sf::Texture textures[num_textures];

    static constexpr int updates_per_movement = 1000;

    static constexpr float delta_rotation_per_update = 45.0 / updates_per_movement;
    static sf::Vector2f delta_tile_position_per_update;

    float target_rotation = 0;
    sf::Vector2f target_position;

    sf::Sprite body;

    movement_state movement_complete = DONE;
    long time_for_movement;

    long t = 0;

    sf::Vector2f tile_position;
    float current_rotation = 0;

    LevelScreen *level;

    void applyOffset();
};
