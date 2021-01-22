#include "button.h"

Button::Button(Window *window, sf::Vector2f pos, sf::Vector2f size, std::string text, float text_size, int outline_thickness, sf::Color fg_color, sf::Color bg_color, Window::event_handler_t handler) : Drawable(pos, size)
{
    this->bg_color = bg_color;
    this->window = window;
    this->handler = handler;

    this->background = sf::RectangleShape(size);
    this->background.setPosition(pos);
    this->background.setFillColor(bg_color);
    this->background.setOutlineThickness(outline_thickness);
    this->background.setOutlineColor({(sf::Uint8)(bg_color.r * 0.5), (sf::Uint8)(bg_color.g * 0.5), (sf::Uint8)(bg_color.b * 0.5)});

    this->text = sf::Text(text, game_font, text_size);
    this->text.setFillColor(fg_color);
    this->text.setStyle(sf::Text::Bold);

    sf::FloatRect text_bounds = this->text.getLocalBounds();
    this->text.setPosition({pos.x - text_bounds.left + size.x / 2 - text_bounds.width / 2, pos.y - text_bounds.top + size.y / 2 - text_bounds.height / 2});

    window->addEventHandler(onMouseMove, this, 1, sf::Event::MouseMoved);
    window->addEventHandler(onMousePress, this, 1, sf::Event::MouseButtonPressed);
}

Button::~Button()
{
    this->window->removeEventHandler(onMouseMove, this);
}

void Button::render(Window *window)
{
    window->draw(background);
    window->draw(text);
}

Button &Button::center()
{
    sf::Vector2f button_size = background.getSize();

    sf::Vector2f background_pos = background.getPosition();
    background.setPosition({background_pos.x - button_size.x / 2, background_pos.y - button_size.y / 2});

    sf::Vector2f text_pos = text.getPosition();
    text.setPosition({text_pos.x - button_size.x / 2, text_pos.y - button_size.y / 2});

    return *this;
}

bool Button::onMouseMove(sf::Event &event, void *_obj)
{
    Button *obj = (Button *)_obj;

    auto &mouseX = event.mouseMove.x;
    auto &mouseY = event.mouseMove.y;

    float hightlighting_factor = 0.5;

    if (obj->background.getGlobalBounds().contains(mouseX, mouseY) && !obj->highlighted)
    {
        obj->highlighted = true;
        obj->background.setFillColor(obj->background.getOutlineColor());

        return true;
    }

    if (!obj->background.getGlobalBounds().contains(mouseX, mouseY) && obj->highlighted)
    {
        obj->highlighted = false;
        obj->background.setFillColor(obj->bg_color);
    }

    return false;
}

bool Button::onMousePress(sf::Event &event, void *_obj)
{
    Button *obj = (Button *)_obj;

    auto &mouseX = event.mouseButton.x;
    auto &mouseY = event.mouseButton.y;

    if (obj->background.getGlobalBounds().contains(mouseX, mouseY))
    {
        obj->handler.ptr(event, obj->handler.data);

        return true;
    }

    return false;
}

void Button::setPosition(int x, int y)
{
    this->background.setPosition(x, y);

    sf::FloatRect text_bounds = this->text.getLocalBounds();
    this->text.setPosition({x - text_bounds.left + size.x / 2 - text_bounds.width / 2, y - text_bounds.top + size.y / 2 - text_bounds.height / 2});
}

void Button::shiftPosition(int dx, int dy)
{
    this->background.move(dx, dy);
    this->text.move(dx, dy);
}