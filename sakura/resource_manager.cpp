#include "resource_manager.h"
#include "utility.h"
#include "widget_factory.h"
#include <fmt/core.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

namespace sakura {

std::shared_ptr<sf::Texture>
ResourceManager::loadTexture(const std::string &file_name) {
  auto it = textures_.find(file_name);
  if (it == textures_.end()) {
    std::shared_ptr<sf::Texture> ptr = std::make_shared<sf::Texture>();
    if (ptr->loadFromFile(concat_if_relative(prefixes["texture"], file_name)) ==
        false) {
      throw std::runtime_error(
          fmt::format("{}: can't load texture file", file_name));
    }
    textures_[file_name] = ptr;
    return ptr;
  } else {
    return it->second;
  }
}

std::shared_ptr<sf::Music>
ResourceManager::loadMusic(const std::string &file_name) {
  auto it = pieces_of_music_.find(file_name);
  if (it == pieces_of_music_.end()) {
    std::shared_ptr<sf::Music> ptr = std::make_shared<sf::Music>();
    if (ptr->openFromFile(concat_if_relative(prefixes["music"], file_name)) ==
        false) {
      throw std::runtime_error(
          fmt::format("{}: can't load music file", file_name));
    }
    pieces_of_music_[file_name] = ptr;
    return ptr;
  } else {
    return it->second;
  }
}

std::shared_ptr<sf::Font>
ResourceManager::loadFont(const std::string &file_name) {
  auto it = fonts_.find(file_name);
  if (it == fonts_.end()) {
    std::shared_ptr<sf::Font> ptr = std::make_shared<sf::Font>();
    if (ptr->loadFromFile(concat_if_relative(prefixes["font"], file_name)) ==
        false) {
      throw std::runtime_error(
          fmt::format("{}: can't load font file", file_name));
    }
    fonts_[file_name] = ptr;
    return ptr;
  } else {
    return it->second;
  }
}

// Scene:
// {
//   "font_face": string,
//   "font_size": <optional: 24> number,
//   "main_dialog": <optional> Dialog,
//   "selector": <optional> {
//     "first": PushButton,
//     "second": PushButton
//   },
//   "widget": [
//     <optional>
//   ]
// }

std::shared_ptr<Scene>
ResourceManager::loadScene(const std::string &file_name) {
  auto it = scenes_.find(file_name);
  if (it != scenes_.end()) {
    return it->second;
  }

  std::ifstream file(concat_if_relative(prefixes["scene"], file_name));
  nlohmann::json config;
  WidgetFactory factory(*this);
  if (!file.is_open()) {
    throw std::runtime_error(
        fmt::format("{}: can't open scene config file", file_name));
  }
  file >> config;

  std::shared_ptr<Scene> scene = std::make_shared<Scene>();
  if (config["font_face"].is_null()) {
    throw std::runtime_error(
        fmt::format("{}: expects font_face configuration", file_name));
  }
  if (config["font_size"].is_null()) {
    config["font_size"] = 24;
  }
  scene->main_dialog = std::unique_ptr<Dialog>{dynamic_cast<Dialog *>(
      factory.from(config["main_dialog"], config).release())};
  scene->selectors.first =
      std::unique_ptr<PushButton>{dynamic_cast<PushButton *>(
          factory.from(config["selector"]["first"], config).release())};
  scene->selectors.second =
      std::unique_ptr<PushButton>{dynamic_cast<PushButton *>(
          factory.from(config["selector"]["second"], config).release())};
  for (auto &widget : config["widgets"]) {
    scene->widgets.push_back(factory.from(widget, config));
  }

  scenes_[file_name] = scene;
  return scene;
}

} // namespace sakura
