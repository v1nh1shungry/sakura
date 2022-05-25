#include "elaina/script_engine.h"
#include <fmt/core.h>
#include <iostream>

int main(int argc, char *argv[]) {
  using namespace sakura::elaina;
  ScriptEngine se;
  se.registerCommand("say",
                     std::function<void(ScriptEngine &)>{[](ScriptEngine &se) {
                       std::string msg = se.popString();
                       std::string name = se.popString();
                       std::cerr << fmt::format("{} says \"{}\"\n", name, msg);
                     }});
  se.registerCommand("printInt",
                     std::function<void(ScriptEngine &)>{[](ScriptEngine &se) {
                       std::cerr << se.popInt() << std::endl;
                     }});
  se.loadScript("scripts/example.ela");
  while (true) {
    se.run();
  }
  return 0;
}
