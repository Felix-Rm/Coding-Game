#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdarg>

#include "lib_helpers/helpers.h"
#include "style.h"

class Window : public sf::RenderWindow {
   public:
    typedef struct
    {
        bool (*ptr)(sf::Event &, void *);
        void *data;
        sf::Uint32 react_mask = 0;
    } event_handler_t;

    Window(sf::VideoMode video_mode, std::string title, sf::Uint32 style);

    void addEventHandler(bool (*ptr)(sf::Event &, void *), void *data, int numEvents...);
    void removeEventHandler(bool (*ptr)(sf::Event &, void *), void *data);

    sf::VideoMode getVideoMode() { return video_mode; };
    sf::Uint32 getStyle() { return style; };

    bool run();

    static event_handler_t createEventHandler(bool (*ptr)(sf::Event &, void *), void *data);

    static bool noop(sf::Event &, void *) { return true; };

    float fps = 0;
    float mspf = 0;

   protected:
    sf::Uint32 style;
    sf::VideoMode video_mode;

    std::vector<event_handler_t> event_handlers;

    sf::Vector2f view_size;

    virtual void render() = 0;
    virtual void setup() = 0;

    void checkEvents();

   private:
    sf::Text framerate_display;
    sf::Clock clock;
};