#ifndef SAKURA_ELAINA_SCRIPT_ENGINE_H
#define SAKURA_ELAINA_SCRIPT_ENGINE_H

#include "ast.h"
#include "object.h"
#include <filesystem>
#include <functional>
#include <stack>
#include <string>
#include <unordered_map>

namespace sakura {

namespace elaina {

class ScriptEngine {
public:
  ScriptEngine();
  void loadScript(const std::string &file_name, std::size_t index = 0);
  void run();
  void registerCommand(const std::string &func_name,
                       std::function<void(ScriptEngine &)> func);
  int popInt();
  std::string popString();

private:
  void pushInt(int value);
  void pushString(const std::string &value);
  void evaluate(const std::unique_ptr<Ast> &ast);

public:
  std::filesystem::path script_dir_prefix;
  bool blocked = false;
  std::unordered_map<std::string, int> variables;

private:
  std::unordered_map<std::string, std::vector<std::unique_ptr<Ast>>> scripts_;
  std::unordered_map<std::string, std::function<void(ScriptEngine &)>>
      commands_;
  std::stack<std::unique_ptr<Object>> stack_;
  decltype(scripts_)::const_iterator ptr_to_script_;
  std::size_t index_ = -1;
};

} // namespace elaina

} // namespace sakura

#endif // !SAKURA_ELAINA_SCRIPT_ENGINE_H
