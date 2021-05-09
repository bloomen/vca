#include "HandlerFactory.h"
#include "Model.h"

#include "handler/SettingsBar.h"
#include "handler/SettingsFoo.h"

namespace
{

template <typename HandlerType>
HandlerType&
insertHandler(std::vector<std::unique_ptr<app::Handler>>& handlers,
              app::Model& model)
{
    auto handler = std::make_unique<HandlerType>();
    handler->setModel(model);
    auto handler_ptr = handler.get();
    handlers.push_back(std::move(handler));
    return *handler_ptr;
}

} // namespace

namespace app
{

std::vector<std::unique_ptr<Handler>>
makeHandlers(Model& model)
{
    std::vector<std::unique_ptr<Handler>> handlers;
    insertHandler<handler::SettingsBar>(handlers, model);
    insertHandler<handler::SettingsFoo>(handlers, model);
    return handlers;
}

} // namespace app
