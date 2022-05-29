#ifndef SAKURA_UTILITY_H
#define SAKURA_UTILITY_H

#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>

namespace sakura {

std::wstring utf8ToWstring(const std::string &str);

std::string concat_if_relative(const std::filesystem::path &prefix,
                               const std::string &path);

template <nlohmann::json::value_t Ty>
bool exists(const nlohmann::json &j, const std::string &key) {
  auto it = j.find(key);
  if (it != j.end() && it->type() == Ty) {
    return true;
  }
  return false;
}

} // namespace sakura

#endif // !SAKURA_UTILITY_H
