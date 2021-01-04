#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdarg>

class Window : public sf::RenderWindow {
   public:
    typedef struct {
        bool (*ptr)(sf::Event&, void*);
        void* data;
        sf::Uint32 react_mask = 0;
    } event_handler_t;

    std::vector<event_handler_t> event_handlers;

    Window(sf::VideoMode v, std::string title, sf::Uint32 style);

    void addEventHandler(bool (*ptr)(sf::Event&, void*), void* data, int numEvents...);
    void removeEventHandler(bool (*ptr)(sf::Event&, void*), void* data);

    static event_handler_t createEventHandler(bool (*ptr)(sf::Event&, void*), void* data);

    void checkEvents();

    static bool noop(sf::Event&, void*) { return true; };

    virtual void setup() = 0;
    virtual bool run() = 0;
};