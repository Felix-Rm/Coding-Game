#pragma once

#include <string>

#include "../lib_window/button.h"
#include "../lib_window/drawable.h"
#include "../lib_window/style.h"
#include "level_screen.h"

class LevelScreen;

class TopBar : public Drawable {
   private:
    sf::Texture background_tex;
    sf::Sprite background;
    sf::Text time_text;
    sf::Text speed_text;
    Button btn_speed_plus, btn_speed_minus;
    Button btn_stop, btn_play, btn_exit;

    float text_size;

    LevelScreen *window;

   public:
    TopBar(LevelScreen *parent_window);
    void setPosition(float x, float y) override;
    void shiftPosition(float dx, float dy) override;

    void updatePosition();

    void update();

    void render(Window *window) override;
};