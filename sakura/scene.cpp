#include "scene.h"

namespace sakura {

void Scene::render(sf::RenderTarget &render_target) const {
  for (auto &widget : widgets) {
    widget->render(render_target);
  }
  main_dialog->render(render_target);
  if (selected) {
    selectors.first->render(render_target);
    selectors.second->render(render_target);
  }
}

std::string Scene::on(const sf::Event &event) {
  std::string action;
  if (selected) {
    action = selectors.first->on(event);
    if (action.empty()) {
      action = selectors.second->on(event);
    }
    if (!action.empty()) {
      selected = false;
    }
  } else {
    action = main_dialog->on(event);
    if (action.empty()) {
      for (auto &widget : widgets) {
        action = widget->on(event);
        if (!action.empty()) {
          break;
        }
      }
    }
  }
  return action;
}

void Scene::select(const std::string &first_selector_text,
                   const std::string &first_selector_action,
                   const std::string &second_selector_text,
                   const std::string &second_selector_action) {
  selected = true;
  selectors.first->setText(first_selector_text);
  selectors.first->actions["clicked"] = first_selector_action;
  selectors.second->setText(second_selector_text);
  selectors.second->actions["clicked"] = second_selector_action;
}

} // namespace sakura
