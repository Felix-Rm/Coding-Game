#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

class Window : public sf::RenderWindow {
   public:
    static sf::Font font;

    typedef struct {
        void (*ptr)();
        void* data;
    } event_handler_t;

    std::vector<event_handler_t> event_handlers;

    Window(sf::VideoMode v, std::string title, sf::Uint32 style);

    void addEventHandler();
    void removeEventHandler();

    virtual void setup() = 0;
    virtual bool run() = 0;
};
