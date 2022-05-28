#include "engine.h"
#include <fmt/core.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

namespace sakura {

Engine::Engine(const std::filesystem::path &dir) {
  std::filesystem::current_path(dir);

  script_engine_.registerCommand("say",
                                 std::function<void(elaina::ScriptEngine &)>{
                                     [this](elaina::ScriptEngine &se) {
                                       if (scene_->main_dialog == nullptr) {
                                         // TODO: say something
                                       } else {
                                         std::string msg = se.popString();
                                         std::string name = se.popString();
                                         scene_->main_dialog->setName(name);
                                         scene_->main_dialog->setText(msg);
                                         se.blocked = true;
                                       }
                                     }});
  script_engine_.registerCommand(
      "bgm", std::function<void(elaina::ScriptEngine &)>{
                 [this](elaina::ScriptEngine &se) {
                   if (bgm_ != nullptr) {
                     bgm_->pause();
                   }
                   bgm_ = resource_manager_.loadMusic(se.popString());
                   bgm_->setLoop(true);
                   bgm_->play();
                 }});
  script_engine_.registerCommand("pauseBgm", std::function<void(elaina::ScriptEngine &)> {
    [this](elaina::ScriptEngine &se) {
      if (bgm_ != nullptr) {
        bgm_->pause();
      }
    }
  });
  script_engine_.registerCommand(
      "background",
      std::function<void(elaina::ScriptEngine &)>{
          [this](elaina::ScriptEngine &se) {
            background_.setTexture(
                resource_manager_.loadTexture(se.popString()).get());
          }});
  script_engine_.registerCommand(
      "addSprite",
      std::function<void(elaina::ScriptEngine &)>{
          [this](elaina::ScriptEngine &se) {
            sf::Sprite sprite;
            float top = static_cast<float>(se.popInt());
            float left = static_cast<float>(se.popInt());
            std::string texture = se.popString();
            std::string name = se.popString();
            sprite.setTexture(*resource_manager_.loadTexture(texture));
            sprite.setPosition({left, top});
            sprites_.emplace(name, sprite);
          }});
  script_engine_.registerCommand("rmSprite",
                                 std::function<void(elaina::ScriptEngine &)>{
                                     [this](elaina::ScriptEngine &se) {
                                       auto it = sprites_.find(se.popString());
                                       if (it != sprites_.end()) {
                                         sprites_.erase(it);
                                       }
                                     }});
  script_engine_.registerCommand(
      "select", std::function<void(elaina::ScriptEngine &)>{
                    [this](elaina::ScriptEngine &se) {
                      auto second_action = se.popString();
                      auto second_text = se.popString();
                      auto first_action = se.popString();
                      auto first_text = se.popString();
                      scene_->select(first_text, first_action, second_text,
                                     second_action);
                      se.blocked = true;
                    }});
  script_engine_.registerCommand(
      "scene", std::function<void(elaina::ScriptEngine &)>{
                   [this](elaina::ScriptEngine &se) {
                     scene_ = resource_manager_.loadScene(se.popString());
                   }});
}

void Engine::run() {
  // try {
  //   loadProject();
  //   mainloop();
  // } catch (const std::exception &err) {
  //   this->error(err.what());
  // }
  loadProject();
  mainloop();
}

void Engine::error(const std::string &msg) {
  // TODO
}

void Engine::loadProject() {
  std::ifstream file("sakura.json");
  if (!file.is_open()) {
    throw std::runtime_error(
        "sakura.json: no such file, expects project configuration");
  }
  nlohmann::json config;
  file >> config;

  if (!config["name"].is_string()) {
    throw std::runtime_error("sakura.json: expects project name <string>");
  }
  if (config["window"].is_null()) {
    throw std::runtime_error("sakura.json: expects window configuration");
  } else {
    auto window = config["window"];
    auto width = window["width"].get<unsigned>();
    auto height = window["height"].get<unsigned>();
    window_.create(sf::VideoMode(width, height), config["name"],
                   sf::Style::Titlebar | sf::Style::Close);
    background_.setSize(
        {static_cast<float>(width), static_cast<float>(height)});
    if (window["icon"].is_string()) {
      sf::Image icon;
      icon.loadFromFile(window["icon"]);
      window_.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }
  }
  if (!config["prefixes"].is_null()) {
    for (auto &prefix : config["prefixes"].items()) {
      resource_manager_.prefixes[prefix.key()] =
          std::filesystem::path{prefix.value()};
    }
    script_engine_.script_dir_prefix = resource_manager_.prefixes["script"];
  }

  script_engine_.loadScript("entry.ela");
}

void Engine::render() {
  window_.draw(background_);
  for (auto &[_, spirite] : sprites_) {
    window_.draw(spirite);
  }
  if (scene_ != nullptr) {
    scene_->render(window_);
  }
}

void Engine::handle(const sf::Event &event) {
  switch (event.type) {
  case sf::Event::Closed:
    window_.close();
    break;
  case sf::Event::MouseButtonPressed: {
    auto action = scene_->on(event);
    if (action.empty()) {
      if (!scene_->selected) {
        script_engine_.blocked = false;
      }
    } else {
      script_engine_.loadScript(action);
      script_engine_.blocked = false;
    }
    break;
  }
  case sf::Event::KeyPressed: {
    switch (event.key.code) {
    case sf::Keyboard::Space:
    case sf::Keyboard::Enter:
      if (!scene_->selected) {
        script_engine_.blocked = false;
      }
      break;
    default:
      break;
    }
  }
  default:
    break;
  }
}

void Engine::mainloop() {
  while (window_.isOpen()) {
    sf::Event event;
    while (window_.pollEvent(event)) {
      handle(event);
    }
    script_engine_.run();
    window_.clear();
    render();
    window_.display();
  }
}

} // namespace sakura
