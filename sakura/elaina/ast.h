#ifndef SAKURA_ELAINA_AST_H
#define SAKURA_ELAINA_AST_H

#include "token.h"
#include <memory>
#include <string>
#include <vector>

namespace sakura {

namespace elaina {

struct Ast {
  enum Type { INTEGER, STRING, IDENTIFIER, COMMAND };
  virtual ~Ast() = default;
  virtual Type type() = 0;
};

struct IntegerAst : public Ast {
  explicit IntegerAst(const std::string &value) : value(std::stoi(value)) {}
  Type type() override { return INTEGER; }
  int value;
};

struct StringAst : public Ast {
  explicit StringAst(const std::string &value) : value(value) {}
  Type type() override { return STRING; }
  std::string value;
};

struct IdentifierAst : public Ast {
  explicit IdentifierAst(const Token &value) : identifier(value) {}
  Type type() override { return IDENTIFIER; }
  Token identifier;
};

struct CommandAst : public Ast {
  Type type() override { return COMMAND; }
  Token op;
  std::vector<std::unique_ptr<Ast>> args;
};

} // namespace elaina

} // namespace sakura

#endif // !SAKURA_ELAINA_AST_H
