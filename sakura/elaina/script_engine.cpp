#include "script_engine.h"
#include "lexer.h"
#include "parser.h"
#include <fmt/core.h>
#include <fstream>
#include <magic_enum.hpp>
#include <stdexcept>

namespace sakura {

namespace elaina {

ScriptEngine::ScriptEngine() {
  registerCommand(":=",
                  std::function<void(ScriptEngine &)>{[this](ScriptEngine &) {
                    int value = this->popInt();
                    std::string name = this->popString();
                    this->variables[name] = value;
                  }});
  registerCommand("==",
                  std::function<void(ScriptEngine &)>{[this](ScriptEngine &) {
                    int rhs = this->popInt();
                    int lhs = this->popInt();
                    this->pushInt(lhs == rhs);
                  }});
  registerCommand("!=",
                  std::function<void(ScriptEngine &)>{[this](ScriptEngine &) {
                    int rhs = this->popInt();
                    int lhs = this->popInt();
                    this->pushInt(lhs != rhs);
                  }});
  registerCommand("<",
                  std::function<void(ScriptEngine &)>{[this](ScriptEngine &) {
                    int rhs = this->popInt();
                    int lhs = this->popInt();
                    this->pushInt(lhs < rhs);
                  }});
  registerCommand("<=",
                  std::function<void(ScriptEngine &)>{[this](ScriptEngine &) {
                    int rhs = this->popInt();
                    int lhs = this->popInt();
                    this->pushInt(lhs <= rhs);
                  }});
  registerCommand(">",
                  std::function<void(ScriptEngine &)>{[this](ScriptEngine &) {
                    int rhs = this->popInt();
                    int lhs = this->popInt();
                    this->pushInt(lhs > rhs);
                  }});
  registerCommand(">=",
                  std::function<void(ScriptEngine &)>{[this](ScriptEngine &) {
                    int rhs = this->popInt();
                    int lhs = this->popInt();
                    this->pushInt(lhs >= rhs);
                  }});
  registerCommand("+",
                  std::function<void(ScriptEngine &)>{[this](ScriptEngine &) {
                    int rhs = this->popInt();
                    int lhs = this->popInt();
                    this->pushInt(lhs + rhs);
                  }});
  registerCommand("-",
                  std::function<void(ScriptEngine &)>{[this](ScriptEngine &) {
                    int rhs = this->popInt();
                    int lhs = this->popInt();
                    this->pushInt(lhs - rhs);
                  }});
  registerCommand("*",
                  std::function<void(ScriptEngine &)>{[this](ScriptEngine &) {
                    int rhs = this->popInt();
                    int lhs = this->popInt();
                    this->pushInt(lhs * rhs);
                  }});
  registerCommand("/",
                  std::function<void(ScriptEngine &)>{[this](ScriptEngine &) {
                    int rhs = this->popInt();
                    int lhs = this->popInt();
                    this->pushInt(lhs / rhs);
                  }});
}

void ScriptEngine::loadScript(const std::string &file_name, std::size_t index) {
  auto it = scripts_.find(file_name);
  if (it == scripts_.end()) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
      throw std::runtime_error(fmt::format("{}: can't open file", file_name));
    }
    Lexer lexer(file_name, file);
    Parser parser(lexer);
    scripts_.emplace(file_name, parser.parse());
    it = scripts_.find(file_name);
  }
  ptr_to_script_ = it;
  this->index_ = index;
}

void ScriptEngine::run() {
  if (blocked || ptr_to_script_->second.size() <= index_) {
    return;
  }
  evaluate(ptr_to_script_->second[index_]);
  ++index_;
}

void ScriptEngine::registerCommand(const std::string &func_name,
                                   std::function<void(ScriptEngine &)> func) {
  commands_.emplace(func_name, func);
}

int ScriptEngine::popInt() {
  if (stack_.empty()) {
    throw std::runtime_error(fmt::format("too few arguments"));
  }
  std::unique_ptr<Object> ptr{stack_.top().release()};
  stack_.pop();
  if (ptr->type() != Object::INTEGER) {
    throw std::runtime_error(fmt::format("expects {}, but received {}",
                                         magic_enum::enum_name(Object::INTEGER),
                                         magic_enum::enum_name(ptr->type())));
  }
  return ptr->getInt();
}

std::string ScriptEngine::popString() {
  if (stack_.empty()) {
    throw std::runtime_error(fmt::format("too few arguments"));
  }
  std::unique_ptr<Object> ptr{stack_.top().release()};
  stack_.pop();
  if (ptr->type() != Object::STRING) {
    throw std::runtime_error(fmt::format("expects {}, but received {}",
                                         magic_enum::enum_name(Object::STRING),
                                         magic_enum::enum_name(ptr->type())));
  }
  return ptr->getString();
}

void ScriptEngine::pushInt(int value) {
  stack_.push(std::unique_ptr<Object>(new Integer{value}));
}

void ScriptEngine::pushString(const std::string &value) {
  stack_.push(std::unique_ptr<Object>(new String{value}));
}

void ScriptEngine::evaluate(const std::unique_ptr<Ast> &ast) {
  switch (ast->type()) {
  case Ast::INTEGER: {
    auto ptr = dynamic_cast<IntegerAst *>(ast.get());
    pushInt(ptr->value);
    break;
  }
  case Ast::STRING: {
    auto ptr = dynamic_cast<StringAst *>(ast.get());
    pushString(ptr->value);
    break;
  }
  case Ast::IDENTIFIER: {
    auto ptr = dynamic_cast<IdentifierAst *>(ast.get());
    auto it = variables.find(ptr->identifier.value);
    if (it == variables.end()) {
      throw std::runtime_error(
          fmt::format("{}:{}:{}:{}: no such variable", ptr_to_script_->first,
                      ptr->identifier.row_num, ptr->identifier.col_num,
                      ptr->identifier.value));
    }
    pushInt(it->second);
    break;
  }
  case Ast::EXPRESSION: {
    auto ptr = dynamic_cast<ExpressionAst *>(ast.get());
    evaluate(ptr->lhs);
    evaluate(ptr->rhs);
    commands_[ptr->op.value](*this);
    break;
  }
  case Ast::COMMAND: {
    auto ptr = dynamic_cast<CommandAst *>(ast.get());
    auto command = commands_.find(ptr->command.value);
    if (command == commands_.end()) {
      throw std::runtime_error(fmt::format(
          "{}:{}:{}:{}: no such command", ptr_to_script_->first,
          ptr->command.row_num, ptr->command.col_num, ptr->command.value));
    }
    for (auto &arg : ptr->args) {
      evaluate(arg);
    }
    try {
      command->second(*this);
    } catch (const std::runtime_error &error) {
      throw std::runtime_error(fmt::format(
          "{}:{}:{}:{}: {}", ptr_to_script_->first, ptr->command.row_num,
          ptr->command.col_num, ptr->command.value, error.what()));
    }
    if (!stack_.empty()) {
      throw std::runtime_error(fmt::format(
          "{}:{}:{}:{}: too many arguments", ptr_to_script_->first,
          ptr->command.row_num, ptr->command.col_num, ptr->command.value));
    }
    break;
  }
  }
}

} // namespace elaina

} // namespace sakura
