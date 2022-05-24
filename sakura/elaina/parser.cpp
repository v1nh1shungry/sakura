#include "parser.h"
#include <fmt/core.h>
#include <magic_enum.hpp>
#include <stdexcept>

namespace sakura {

namespace elaina {

Parser::Parser(Lexer &lexer) : lexer_(lexer) {}

std::vector<std::unique_ptr<Ast>> Parser::parse() {
  std::vector<std::unique_ptr<Ast>> res;
  while (lexer_.hasNext()) {
    res.emplace_back(parseAst());
  }
  return res;
}

std::string Parser::match(Token::Type type) {
  auto token = lexer_.next();
  if (token.type == type) {
    return token.value;
  }
  throw std::runtime_error(fmt::format(
      "{}:{}:{}: expect {}, but received '{}'", lexer_.file_name, token.row_num,
      token.col_num, magic_enum::enum_name(token.type), token.value));
}

std::unique_ptr<Ast> Parser::parseAst() {
  const auto &token = lexer_.peek();
  switch (token.type) {
  case Token::IDENTIFIER:
    return parseAssignment();
  case Token::COMMAND:
    return parseCommand();
  case Token::NAME_BLOCK:
    return parseDialogue();
  default:
    throw std::runtime_error(fmt::format("{}:{}:{}: unexpected token: '{}'",
                                         lexer_.file_name, token.row_num,
                                         token.col_num, token.value));
  }
}

std::unique_ptr<Ast> Parser::parseAssignment() {
  auto res = new CommandAst;
  res->op = "define";
  res->args.emplace_back(new StringAst{lexer_.next().value});
  match(Token::ASSIGN);
  res->args.emplace_back(parseExpr());
  return std::unique_ptr<Ast>{res};
}

std::unique_ptr<Ast> Parser::parseCommand() {
  auto res = new CommandAst;
  bool done = false;
  res->op = lexer_.next().value;
  while (!done) {
    const auto &token = lexer_.peek();
    switch (token.type) {
    case Token::INTEGER:
    case Token::LPAREN:
      res->args.emplace_back(parseExpr());
      break;
    case Token::COMMAND:
    case Token::NAME_BLOCK:
      done = true;
      break;
    case Token::IDENTIFIER:
      if (lexer_.peek(1).type == Token::ASSIGN) {
        done = true;
      } else {
        res->args.emplace_back(parseExpr());
      }
      break;
    case Token::STRING:
      res->args.emplace_back(
          std::unique_ptr<Ast>{new StringAst(lexer_.next().value)});
      break;
    default:
      throw std::runtime_error(fmt::format("{}:{}:{}: unexpected token '{}'",
                                           lexer_.file_name, token.row_num,
                                           token.col_num, token.value));
    }
  }
  return std::unique_ptr<Ast>{res};
}

std::unique_ptr<Ast> Parser::parseDialogue() {
  auto res = new CommandAst;
  res->op = "say";
  res->args.emplace_back(new StringAst{lexer_.next().value});
  res->args.emplace_back(new StringAst{match(Token::STRING)});
  return std::unique_ptr<Ast>{res};
}

namespace {

std::unique_ptr<Ast> join(std::unique_ptr<Ast> left,
                          std::unique_ptr<Ast> tail) {
  if (tail == nullptr) {
    return left;
  } else {
    auto ptr = dynamic_cast<CommandAst *>(tail.get());
    while (ptr->args.size() == 2) {
      ptr = dynamic_cast<CommandAst *>(ptr->args.front().get());
    }
    ptr->args.emplace(ptr->args.begin(), std::move(left));
    return tail;
  }
}

} // namespace

std::unique_ptr<Ast> Parser::parseExpr() {
  const auto &token = lexer_.peek();
  switch (token.type) {
  case Token::INTEGER:
  case Token::IDENTIFIER:
  case Token::LPAREN: {
    auto left = parseEqualExpr();
    auto tail = parseEqualExprTail();
    return join(std::move(left), std::move(tail));
  }
  default:
    throw std::runtime_error(fmt::format("{}:{}:{}: unexpected token '{}'",
                                         lexer_.file_name, token.row_num,
                                         token.col_num, token.value));
  }
}

std::unique_ptr<Ast> Parser::parseEqualExpr() {
  const auto &token = lexer_.peek();
  switch (token.type) {
  case Token::IDENTIFIER:
  case Token::INTEGER:
  case Token::LPAREN: {
    auto left = parseRelationExpr();
    auto tail = parseRelationExprTail();
    return join(std::move(left), std::move(tail));
  }
  default:
    throw std::runtime_error(fmt::format("{}:{}:{}: unexpected token '{}'",
                                         lexer_.file_name, token.row_num,
                                         token.col_num, token.value));
  }
}

std::unique_ptr<Ast> Parser::parseEqualExprTail() {
  const auto &token = lexer_.peek();
  switch (token.type) {
  case Token::EQUAL: {
    auto ptr = new CommandAst;
    ptr->op = lexer_.next().value;
    ptr->args.emplace_back(parseEqualExpr());
    return join(std::unique_ptr<Ast>{ptr}, parseEqualExprTail());
  }
  case Token::END_OF_FILE:
  case Token::RPAREN:
  case Token::COMMAND:
  case Token::NAME_BLOCK:
  case Token::IDENTIFIER:
  case Token::STRING:
    return nullptr;
  default:
    throw std::runtime_error(fmt::format("{}:{}:{}: unexpected token '{}'",
                                         lexer_.file_name, token.row_num,
                                         token.col_num, token.value));
  }
}

std::unique_ptr<Ast> Parser::parseRelationExpr() {
  const auto &token = lexer_.peek();
  switch (token.type) {
  case Token::INTEGER:
  case Token::IDENTIFIER:
  case Token::LPAREN: {
    auto left = parseAddExpr();
    auto tail = parseAddExprTail();
    return join(std::move(left), std::move(tail));
  }
  default:
    throw std::runtime_error(fmt::format("{}:{}:{}: unexpected token '{}'",
                                         lexer_.file_name, token.row_num,
                                         token.col_num, token.value));
  }
}

std::unique_ptr<Ast> Parser::parseRelationExprTail() {
  const auto &token = lexer_.peek();
  switch (token.type) {
  case Token::RELATION: {
    auto ptr = new CommandAst;
    ptr->op = lexer_.next().value;
    ptr->args.emplace_back(parseRelationExpr());
    return join(std::unique_ptr<Ast>{ptr}, parseRelationExprTail());
  }
  case Token::END_OF_FILE:
  case Token::RPAREN:
  case Token::EQUAL:
  case Token::IDENTIFIER:
  case Token::COMMAND:
  case Token::NAME_BLOCK:
  case Token::STRING:
    return nullptr;
  default:
    throw std::runtime_error(fmt::format("{}:{}:{}: unexpected token '{}'",
                                         lexer_.file_name, token.row_num,
                                         token.col_num, token.value));
  }
}

std::unique_ptr<Ast> Parser::parseAddExpr() {
  const auto &token = lexer_.peek();
  switch (token.type) {
  case Token::INTEGER:
  case Token::IDENTIFIER:
  case Token::LPAREN: {
    auto left = parseMulExpr();
    auto tail = parseMulExprTail();
    return join(std::move(left), std::move(tail));
  }
  default:
    throw std::runtime_error(fmt::format("{}:{}:{}: unexpected token '{}'",
                                         lexer_.file_name, token.row_num,
                                         token.col_num, token.value));
  }
}

std::unique_ptr<Ast> Parser::parseAddExprTail() {
  const auto &token = lexer_.peek();
  switch (token.type) {
  case Token::ADD: {
    auto ptr = new CommandAst;
    ptr->op = lexer_.next().value;
    ptr->args.emplace_back(parseAddExpr());
    return join(std::unique_ptr<Ast>{ptr}, parseAddExprTail());
  }
  case Token::END_OF_FILE:
  case Token::RPAREN:
  case Token::EQUAL:
  case Token::RELATION:
  case Token::IDENTIFIER:
  case Token::COMMAND:
  case Token::NAME_BLOCK:
  case Token::STRING:
    return nullptr;
  default:
    throw std::runtime_error(fmt::format("{}:{}:{}: unexpected token '{}'",
                                         lexer_.file_name, token.row_num,
                                         token.col_num, token.value));
  }
}

std::unique_ptr<Ast> Parser::parseMulExpr() {
  auto token = lexer_.next();
  switch (token.type) {
  case Token::INTEGER:
    return std::unique_ptr<Ast>{new IntegerAst{token.value}};
  case Token::IDENTIFIER:
    return std::unique_ptr<Ast>{new IdentifierAst{token.value}};
  case Token::LPAREN: {
    auto res = parseExpr();
    match(Token::RPAREN);
    return res;
  }
  default:
    throw std::runtime_error(fmt::format("{}:{}:{}: unexpected token '{}'",
                                         lexer_.file_name, token.row_num,
                                         token.col_num, token.value));
  }
}

std::unique_ptr<Ast> Parser::parseMulExprTail() {
  const auto &token = lexer_.peek();
  switch (token.type) {
  case Token::MUL: {
    auto ptr = new CommandAst;
    ptr->op = lexer_.next().value;
    ptr->args.emplace_back(parseMulExpr());
    return join(std::unique_ptr<Ast>{ptr}, parseMulExprTail());
  }
  case Token::END_OF_FILE:
  case Token::RPAREN:
  case Token::EQUAL:
  case Token::RELATION:
  case Token::ADD:
  case Token::IDENTIFIER:
  case Token::COMMAND:
  case Token::NAME_BLOCK:
  case Token::STRING:
    return nullptr;
  default:
    throw std::runtime_error(fmt::format("{}:{}:{}: unexpected token '{}'",
                                         lexer_.file_name, token.row_num,
                                         token.col_num, token.value));
  }
}

} // namespace elaina

} // namespace sakura