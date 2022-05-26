#ifndef SAKURA_SCENE_H
#define SAKURA_SCENE_H

#include "widget.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace sakura {

struct Scene {
  void render(sf::RenderTarget &render_target) const;
  std::string on(const sf::Event &event);
  void select(const std::string &first_selector_text,
              const std::string &first_selector_action,
              const std::string &second_selector_text,
              const std::string &second_selector_action);

  bool selected = false;
  std::unique_ptr<Dialog> main_dialog;
  std::pair<std::unique_ptr<PushButton>, std::unique_ptr<PushButton>> selectors;
  std::vector<std::unique_ptr<Widget>> widgets;
};

} // namespace sakura

#endif // SAKURA_SCENE_H
