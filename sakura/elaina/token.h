#ifndef SAKURA_ELAINA_TOKEN_H
#define SAKURA_ELAINA_TOKEN_H

#include <string>

namespace sakura {

namespace elaina {

struct Token {
  enum Type {
    INTEGER,
    STRING,
    IDENTIFIER,
    COMMAND,
    NAME_BLOCK,
    ASSIGN,
    EQUAL,
    RELATION,
    ADD,
    MUL,
    LPAREN,
    RPAREN,
    END_OF_FILE
  } type;
  std::size_t row_num;
  std::size_t col_num;
  std::string value;

  static const Token &eof();
};

} // namespace elaina

} // namespace sakura

#endif // !SAKURA_ELAINA_TOKEN_H
