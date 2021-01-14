#include "window.h"

Window::Window(sf::VideoMode v, std::string title, sf::Uint32 style) : RenderWindow(v, title, style) {
    addEventHandler([](sf::Event& event, void* data) {
        Window* obj = (Window*)data;

        obj->view_size = (sf::Vector2f)obj->getView().getSize();
        sf::Vector2f window_size = (sf::Vector2f)obj->getSize();

        obj->setView(sf::View(sf::FloatRect(0, 0, window_size.x, window_size.y)));
        obj->setup();

        return true;
    },
                    this, 1, sf::Event::Resized);

    addEventHandler([](sf::Event& event, void* data) {
        ((Window*)data)->close();
        return true;
    },
                    this, 1, sf::Event::Closed);
}

Window::event_handler_t Window::createEventHandler(bool (*ptr)(sf::Event&, void*), void* data) {
    Window::event_handler_t handler;
    handler.ptr = ptr;
    handler.data = data;
    return handler;
}

void Window::addEventHandler(bool (*ptr)(sf::Event&, void*), void* data, int numEvents, ...) {
    va_list args;
    va_start(args, numEvents);

    Window::event_handler_t handler;
    handler.ptr = ptr;
    handler.data = data;

    for (int i = 0; i < numEvents; i++) {
        handler.react_mask |= 1 << va_arg(args, int);
    }

    for (auto i = 0; i < event_handlers.size(); i++) {
        if (event_handlers[i].ptr == nullptr && event_handlers[i].data == nullptr) {
            event_handlers[i] = handler;
            return;
        }
    }
    event_handlers.push_back(handler);
}

void Window::removeEventHandler(bool (*ptr)(sf::Event&, void*), void* data) {
    for (auto i = 0; i < event_handlers.size(); i++) {
        if (event_handlers[i].ptr == ptr && event_handlers[i].data == data) {
            event_handlers[i].ptr = nullptr;
            event_handlers[i].data = nullptr;
        }
    }
}

void Window::checkEvents() {
    sf::Event event;
    while (pollEvent(event)) {
        sf::Uint32 mask = 1 << event.type;

        for (int i = 0; i < event_handlers.size(); i++) {
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

    display();

    return isOpen();
}