#include "top_bar.h"

TopBar::TopBar(LevelScreen *parent_window) : Drawable(nullptr, {}, {}) {
    this->window = parent_window;
    this->background_tex.loadFromFile("_assets/textures/miscellaneous/top_bar.png");
    this->background.setTexture(this->background_tex);

    this->size = {this->background.getGlobalBounds().width, this->background.getGlobalBounds().height};

    this->text_size = this->size.y * 0.8;

    updatePosition();
}

void TopBar::updatePosition() {
    this->pos = {(this->window->view_size.x - this->size.x) / 2, 0};
    this->background.setPosition(this->pos);

    this->time_text = sf::Text("", GameStyle::game_font, text_size);
    this->time_text.setFillColor(GameStyle::GOLD);
    this->time_text.setStyle(sf::Text::Bold);

    this->speed_text = sf::Text("", GameStyle::game_font, text_size);
    this->speed_text.setFillColor(GameStyle::GOLD);
    this->speed_text.setStyle(sf::Text::Bold);

    update();

    this->btn_play = ImageButton(this->window, {0, 0}, {this->text_size, this->text_size}, GameStyle::Icon::PLAY);
    this->btn_play.center();

    this->btn_stop = ImageButton(this->window, {0, 0}, {this->text_size, this->text_size}, GameStyle::Icon::STOP);
    this->btn_stop.center();

    this->btn_exit = ImageButton(this->window, {0, 0}, {this->text_size, this->text_size}, GameStyle::Icon::EXIT);
    this->btn_exit.setEventHandler(sf::Mouse::Button::Left, Window::createEventHandler(this->window, Window::event_close));
    this->btn_exit.center();

    this->btn_speed_plus = TextButton(this->window, {0, 0}, {(float)(this->size.y * 0.35), (float)(this->size.y * 0.35)}, "+", this->text_size * (1 / 3.0));
    this->btn_speed_plus.setEventHandler(sf::Mouse::Button::Left, Window::createEventHandler(this, event_speed_plus));
    this->btn_speed_plus.center();

    this->btn_speed_minus = TextButton(this->window, {0, 0}, {(float)(this->size.y * 0.35), (float)(this->size.y * 0.35)}, "-", this->text_size * (1 / 3.0));
    this->btn_speed_minus.setEventHandler(sf::Mouse::Button::Left, Window::createEventHandler(this, event_speed_minus));
    this->btn_speed_minus.center();

    auto time_text_bounds = this->time_text.getLocalBounds();
    auto btn_play_bounds = this->btn_play.getLocalBounds();
    auto btn_stop_bounds = this->btn_stop.getLocalBounds();
    auto btn_exit_bounds = this->btn_exit.getLocalBounds();
    auto speed_text_bounds = this->speed_text.getLocalBounds();
    auto btn_speed_plus_bounds = this->btn_speed_plus.getLocalBounds();
    //auto btn_speed_minus_bounds = this->btn_speed_minus.getLocalBounds();

    float free_space = size.x - (time_text_bounds.width + btn_play_bounds.width + btn_stop_bounds.width + btn_exit_bounds.width + speed_text_bounds.width + btn_speed_plus_bounds.width);
    float padding = free_space / (2 + 5);

    float common_pos_y = pos.y + size.y * 0.1;
    float current_pos_x = pos.x + padding;

    this->time_text.setPosition(current_pos_x, common_pos_y - time_text_bounds.top / 2);
    current_pos_x += time_text_bounds.width + padding;

    this->btn_play.setPosition(current_pos_x, common_pos_y);
    current_pos_x += btn_play_bounds.width + padding;

    this->btn_stop.setPosition(current_pos_x, common_pos_y);
    current_pos_x += btn_stop_bounds.width + padding;

    this->btn_exit.setPosition(current_pos_x, common_pos_y);
    current_pos_x += btn_exit_bounds.width + padding;

    this->speed_text.setPosition(current_pos_x, common_pos_y - speed_text_bounds.top / 2);
    current_pos_x += speed_text_bounds.width + padding;

    this->btn_speed_plus.setPosition(current_pos_x, common_pos_y);
    this->btn_speed_minus.setPosition(current_pos_x, common_pos_y + btn_speed_plus_bounds.height + size.y * 0.1);
}

void TopBar::setPosition(float x, float y) {}
void TopBar::shiftPosition(float dx, float dy) {}

void TopBar::render() {
    this->window->draw(this->background);
    this->window->draw(this->speed_text);
    this->window->draw(this->time_text);

    btn_play.render();
    btn_stop.render();
    btn_exit.render();

    btn_speed_plus.render();
    btn_speed_minus.render();
}

void TopBar::update() {
    int time_part_minutes = this->window->elapsed_time / 60;
    int time_part_seconds = (int)this->window->elapsed_time % 60;
    int time_part_milliseconds = (int)(this->window->elapsed_time * 10) % 10;

    char buffer[15] = {' '};
    sprintf(buffer, "%02d:%02d:%01ds", time_part_minutes, time_part_seconds, time_part_milliseconds);

    this->time_text.setString(std::string(buffer));

    window->ms_per_unit_movement = window->original_ms_per_unit_movement / window->movement_multiplyer;

    sprintf(buffer, "%03dx", window->movement_multiplyer);

    this->speed_text.setString(std::string(buffer));
}

bool TopBar::event_speed_plus(sf::Event &, void *data) {
    TopBar *top_bar = ((TopBar *)data);
    LevelScreen *window = top_bar->window;

    if (window->movement_multiplyer < 128) window->movement_multiplyer *= 2;
    top_bar->update();

    return true;
};

bool TopBar::event_speed_minus(sf::Event &, void *data) {
    TopBar *top_bar = ((TopBar *)data);
    LevelScreen *window = top_bar->window;

    if (window->movement_multiplyer > 1) window->movement_multiplyer /= 2;
    top_bar->update();

    return true;
};