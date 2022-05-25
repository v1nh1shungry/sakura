#ifndef SAKURA_ELAINA_PARSER_H
#define SAKURA_ELAINA_PARSER_H

#include "ast.h"
#include "lexer.h"
#include <memory>
#include <string>
#include <vector>

namespace sakura {

namespace elaina {

class Parser {
public:
  explicit Parser(Lexer &lexer);
  std::vector<std::unique_ptr<Ast>> parse();

private:
  Token match(Token::Type type);
  std::unique_ptr<Ast> parseAst();
  std::unique_ptr<Ast> parseAssignment();
  std::unique_ptr<Ast> parseCommand();
  std::unique_ptr<Ast> parseDialogue();
  std::unique_ptr<Ast> parseExpr();
  std::unique_ptr<Ast> parseEqualExpr();
  std::unique_ptr<Ast> parseEqualExprTail();
  std::unique_ptr<Ast> parseRelationExpr();
  std::unique_ptr<Ast> parseRelationExprTail();
  std::unique_ptr<Ast> parseAddExpr();
  std::unique_ptr<Ast> parseAddExprTail();
  std::unique_ptr<Ast> parseMulExpr();
  std::unique_ptr<Ast> parseMulExprTail();

private:
  Lexer &lexer_;
};

} // namespace elaina

} // namespace sakura

#endif // !SAKURA_ELAINA_PARSER_H
