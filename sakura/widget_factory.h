#ifndef SAKURA_WIDGET_FACTORY_H
#define SAKURA_WIDGET_FACTORY_H

#include "resource_manager.h"
#include "widget.h"
#include <memory>
#include <nlohmann/json.hpp>

namespace sakura {

class WidgetFactory {
public:
  explicit WidgetFactory(ResourceManager &resources);
  std::unique_ptr<Widget> from(const nlohmann::json &config,
                               const nlohmann::json &global) const;

private:
  std::unique_ptr<Widget> createPushButton(const nlohmann::json &config, const nlohmann::json &global) const;
  std::unique_ptr<Widget> createDialog(const nlohmann::json &config, const nlohmann::json &global) const;

private:
  ResourceManager &resources_;
};

} // namespace sakura

#endif // !SAKURA_WIDGET_FACTORY_H
