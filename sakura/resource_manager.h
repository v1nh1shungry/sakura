#ifndef SAKURA_RESOURCE_MANAGER_H
#define SAKURA_RESOURCE_MANAGER_H

#include "scene.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

namespace sakura {

class ResourceManager {
public:
  std::shared_ptr<sf::Texture> loadTexture(const std::string &file_name);
  std::shared_ptr<sf::Music> loadMusic(const std::string &file_name);
  std::shared_ptr<sf::Font> loadFont(const std::string &file_name);
  std::shared_ptr<Scene> loadScene(const std::string &file_name);

public:
  std::unordered_map<std::string, std::filesystem::path> prefixes;

private:
  std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures_;
  std::unordered_map<std::string, std::shared_ptr<sf::Music>> pieces_of_music_;
  std::unordered_map<std::string, std::shared_ptr<sf::Font>> fonts_;
  std::unordered_map<std::string, std::shared_ptr<Scene>> scenes_;
};

} // namespace sakura

#endif // !SAKURA_RESOURCE_MANAGER_H
