#include "utility.h"
#include "widget.h"

namespace sakura {

void Dialog::render(sf::RenderTarget &render_target) const {
  render_target.draw(shape);
  for (auto &child : children) {
    child->render(render_target);
  }
  render_target.draw(name);
  render_target.draw(text);
}

std::string Dialog::on(const sf::Event &event) const {
  for (auto &child : children) {
    auto action = child->on(event);
    if (!action.empty()) {
      return action;
    }
  }
  return {};
}

void Dialog::setText(const std::string &text) {
  this->text.setString(utf8ToWstring(text));
}

void Dialog::setName(const std::string &name) {
  this->name.setString(utf8ToWstring(name));
}

} // namespace sakura
