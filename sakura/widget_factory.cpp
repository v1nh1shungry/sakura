#include "widget_factory.h"
#include "utility.h"
#include <fmt/core.h>
#include <stdexcept>

namespace sakura {

WidgetFactory::WidgetFactory(ResourceManager &resources)
    : resources_(resources) {}

// PushButton:
// {
//   "class": "push_button",
//   "shape": {
//     "left": number,
//     "top": number,
//     "width": number,
//     "height": number,
//     "texture": <optional> string
//   },
//   "text": <optional> string,
//   "actions": {
//     <optional> "clicked": string
//   }
// }

// Dialog:
// {
//   "class": "dialog",
//   "shape": {
//     "left": number,
//     "right": number,
//     "width": number,
//     "height": number,
//     "texture": <optional> string
//   },
//   "text": {
//     "left": number,
//     "top": number
//   },
//   "name": {
//     "left": number,
//     "top": number
//   },
//   "children": [
//     <optional>
//   ]
// }

std::unique_ptr<Widget>
WidgetFactory::from(const nlohmann::json &config,
                    const nlohmann::json &global) const {
  std::string type;
  if (exists<nlohmann::json::value_t::string>(config, "class")) {
    type = config["class"].get<std::string>();
  } else {
    throw std::runtime_error("empty widget class name");
  }
  if (type == "push_button") {
    return createPushButton(config, global);
  } else if (type == "dialog") {
    return createDialog(config, global);
  } else {
    throw std::runtime_error(fmt::format("{}: invalid widget type", type));
  }
}

std::unique_ptr<Widget>
WidgetFactory::createPushButton(const nlohmann::json &config,
                                const nlohmann::json &global) const {
  auto button = new PushButton;

  if (!exists<nlohmann::json::value_t::object>(config, "shape")) {
    throw std::runtime_error(
        "<Anonymous PushButton>: expects shape configuration");
  }
  auto shape = config["shape"];

  // TODO: check "left", "top" and so on
  button->shape.setPosition(
      {shape["left"].get<float>(), shape["top"].get<float>()});
  button->shape.setSize(
      {shape["width"].get<float>(), shape["height"].get<float>()});
  if (exists<nlohmann::json::value_t::string>(shape, "texture")) {
    button->shape.setTexture(
        resources_.loadTexture(shape["texture"].get<std::string>()).get());
  }

  button->text.setFont(
      *resources_.loadFont(global["font_face"].get<std::string>()));
  button->text.setCharacterSize(global["font_size"].get<int>());
  if (exists<nlohmann::json::value_t::string>(config, "text")) {
    button->setText(config["text"].get<std::string>());
  }

  for (auto &action : config["actions"].items()) {
    button->actions[action.key()] = action.value();
  }

  return std::unique_ptr<Widget>{button};
}

std::unique_ptr<Widget>
WidgetFactory::createDialog(const nlohmann::json &config,
                            const nlohmann::json &global) const {
  auto dialog = new Dialog;

  if (!exists<nlohmann::json::value_t::object>(config, "shape")) {
    throw std::runtime_error("<Anonymous Dialog>: expects shape configuration");
  }
  if (!exists<nlohmann::json::value_t::object>(config, "text")) {
    throw std::runtime_error("<Anonymous Dialog>: expects text configuration");
  }
  if (!exists<nlohmann::json::value_t::object>(config, "name")) {
    throw std::runtime_error("<Anonymous Dialog>: expects name configuration");
  }

  auto shape = config["shape"];
  auto text = config["text"];
  auto name = config["name"];

  // TODO: check "left", "top" and so on
  dialog->shape.setPosition(
      {shape["left"].get<float>(), shape["top"].get<float>()});
  dialog->shape.setSize(
      {shape["width"].get<float>(), shape["height"].get<float>()});
  if (exists<nlohmann::json::value_t::string>(shape, "texture")) {
    dialog->shape.setTexture(
        resources_.loadTexture(shape["texture"].get<std::string>()).get());
  }

  dialog->text.setPosition(
      {text["left"].get<float>(), text["top"].get<float>()});
  dialog->text.setFont(
      *resources_.loadFont(global["font_face"].get<std::string>()));
  dialog->text.setCharacterSize(global["font_size"].get<int>());

  dialog->name.setPosition(
      {name["left"].get<float>(), name["top"].get<float>()});
  dialog->name.setFont(
      *resources_.loadFont(global["font_face"].get<std::string>()));
  dialog->name.setCharacterSize(global["font_size"].get<int>());

  for (auto &child : config["children"]) {
    dialog->children.push_back(from(child, global));
  }

  return std::unique_ptr<Widget>{dialog};
}

} // namespace sakura
