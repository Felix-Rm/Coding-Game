#include "title_screen.h"

TitleScreen::TitleScreen() : Window(sf::VideoMode::getDesktopMode(), "Info Game", sf::Style::Default) {
    setup();
}

void TitleScreen::setup() {
    sf::Vector2f size = (sf::Vector2f)this->getSize();

    setView(sf::View(sf::FloatRect(0, 0, size.x, size.y)));

    framerate_display = sf::Text("0", game_font, 20);
    framerate_display.setPosition({0, 0});

    buttons.clear();

    buttons.push_back(Button({size.x / 2, size.y / 2}, {200, 20}, "SaMpLeTxT", 20, {255, 0, 0}, {0, 255, 0}, {nullptr, nullptr}).center());
}

bool TitleScreen::run() {
    // Process events
    sf::Event event;
    while (pollEvent(event)) {
        // Close window: exit
        if (event.type == sf::Event::Closed)
            close();

        if (event.type == sf::Event::Resized)
            setup();
    }
    // Clear screen
    clear();

    framerate = (framerate * 10 + 1.f / clock.getElapsedTime().asSeconds()) / 11;
    clock.restart();

    char framerate_text[15];
    sprintf(framerate_text, "%06.2f", framerate);
    framerate_display.setString(framerate_text);

    draw(framerate_display);

    for (int i = 0; i < buttons.size(); i++) {
        buttons[i].render(this);
    }

    display();

    return isOpen();
}