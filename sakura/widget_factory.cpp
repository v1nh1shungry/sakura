#include "widget_factory.h"
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
  if (config.is_null()) {
    return nullptr;
  }
  std::string type;
  try {
    type = config["class"].get<std::string>();
  } catch (...) {
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

  auto shape = config["shape"];
  if (shape.is_null()) {
    throw std::runtime_error(
        "<Anonymous PushButton>: expects shape configuration");
  }

  button->shape.setPosition(
      {shape["left"].get<float>(), shape["top"].get<float>()});
  button->shape.setSize(
      {shape["width"].get<float>(), shape["height"].get<float>()});
  if (!shape["texture"].is_null()) {
    button->shape.setTexture(
        resources_.loadTexture(shape["texture"].get<std::string>()).get());
  }

  if (config["text"].is_string()) {
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

  auto shape = config["shape"];
  if (shape.is_null()) {
    throw std::runtime_error("<Anonymous Dialog>: expects shape configuration");
  }
  auto text = config["text"];
  if (text.is_null()) {
    throw std::runtime_error("<Anonymous Dialog>: expects text configuration");
  }
  auto name = config["name"];
  if (name.is_null()) {
    throw std::runtime_error("<Anonymous Dialog>: expects name configuration");
  }

  dialog->shape.setPosition(
      {shape["left"].get<float>(), shape["top"].get<float>()});
  dialog->shape.setSize(
      {shape["width"].get<float>(), shape["height"].get<float>()});
  if (!shape["texture"].is_null()) {
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
