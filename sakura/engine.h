#ifndef SAKURA_ENGINE_H
#define SAKURA_ENGINE_H

#include "elaina/script_engine.h"
#include "resource_manager.h"
#include "scene.h"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

namespace sakura {

class Engine {
public:
  Engine(const std::filesystem::path &dir);
  void run();

private:
  void error(const std::string &msg);
  void loadProject();
  void render();
  void handle(const sf::Event &event);
  void mainloop();

private:
  sf::RenderWindow window_;
  elaina::ScriptEngine script_engine_;
  ResourceManager resource_manager_;
  std::unordered_map<std::string, sf::Sprite> sprites_;
  std::shared_ptr<sf::Music> bgm_;
  sf::RectangleShape background_;
  std::shared_ptr<Scene> scene_;
};

} // namespace sakura

#endif // !SAKURA_ENGINE_H
