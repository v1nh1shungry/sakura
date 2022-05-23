#include "elaina/lexer.h"
#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <magic_enum.hpp>

int main(int argc, char *argv[]) {
  std::ifstream file("scripts/example.ela");
  if (!file.is_open()) {
    std::cerr << "failed to open file";
    exit(EXIT_FAILURE);
  }
  sakura::elaina::Lexer lexer("example.ela", file);
  while (lexer.hasNext()) {
    auto token = lexer.next();
    std::cout << fmt::format("{} {} {} {}\n", magic_enum::enum_name(token.type),
                             token.row_num, token.col_num, token.value);
  }
  return 0;
}
