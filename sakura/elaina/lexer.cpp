#include "lexer.h"
#include <cctype>
#include <fmt/core.h>
#include <stdexcept>

namespace sakura {

namespace elaina {

Lexer::Lexer(std::string_view file_name, std::istream &stream)
    : file_name(file_name), stream_(stream) {}

bool Lexer::hasNext() { return peek().type != Token::END_OF_FILE; }

Token Lexer::next() {
  if (buffer_.empty()) {
    return getToken();
  } else {
    auto token = buffer_.front();
    buffer_.erase(buffer_.begin());
    return token;
  }
}

const Token &Lexer::peek(std::size_t index) {
  if (index < buffer_.size()) {
    return buffer_[index];
  } else {
    while (!reachEOF() && buffer_.size() <= index) {
      buffer_.push_back(getToken());
    }
    return index < buffer_.size() ? buffer_.back() : Token::eof();
  }
}

bool Lexer::reachEOF() const { return stream_.peek() == EOF; }

char Lexer::getChar() {
  char res = stream_.get();
  if (res == '\n') {
    row_num_ += 1;
    col_num_ = 1;
  } else {
    col_num_ += 1;
  }
  return res;
}

void Lexer::consumeWhitespace() {
  while (!reachEOF() && std::isspace(stream_.peek())) {
    getChar();
  }
}

void Lexer::consumeComment() {
  while (!reachEOF() && getChar() != '\n') {
  }
}

Token Lexer::getToken() {
  consumeWhitespace();
  if (reachEOF()) {
    return Token::eof();
  }
  char c = stream_.peek();
  if (c == ';') {
    consumeComment();
    return getToken();
  } else if (std::isdigit(c)) {
    return getInteger();
  } else if (c == '"') {
    return getString();
  } else if (c == '$') {
    return getIdentifier();
  } else if (c == '@') {
    return getCommand();
  } else if (c == '[') {
    return getNameBlock();
  } else if (c == '(') {
    getChar();
    return {Token::LPAREN, row_num_, col_num_ - 1, "("};
  } else if (c == ')') {
    getChar();
    return {Token::RPAREN, row_num_, col_num_ - 1, ")"};
  } else if (c == '+' || c == '-') {
    std::string t;
    t.push_back(getChar());
    return {Token::ADD, row_num_, col_num_ - 1, t};
  } else if (c == '*' || c == '/') {
    std::string t;
    t.push_back(getChar());
    return {Token::MUL, row_num_, col_num_ - 1, t};
  } else if (c == ':') {
    return getAssignOperator();
  } else if (c == '=' || c == '!') {
    return getEqualOperator();
  } else if (c == '<' || c == '>') {
    return getRelationOperator();
  } else {
    throw std::runtime_error(
        fmt::format("{}:{}:{}: invalid token", file_name, row_num_, col_num_));
  }
}

Token Lexer::getInteger() {
  std::string t;
  std::size_t r = row_num_;
  std::size_t c = col_num_;
  while (!reachEOF() && std::isdigit(stream_.peek())) {
    t.push_back(getChar());
  }
  return {Token::INTEGER, r, c, t};
}

Token Lexer::getString() {
  std::string t;
  std::size_t r = row_num_;
  std::size_t c = col_num_;
  getChar();
  while (!reachEOF() && stream_.peek() != '"') {
    t.push_back(getChar());
  }
  if (reachEOF()) {
    throw std::runtime_error(
        fmt::format("{}:{}:{}: unterminated string literal", file_name,
                    row_num_, col_num_));
  }
  getChar();
  return {Token::STRING, r, c, t};
}

namespace {

inline bool isword(char c) { return std::isalnum(c) || c == '_'; }

} // namespace

Token Lexer::getIdentifier() {
  std::string t;
  std::size_t r = row_num_;
  std::size_t c = col_num_;
  getChar();
  while (!reachEOF() && isword(stream_.peek())) {
    t.push_back(getChar());
  }
  if (t.empty()) {
    throw std::runtime_error(
        fmt::format("{}:{}:{}: empty identifier", file_name, r, c));
  }
  return {Token::IDENTIFIER, r, c, t};
}

Token Lexer::getCommand() {
  std::string t;
  std::size_t r = row_num_;
  std::size_t c = col_num_;
  getChar();
  while (!reachEOF() && isword(stream_.peek())) {
    t.push_back(getChar());
  }
  if (t.empty()) {
    throw std::runtime_error(
        fmt::format("{}:{}:{}: empty command", file_name, r, c));
  }
  return {Token::COMMAND, r, c, t};
}

Token Lexer::getNameBlock() {
  std::string t;
  std::size_t r = row_num_;
  std::size_t c = col_num_;
  getChar();
  while (!reachEOF() && stream_.peek() != ']') {
    t.push_back(getChar());
  }
  if (reachEOF()) {
    throw std::runtime_error(fmt::format("{}:{}:{}: unterminated name block",
                                         file_name, row_num_, col_num_));
  }
  getChar();
  return {Token::NAME_BLOCK, r, c, t};
}

Token Lexer::getAssignOperator() {
  auto r = row_num_;
  auto c = col_num_;
  char head = getChar();
  if (getChar() != '=') {
    throw std::runtime_error(fmt::format(
        "{}:{}:{}: invalid token, you mean '{}=' ?", file_name, r, c, head));
  }
  return {Token::ASSIGN, r, c, ":="};
}

Token Lexer::getEqualOperator() {
  std::string t;
  auto r = row_num_;
  auto c = col_num_;
  char head = getChar();
  t.push_back(head);
  if (getChar() != '=') {
    throw std::runtime_error(fmt::format(
        "{}:{}:{}: invalid token, you mean '{}=' ?", file_name, r, c, head));
  }
  t.push_back('=');
  return {Token::ASSIGN, r, c, t};
}

Token Lexer::getRelationOperator() {
  std::string t;
  auto r = row_num_;
  auto c = col_num_;
  t.push_back(getChar());
  if (stream_.peek() == '=') {
    t.push_back(getChar());
  }
  return {Token::RELATION, r, c, t};
}

} // namespace elaina

} // namespace sakura
