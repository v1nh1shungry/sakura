#ifndef SAKURA_WIDGET_H
#define SAKURA_WIDGET_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace sakura {

struct Widget {
  virtual ~Widget() = default;
  virtual void render(sf::RenderTarget &render_target) const = 0;
  virtual std::string on(const sf::Event &event) const = 0;
};

struct PushButton : public Widget {
  void render(sf::RenderTarget &render_target) const override;
  std::string on(const sf::Event &event) const override;
  void setText(const std::string &text);

  sf::RectangleShape shape;
  sf::Text text;
  std::unordered_map<std::string, std::string> actions;
};

struct Dialog : public Widget {
  void render(sf::RenderTarget &render_target) const override;
  std::string on(const sf::Event &event) const override;
  void setText(const std::string &text);
  void setName(const std::string &name);

  sf::RectangleShape shape;
  sf::Text text;
  sf::Text name;
  std::vector<std::unique_ptr<Widget>> children;
};

} // namespace sakura

#endif // !SAKURA_WIDGET_H
