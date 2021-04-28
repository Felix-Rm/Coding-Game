#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cstdarg>
#include <vector>

#include "../lib_helpers/helpers.h"
#include "style.h"

class Window : public sf::RenderWindow {
   public:
    typedef bool (*event_handler_fnk_t)(sf::Event &, void *);
    typedef struct
    {
        event_handler_fnk_t ptr;
        void *data;
        sf::Uint32 react_mask = 0;
    } event_handler_t;

    Window(sf::VideoMode video_mode, std::string title, sf::Uint32 style);

    void addEventHandler(event_handler_fnk_t ptr, void *data, int numEvents...);
    bool removeEventHandler(event_handler_fnk_t ptr, void *data);

    sf::VideoMode getVideoMode() { return video_mode; };
    sf::Uint32 getStyle() { return style; };

    bool run();

    static event_handler_t createEventHandler(void *data, event_handler_fnk_t ptr);

    static bool event_noop(sf::Event &, void *) { return true; };
    static bool event_close(sf::Event &, void *data) {
        ((Window *)data)->close();
        return true;
    };

    float fps = 0;
    float mspf = 0;

    sf::Vector2f mouse_pos = {0, 0};
    bool mouse_clicked[3] = {false, false, false};

   protected:
    sf::Uint32 style;
    sf::VideoMode video_mode;

    std::vector<event_handler_t> event_handlers;

    sf::Vector2f view_size;

    virtual void render() = 0;
    virtual void setup() = 0;

    void checkEvents();

   private:
    static bool event_track_mouse(sf::Event &, void *);

    sf::Text framerate_display;
    sf::Clock clock;
};