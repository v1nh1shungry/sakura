#include "utility.h"
#include <codecvt>
#include <locale>

namespace sakura {

std::wstring utf8ToWstring(const std::string &str) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> wc;
  return wc.from_bytes(str);
}

std::string concat_if_relative(const std::filesystem::path &prefix,
                               const std::string &path) {
  return std::filesystem::path{path}.is_relative() ? (prefix / path).string()
                                                   : path;
}

} // namespace sakura
