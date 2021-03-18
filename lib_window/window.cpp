#include "window.h"

Window::Window(sf::VideoMode video_mode, std::string title, sf::Uint32 style) : RenderWindow(video_mode, title, style) {
    this->style = style;
    this->video_mode = video_mode;

    this->view_size = (sf::Vector2f)this->getView().getSize();

    framerate_display = sf::Text("0", GameStyle::game_font, 20);
    framerate_display.setPosition({0, 0});

    addEventHandler([](sf::Event &event, void *data) {
        Window *obj = (Window *)data;

        sf::Vector2f window_size = (sf::Vector2f)obj->getSize();
        obj->setView(sf::View(sf::FloatRect(0, 0, window_size.x, window_size.y)));

        obj->view_size = (sf::Vector2f)obj->getView().getSize();
        obj->setup();

        return true;
    },
                    this, 1, sf::Event::Resized);

    addEventHandler([](sf::Event &event, void *data) {
        ((Window *)data)->close();
        return true;
    },
                    this, 1, sf::Event::Closed);
}

Window::event_handler_t Window::createEventHandler(bool (*ptr)(sf::Event &, void *), void *data) {
    Window::event_handler_t handler;
    handler.ptr = ptr;
    handler.data = data;
    return handler;
}

void Window::addEventHandler(bool (*ptr)(sf::Event &, void *), void *data, int numEvents, ...) {
    va_list args;
    va_start(args, numEvents);

    Window::event_handler_t handler;
    handler.ptr = ptr;
    handler.data = data;

    for (int i = 0; i < numEvents; i++) {
        handler.react_mask |= 1 << va_arg(args, int);
    }

    event_handlers.push_back(handler);
}

void Window::removeEventHandler(bool (*ptr)(sf::Event &, void *), void *data) {
    for (size_t i = 0; i < event_handlers.size(); i++) {
        if (event_handlers[i].ptr == ptr && event_handlers[i].data == data) {
            event_handlers.erase(event_handlers.begin() + i);
            return;
        }
    }
}

void Window::checkEvents() {
    sf::Event event;
    while (pollEvent(event)) {
        sf::Uint32 mask = 1 << event.type;
        //printf("%d\n", event.type);

        for (size_t i = 0; i < event_handlers.size(); i++) {
            if (event_handlers[i].react_mask & mask)
                if (event_handlers[i].ptr(event, event_handlers[i].data))
                    break;
        }
    }
}

bool Window::run() {
    // Process events
    checkEvents();

    render();

    mspf = clock.getElapsedTime().asMilliseconds();
    if (mspf > 0) {
        fps = (fps * 10 + 1.f / (mspf / 1000)) / 11;
        clock.restart();
    }

    char framerate_text[15];
    snprintf(framerate_text, sizeof(framerate_text), "%06.2f", fps);
    framerate_display.setString(framerate_text);

    draw(framerate_display);

    display();

    return isOpen();
}