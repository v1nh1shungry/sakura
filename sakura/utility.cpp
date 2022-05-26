#include "utility.h"
#include <codecvt>
#include <locale>

namespace sakura {

std::wstring utf8ToWstring(const std::string &str) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> wc;
  return wc.from_bytes(str);
}

} // namespace sakura
