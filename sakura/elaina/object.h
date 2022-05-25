#ifndef SAKURA_ELAINA_OBJECT_H
#define SAKURA_ELAINA_OBJECT_H

#include <string>

namespace sakura {

namespace elaina {

struct Object {
  enum Type { INTEGER, STRING };
  virtual ~Object() = default;
  virtual Type type() = 0;
  virtual int getInt() { return {}; }
  virtual std::string getString() { return {}; }
};

struct Integer : public Object {
  explicit Integer(int value) : value(value) {}
  Type type() override { return INTEGER; }
  int getInt() override { return value; }
  int value;
};

struct String : public Object {
  explicit String(const std::string &value) : value(value) {}
  Type type() override { return STRING; }
  std::string getString() override { return value; }
  std::string value;
};

} // namespace elaina

} // namespace sakura

#endif // !SAKURA_ELAINA_OBJECT_H
