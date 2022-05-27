#ifndef SAKURA_UTILITY_H
#define SAKURA_UTILITY_H

#include <filesystem>
#include <string>

namespace sakura {

std::wstring utf8ToWstring(const std::string &str);
std::string concat_if_relative(const std::filesystem::path &prefix,
                               const std::string &path);

} // namespace sakura

#endif // !SAKURA_UTILITY_H
