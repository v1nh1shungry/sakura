#ifndef SAKURA_ELAINA_LEXER_H
#define SAKURA_ELAINA_LEXER_H

#include "token.h"
#include <iostream>
#include <string_view>
#include <vector>

namespace sakura {

namespace elaina {

class Lexer {
public:
  Lexer(std::string_view file_name, std::istream &stream);
  bool hasNext();
  Token next();
  const Token &peek(std::size_t index = 0);

private:
  bool reachEOF() const;
  char getChar();
  void consumeWhitespace();
  void consumeComment();
  Token getToken();
  Token getInteger();
  Token getString();
  Token getIdentifier();
  Token getCommand();
  Token getTag();
  Token getNameBlock();
  Token getAssignOperator();
  Token getEqualOperator();
  Token getRelationOperator();

private:
  std::string_view file_name_;
  std::istream &stream_;
  std::size_t row_num_ = 1;
  std::size_t col_num_ = 1;
  std::vector<Token> buffer_;
};

} // namespace elaina

} // namespace sakura

#endif // !SAKURA_ELAINA_LEXER_H
