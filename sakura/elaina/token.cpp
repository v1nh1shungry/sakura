#include "token.h"

namespace sakura {

namespace elaina {

const Token &Token::eof() {
  static const Token eof{Token::END_OF_FILE, 0, 0, ""};
  return eof;
}

} // namespace elaina

} // namespace sakura
