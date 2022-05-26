#include "utility.h"
#include "widget.h"

namespace sakura {

void PushButton::render(sf::RenderTarget &render_target) const {
  render_target.draw(shape);
  render_target.draw(text);
}

std::string PushButton::on(const sf::Event &event) const {
  switch (event.type) {
  case sf::Event::MouseButtonPressed: {
    auto rect = shape.getGlobalBounds();
    if (rect.left <= event.mouseButton.x &&
        event.mouseButton.x <= rect.left + rect.width &&
        rect.top <= event.mouseButton.y &&
        event.mouseButton.y <= rect.top + rect.height) {
      auto it = actions.find("clicked");
      if (it != actions.end()) {
        return it->second;
      }
    }
    return {};
  }
  default:
    return {};
  }
}

void PushButton::setText(const std::string &text) {
  this->text.setString(utf8ToWstring(text));
}

} // namespace sakura
