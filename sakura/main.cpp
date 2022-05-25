#include "elaina/ast.h"
#include "elaina/lexer.h"
#include "elaina/parser.h"
#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <magic_enum.hpp>
#include <nlohmann/json.hpp>
#include <string>

nlohmann::json pretty_print(std::unique_ptr<sakura::elaina::Ast> &ast) {
  switch (ast->type()) {
  case sakura::elaina::Ast::INTEGER:
    return dynamic_cast<sakura::elaina::IntegerAst *>(ast.get())->value;
  case sakura::elaina::Ast::IDENTIFIER:
    return dynamic_cast<sakura::elaina::IdentifierAst *>(ast.get())->identifier.value;
  case sakura::elaina::Ast::STRING:
    return dynamic_cast<sakura::elaina::StringAst *>(ast.get())->value;
  case sakura::elaina::Ast::EXPRESSION: {
    nlohmann::json j;
    auto ptr = dynamic_cast<sakura::elaina::ExpressionAst *>(ast.get());
    j["op"] = ptr->op.value;
    j["lhs"] = pretty_print(ptr->lhs);
    j["rhs"] = pretty_print(ptr->rhs);
    return j;
  }
  case sakura::elaina::Ast::COMMAND: {
    nlohmann::json j;
    auto ptr = dynamic_cast<sakura::elaina::CommandAst *>(ast.get());
    j["command"] = ptr->command.value;
    for (auto &arg : ptr->args) {
      j["args"].emplace_back(pretty_print(arg));
    }
    return j;
  }
  }
}

int main(int argc, char *argv[]) {
  std::string file_name = "scripts/example.ela";
  std::ifstream file(file_name);
  if (!file.is_open()) {
    std::cerr << fmt::format("failed to open file '{}'", file_name);
    return -1;
  }
  sakura::elaina::Lexer lexer(file_name, file);
  // while (lexer.hasNext()) {
  //   auto token = lexer.next();
  //   fmt::print("{} {} {} {}\n", magic_enum::enum_name(token.type),
  //              token.row_num, token.type, token.value);
  // }
  sakura::elaina::Parser parser(lexer);
  auto forest = parser.parse();
  for (auto &&ast : forest) {
    std::cout << pretty_print(ast);
  }
  return 0;
}
