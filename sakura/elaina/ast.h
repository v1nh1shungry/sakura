#ifndef SAKURA_ELAINA_AST_H
#define SAKURA_ELAINA_AST_H

#include "token.h"
#include <memory>
#include <string>
#include <vector>

namespace sakura {

namespace elaina {

struct Ast {
  enum Type { INTEGER, STRING, IDENTIFIER, EXPRESSION, COMMAND };

  virtual ~Ast() = default;
  virtual Type type() const = 0;
};

struct IntegerAst : public Ast {
  explicit IntegerAst(const std::string &value) : value(std::stoi(value)) {}
  Type type() const override { return INTEGER; }

  int value;
};

struct StringAst : public Ast {
  explicit StringAst(const std::string &value) : value(value) {}
  Type type() const override { return STRING; }

  std::string value;
};

struct IdentifierAst : public Ast {
  explicit IdentifierAst(const Token &value) : identifier(value) {}
  Type type() const override { return IDENTIFIER; }

  Token identifier;
};

struct ExpressionAst : public Ast {
  Type type() const override { return EXPRESSION; }

  Token op;
  std::unique_ptr<Ast> lhs;
  std::unique_ptr<Ast> rhs;
};

struct CommandAst : public Ast {
  Type type() const override { return COMMAND; }

  Token command;
  std::vector<std::unique_ptr<Ast>> args;
};

} // namespace elaina

} // namespace sakura

#endif // !SAKURA_ELAINA_AST_H
