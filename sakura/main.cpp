#include "engine.h"
#include <fmt/core.h>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  std::string dir = "sakura";
  if (argc == 2) {
    dir = argv[1];
  }
  if (!std::filesystem::is_directory(dir)) {
    std::cerr << fmt::format("{}: no such directory", dir);
    return -1;
  }

  sakura::Engine engine(dir);
  engine.run();

  return 0;
}
